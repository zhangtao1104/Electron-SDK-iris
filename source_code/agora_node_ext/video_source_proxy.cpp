#include "video_source_proxy.h"

namespace agora
{
    namespace rtc
    {
        namespace electron
        {
            VideoSourceProxy::VideoSourceProxy() : _statusEvent(false)
            {
            }

            VideoSourceProxy::~VideoSourceProxy()
            {
                Clear();
            }

            bool VideoSourceProxy::Initialize(IVideoSourceEventHandler *videoSourceEventHandler, std::string &parameter)
            {
                if (_initialized)
                    return true;

                Clear();
                _videoSourceEventHandlerPtr.reset(videoSourceEventHandler);

#ifdef _WIN32
                UUID uuid = {0};
                RPC_CSTR struuid;

                if (UuidCreate(&uuid) != RPC_S_OK)
                    return false;
                if (UuidToStringA(&uuid, &struuid) != RPC_S_OK)
                    return false;
                _peerId = (LPSTR)struuid;
                RpcStringFreeA(&struuid);
#else
                uuid_t uuid;
                uuid_generate(uuid);
                uuid_string_t uid = {0};
                uuid_unparse(uuid, uid);
                _peerId = "/";
                _peerId += uid;
                _peerId = _peerId.substr(0, 20);
#endif
                LOG_F(INFO, "video_source_proxy peerId %s", _peerId.c_str());
                _iAgoraIpc.reset(createAgoraIpc(this));
                _iAgoraIpc->initialize(_peerId);
                _iAgoraIpc->listen();

                _messageThread = std::thread(&VideoSourceProxy::LoopMessage, this);
                std::string targetPath;
                if (!INodeProcess::getCurrentModuleFileName(targetPath))
                {
                    return false;
                }

                size_t pos = targetPath.find_last_of("\\/");
                if (pos == targetPath.npos)
                {
                    return false;
                }

                std::stringstream ss;
                ss << INodeProcess::GetCurrentNodeProcessId();
                std::string path = targetPath.substr(0, pos + 1);
                std::string cmdname = "VideoSource";
                std::string idparam = "id:" + _peerId;
                std::string pidparam = "pid:" + ss.str();
                std::string apiParameter = "apiParameter:" + std::string(parameter);
                const char *params[] = {cmdname.c_str(), idparam.c_str(), pidparam.c_str(), apiParameter.c_str(), nullptr};
                _iNodeProcess.reset(INodeProcess::CreateNodeProcess(path.c_str(), params));
                if (!_iNodeProcess.get())
                {
                    return false;
                }

                NodeEvent::WaitResult result = _statusEvent.WaitFor(5000);
                if (result != NodeEvent::WAIT_RESULT_SET)
                {
                    return false;
                }
                _iNodeProcess->Monitor([videoSourceEventHandler](INodeProcess *pProcess) {
                    videoSourceEventHandler->OnVideoSourceExit();
                });

                _iAgoraIpcDataReceiver.reset(new AgoraIpcDataReceiver());
                if (!_iAgoraIpcDataReceiver->initialize(_peerId + DATA_IPC_NAME, nullptr))
                {
                    _iAgoraIpcDataReceiver.reset();
                    return false;
                }

                _iAgoraIpcDataReceiver->run(true);

                _initialized = true;
                return true;
            }

            void VideoSourceProxy::LoopMessage()
            {
                _iAgoraIpc->run();
            }

            void VideoSourceProxy::OnMessage(unsigned int msg, char *payload, unsigned int len)
            {
                if (msg == AGORA_IPC_SOURCE_READY)
                {
                    LOG_F(INFO, "VideoSourceProxy::OnMessage  AGORA_IPC_SOURCE_READY");
                    _statusEvent.notifyAll();
                }

                if (!_initialized)
                    return;

                switch (msg)
                {
                case AGORA_IPC_ON_EVENT:
                {
                    auto _parameter = reinterpret_cast<CallbackParameter *>(payload);
                    if (_videoSourceEventHandlerPtr.get())
                        _videoSourceEventHandlerPtr.get()->OnVideoSourceEvent(_parameter->_eventName, _parameter->_eventData);
                }
                break;

                case AGORA_IPC_ON_EVENT_WITH_BUFFER:
                {
                    auto _parameter = reinterpret_cast<CallbackParameter *>(payload);
                    if (_videoSourceEventHandlerPtr.get())
                        _videoSourceEventHandlerPtr.get()->OnVideoSourceEvent(_parameter->_eventName, _parameter->_eventData, _parameter->_buffer, _parameter->_length);
                }
                break;

                default:
                    break;
                }
            }

