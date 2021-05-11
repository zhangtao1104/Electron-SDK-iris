#include "video_source.h"

int main(int argc, char *argv[])
{
    std::string _parameter;
    for (int i = 1; i < argc; i++)
    {
        _parameter.append(argv[i]);
        _parameter.append(" ");
    }
    LOG_F(INFO, "VideoSource::main");
    auto _videoSource = new agora::rtc::electron::VideoSource();
    _videoSource->Initialize(_parameter);
    _videoSource->Run();
    _videoSource->Release();
    return 0;
}

namespace agora
{
    namespace rtc
    {
        namespace electron
        {
            using namespace iris::rtc;

            VideoSource::VideoSource()
            {
            }

            VideoSource::~VideoSource()
            {
                _iris_engine.reset();
                _iris_event_handler.reset();
                _ipc_controller.reset();
                _ipc_sender.reset();
                _parameter_parser.reset();
                _video_processer.reset();
            }

            bool VideoSource::Initialize(std::string &parameter)
            {
                _parameter_parser.reset(new VideoSourceParamParser());
                _parameter_parser->initialize(parameter);

                auto _apiParameter = _parameter_parser->getParameter("apiParameter");
                auto _peerId = _parameter_parser->getParameter("id");

                LOG_F(INFO, "VideoSource::Initialize");

                LOG_F(INFO, "initialize createAgoraIpc peerId: %s", _peerId.c_str());
                _ipc_controller.reset(createAgoraIpc(this));
                if (!_ipc_controller->initialize(_peerId))
                {
                    LOG_F(INFO, "initialize createAgoraIpc _ipc_controller fail");
                    return false;
                }

                if (!_ipc_controller->connect())
                {
                    LOG_F(INFO, "initialize createAgoraIpc connect fail");
                    return false;
                }

                _iris_engine.reset(new IrisRtcEngine());
                _iris_raw_data.reset(_iris_engine->raw_data());
                _iris_raw_data_plugin_manager.reset(_iris_raw_data.get()->plugin_manager());
                _iris_event_handler.reset(new VideoSourceIrisEventhandler(_ipc_controller.get()));
                _iris_rtc_renderer.reset(_iris_raw_data->renderer());
                _iris_engine->SetEventHandler(_iris_event_handler.get());
                _video_processer.reset(new VideoProcesser(_iris_engine.get()));
                char result[512];
                auto ret = _iris_engine->CallApi(ApiTypeEngine::kEngineInitialize, _apiParameter.c_str(), result);
                if (ret != 0)
                {
                    LOG_F(INFO, "initialize _iris_engine initialize fail");
                    return false;
                }

                _ipc_sender.reset(new AgoraIpcDataSender());
                if (!_ipc_sender->initialize(_peerId + DATA_IPC_NAME))
                {
                    return false;
                }

                LOG_F(INFO, "initialize ipc sendMessage");
                _ipc_controller->sendMessage(AGORA_IPC_SOURCE_READY, nullptr, 0);
                _initialize = true;
                return true;
            }

            void VideoSource::Run()
            {
#ifdef _WIN32
                std::string idstr = _parameter_parser->getParameter("pid");
#else
                std::string idstr = _parameter_parser->getParameter("fd");
#endif
                _process.reset(INodeProcess::OpenNodeProcess(std::atoi(idstr.c_str())));

                if (!_process.get())
                {
                    LOG_F(INFO, "VideoSource process open fail");
                    return;
                }
                _process->Monitor([this](INodeProcess *) {
                    this->Exit(false);
                });

                _ipc_controller->run();
            }

            void VideoSource::Release()
            {
                delete this;
            }

            void VideoSource::OnMessage(unsigned int msg, char *data, unsigned int len)
            {
                if (!_initialize)
                    return;

                switch (msg)
                {
                case AGORA_IPC_CALL_API:
                {
                    ApiParameter *parameter = (ApiParameter *)data;
                    char result[512];
                    try
                    {
                        _iris_engine->CallApi(ApiTypeEngine(parameter->_apiType), parameter->_parameters, result);
                    }
                    catch (std::exception &e)
                    {
                        LOG_F(INFO, "VideoSourcePluginCallApi catch exception: %s", e.what());
                        this->OnApiError("videoSourceApiError", e.what());
                    }
                }
                break;

                case AGORA_IPC_CALL_API_WITH_BUFFER:
                {
                    ApiParameter *parameter = (ApiParameter *)data;
                    char result[512];
                    try
                    {
                        _iris_engine->CallApi(ApiTypeEngine(parameter->_apiType), parameter->_parameters, const_cast<char *>(parameter->_buffer), result);
                    }
                    catch (std::exception &e)
                    {
                        LOG_F(INFO, "VideoSourcePluginCallApi catch exception: %s", e.what());
                        this->OnApiError("videoSourceApiError", e.what());
                    }
                }
                break;

                case AGORA_IPC_PLUGIN_CALL_API:
                {
                    ApiParameter *parameter = (ApiParameter *)data;
                    char result[512];
                    try
                    {
                        _iris_raw_data_plugin_manager->CallApi(ApiTypeRawDataPlugin(parameter->_apiType), parameter->_parameters, result);
                    }
                    catch (std::exception &e)
                    {
                        LOG_F(INFO, "VideoSourcePluginCallApi catch exception: %s", e.what());
                        this->OnApiError("videoSourceApiError", e.what());
                    }
                }
                break;

                case AGORA_IPC_ENABLE_VIDEO_FRAME_CACHE:
                {
                    VideoFrameCacheConfigParameter *_parameter = (VideoFrameCacheConfigParameter *)data;
                    IrisRtcRendererCacheConfig _cacheConfig(iris::rtc::IrisRtcVideoFrameObserver::VideoFrameType::kFrameTypeYUV420, nullptr, _parameter->_width, _parameter->_height);
                    _video_processer->EnableVideoFrameCache(_cacheConfig, _parameter->_uid, _parameter->_channelId);
                }

                case AGORA_IPC_DISABLE_VIDEO_FRAME_CACHE:
                {
                    VideoFrameCacheConfigParameter *_parameter = (VideoFrameCacheConfigParameter *)data;
                    _video_processer->DisableVideoFrameCache(_parameter->_channelId, _parameter->_uid);
                }

                case AGORA_IPC_DISCONNECT:
                {
                    this->Exit(false);
                }
                break;

                default:
                    break;
                }
            }

            void VideoSource::Exit(bool flag)
            {
                _ipc_sender->Disconnect();
                _ipc_controller->disconnect();
            }

            void VideoSource::OnApiError(const char *event, const char *data)
            {
                _iris_event_handler->OnEvent(event, data);
            }

            bool VideoSource::sendData(char *data, int len)
            {
                if (_ipc_sender)
                {
                    _ipc_sender->sendData(data, len);
                    return true;
                }
                else
                {
                    return false;
                }
            }
        }
    }
}