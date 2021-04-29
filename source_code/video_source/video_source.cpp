#include "video_source.h"
#include "iris_base.h"
#include "loguru.hpp"



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
            using namespace iris;

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

                _iris_engine.reset(new IrisEngine());
                _iris_raw_data.reset(_iris_engine->iris_raw_data());
                _iris_raw_data_plugin_manager.reset(_iris_raw_data.get()->iris_raw_data_plugin_manager());
                _iris_event_handler.reset(new VideoSourceIrisEventhandler(_ipc_controller.get()));
                _iris_engine->SetEventHandler(_iris_event_handler.get());
                char result[512];
                auto ret = _iris_engine->CallApi(ApiTypeEngine::kEngineInitialize, _apiParameter.c_str(), result);
                if (ret != 0)
                {
                    LOG_F(INFO, "initialize _iris_engine initialize fail");
                    return false;
                }

                _ipc_sender.reset(new AgoraIpcDataSender());
                if (!_ipc_sender->initialize(_peerId + DATA_IPC_NAME)) {
                    return false;
                }

                LOG_F(INFO, "initialize ipc sendMessage");
                _ipc_controller->sendMessage(AGORA_IPC_SOURCE_READY, nullptr, 0);
                _initialize = true;
                return true;
            }

            void VideoSource::Run()
            {
                LOG_F(INFO, "VideoSource::Run11()");
#ifdef _WIN32
                std::string idstr = _parameter_parser->getParameter("pid");
#else
                std::string idstr = _parameter_parser->getParameter("fd");
#endif
                _process.reset(INodeProcess::OpenNodeProcess(std::atoi(idstr.c_str())));

                if (!_process.get()) {
                    LOG_F(INFO, "VideoSource process open fail");
                    return;
                }
                LOG_F(INFO, "VideoSource::Run22()");
                _process->Monitor([this](INodeProcess *) {
                    this->Exit(false);
                });

                _ipc_controller->run();
            }

            void VideoSource::Release()
            {
                delete this;
            }

            void VideoSource::OnMessage(unsigned int msg, char *payload, unsigned int len)
            {
                LOG_F(INFO, "VideoSource::OnMessage  msg: %d", msg);
                if (!_initialize)
                    return;

                switch (msg)
                {
                    case AGORA_IPC_CALL_API:
                    {
                        LOG_F(INFO, "VideoSource::OnMessage  111 ");
                        LOG_F(INFO, "VideoSource::OnMessage  111  %p", payload);
                        ApiParameter *parameter = (ApiParameter *)payload;
                        LOG_F(INFO, "VideoSource::OnMessage  23232323 ");
                        char result[512];
                        try {
                            LOG_F(INFO, "VideoSource::OnMessage  2222 ");
                            _iris_engine->CallApi(ApiTypeEngine(parameter->_apiType), parameter->_parameters, result);
                            LOG_F(INFO, "VideoSource::OnMessage  333 ");
                        } catch (std::exception& e) {
                            LOG_F(INFO, "VideoSourcePluginCallApi catch exception: %s", e.what());
                            this->OnApiError("videoSourceApiError", e.what());
                        }
                    }
                    break;

                    case AGORA_IPC_CALL_API_WITH_BUFFER:
                    {
                        ApiParameter *parameter = (ApiParameter *)payload;
                        char result[512];
                        try {
                            _iris_engine->CallApi(ApiTypeEngine(parameter->_apiType), parameter->_parameters, const_cast<char *>(parameter->_buffer), result);
                        } catch (std::exception& e) {
                            LOG_F(INFO, "VideoSourcePluginCallApi catch exception: %s", e.what());
                            this->OnApiError("videoSourceApiError", e.what());
                        }
                    }
                    break;

                    case AGORA_IPC_PLUGIN_CALL_API:
                    {   
                        LOG_F(INFO, "VideoSourcePluginCallApi xxxx");
                        if (!payload) {
                            LOG_F(INFO, "VideoSourcePluginCallApi 32323");
                        }

                        ApiParameter *parameter = (ApiParameter *)payload;
                        LOG_F(INFO, "VideoSourcePluginCallApi type: %d, msg: %s", parameter->_apiType, parameter->_parameters);
                        char result[512];
                        try {
                            LOG_F(INFO, "VideoSourcePluginCallApi 1111");
                            _iris_raw_data_plugin_manager->CallApi(ApiTypeRawDataPlugin(parameter->_apiType), parameter->_parameters, result);
                            LOG_F(INFO, "VideoSourcePluginCallApi 2222");
                        } catch (std::exception& e) {
                            LOG_F(INFO, "VideoSourcePluginCallApi catch exception: %s", e.what());
                            this->OnApiError("videoSourceApiError", e.what());
                        }
                    }
                    break;

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

            void VideoSource::OnApiError(const char* event, const char* data)
            {
                _iris_event_handler->OnEvent(event, data);
            }
        }
    }
}