            int VideoSourceProxy::CallApi(ApiTypeEngine apiType, const char *parameter, char *result)
            {
                if (_initialized)
                {
                    ApiParameter apiParameter;
                    apiParameter._apiType = apiType;
                    strncpy(apiParameter._parameters, parameter, MAX_CHAR_LENGTH);
                    return _iAgoraIpc->sendMessage(AGORA_IPC_CALL_API, (char *)&apiParameter, sizeof(apiParameter)) ? 0 : -1;
                }
                return -1;
            }

            int VideoSourceProxy::CallApi(ApiTypeEngine apiType, const char *parameter, const char *buffer, int length, char *result)
            {
                if (_initialized)
                {
                    ApiParameter apiParameter;
                    apiParameter._apiType = apiType;
                    strncpy(apiParameter._parameters, parameter, MAX_CHAR_LENGTH);
                    strncpy(apiParameter._buffer, buffer, MAX_CHAR_LENGTH);
                    apiParameter.length = length;
                    return _iAgoraIpc->sendMessage(AGORA_IPC_CALL_API_WITH_BUFFER, (char *)&apiParameter, sizeof(apiParameter)) ? 0 : -1;
                }
                return -1;
            }

            int VideoSourceProxy::PluginCallApi(ApiTypeRawDataPlugin apiType, const char *parameter, char *result)
            {
                if (_initialized)
                {
                    ApiParameter apiParameter;
                    apiParameter._apiType = apiType;
                    strncpy(apiParameter._parameters, parameter, MAX_CHAR_LENGTH);
                    return _iAgoraIpc->sendMessage(AGORA_IPC_PLUGIN_CALL_API, (char *)&apiParameter, sizeof(apiParameter)) ? 0 : -1;
                }
                return -1;
            }

            int VideoSourceProxy::EnableVideoFrameCache(const char *channelId, unsigned int uid, int width, int height)
            {
                if (_initialized)
                {
                    VideoFrameCacheConfigParameter parameter;
                    strncpy(parameter._channelId, channelId, MAX_CHAR_LENGTH);
                    parameter._uid = uid;
                    parameter._width = width;
                    parameter._height = height;
                    return _iAgoraIpc->sendMessage(AGORA_IPC_ENABLE_VIDEO_FRAME_CACHE, (char *)&parameter, sizeof(parameter)) ? 0 : -1;
                }
                return -1;
            }

            int VideoSourceProxy::DisableVideoFrameCache(const char *channelId, unsigned int uid)
            {
                if (_initialized)
                {
                    VideoFrameCacheConfigParameter parameter;
                    strncpy(parameter._channelId, channelId, MAX_CHAR_LENGTH);
                    parameter._uid = uid;
                    return _iAgoraIpc->sendMessage(AGORA_IPC_DISABLE_VIDEO_FRAME_CACHE, (char *)&parameter, sizeof(parameter)) ? 0 : -1;
                }
                return -1;
            }

            int VideoSourceProxy::Release()
            {
                if (_initialized)
                    Clear();

                return 0;
            }

            void VideoSourceProxy::Clear()
            {
                _initialized = false;
                _videoSourceEventHandlerPtr = nullptr;

                _iAgoraIpcDataReceiver.reset();
                if (_iAgoraIpc.get())
                {
                    _iAgoraIpc->sendMessage(AGORA_IPC_DISCONNECT, nullptr, 0);
                    _iAgoraIpc->disconnect();
                }
                if (_messageThread.joinable())
                    _messageThread.join();
            }
        }
    }
}