#include "video_source_proxy.h"
#include "node_event.h"
#include <iosfwd>
#include <sstream>
#include <fstream>


namespace agora {
    namespace rtc {
        namespace electron {
            VideoSourceProxy::VideoSourceProxy(): _statusEvent(false) {

            }

            VideoSourceProxy::~VideoSourceProxy() {
                _videoSourceEventHandlerPtr.reset();
            }

            bool VideoSourceProxy::Initialize(IVideoSourceEventHandler *videoSourceEventHandler, std::string& parameter) {
                if (_initialized) {
                    return true;
                }

                _videoSourceEventHandlerPtr.reset(videoSourceEventHandler);

    #ifdef _WIN32
                UUID uuid = { 0 };
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
                _iAgoraIpc.reset(createAgoraIpc(this));
                _iAgoraIpc->initialize(_peerId);
                _iAgoraIpc->listen();

                _messageThread = std::thread(&VideoSourceProxy::LoopMessage, this);

                std::string targetPath;
                if (!INodeProcess::getCurrentModuleFileName(targetPath)) {
                    return false;
                }

                size_t pos = targetPath.find_last_of("\\/");
                if (pos == targetPath.npos) {
                    return false;
                }

                std::stringstream ss;
                ss << INodeProcess::GetCurrentNodeProcessId();
                std::string path = targetPath.substr(0, pos + 1);
                std::string cmdname = "VideoSource";
                std::string idparam = "id:" + _peerId;
                std::string pidparam = "pid:" + ss.str();
                std::string apiParameter = "apiParameter:" + std::string(parameter);
                const char* params[] = { cmdname.c_str(), idparam.c_str(), pidparam.c_str(), apiParameter.c_str(),nullptr };
                _iNodeProcess.reset(INodeProcess::CreateNodeProcess(path.c_str(), params));
                if (!_iNodeProcess.get()) {
                    return false;
                }

                NodeEvent::WaitResult result = _statusEvent.WaitFor(5000);
                if (result != NodeEvent::WAIT_RESULT_SET) {
                    return false;
                }

                _iNodeProcess->Monitor([videoSourceEventHandler](INodeProcess* pProcess) {
                    videoSourceEventHandler->OnVideoSourceExit();
                });

                _initialized = true;
                return true;
            }

            void VideoSourceProxy::LoopMessage()
            {
                _iAgoraIpc->run();
            }

            void VideoSourceProxy::OnMessage(unsigned int msg, char* payload, unsigned int len)
            {
                if (msg == AGORA_IPC_CONNECT)
                    _statusEvent.notifyAll();

                if (!_initialized)
                    return;


                switch (msg) {
                    case AGORA_IPC_ON_EVENT:
                        {
                            CallbackParameter* _parameter = reinterpret_cast<CallbackParameter*>(payload);
                            _videoSourceEventHandlerPtr.get()->OnVideoSourceEvent(_parameter->_eventName.c_str(), _parameter->_eventData.c_str());
                        }
                        break;

                    case AGORA_IPC_ON_EVENT_WITH_BUFFER:
                        {
                            CallbackParameter* _parameter = reinterpret_cast<CallbackParameter*>(payload);
                            _videoSourceEventHandlerPtr.get()->OnVideoSourceEvent(_parameter->_eventName.c_str(), _parameter->_eventData.c_str(), _parameter->_buffer.c_str(), _parameter->_length);
                        }
                        break;
                    
                    default:
                        break;
                }
            }

            int VideoSourceProxy::CallApi(ApiTypeEngine apiType, const char* parameter, char* result)
            {
                if (_initialized) {
                    ApiParameter apiParameter;
                    apiParameter._apiType = apiType;
                    apiParameter._parameters = parameter;
                    return _iAgoraIpc->sendMessage(AGORA_IPC_CALL_API, (char *)&apiParameter, sizeof(apiParameter));
                }
                return -1;
            }

            int VideoSourceProxy::CallApi(ApiTypeEngine apiType, const char* parameter, const char* buffer, int length,  char* result)
            {
                if (_initialized) {
                    ApiParameter apiParameter;
                    apiParameter._apiType = apiType;
                    apiParameter._parameters = parameter;
                    apiParameter._buffer = buffer;
                    apiParameter.length = length;
                    return _iAgoraIpc->sendMessage(AGORA_IPC_CALL_API_WITH_BUFFER, (char *)&apiParameter, sizeof(apiParameter));
                }
                return -1;
            }
        }
    }
}