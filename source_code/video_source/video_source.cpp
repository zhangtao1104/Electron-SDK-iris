#include "video_source.h"
#include "iris_base.h"

namespace agora
{
    namespace rtc
    {
        namespace electron
        {
            using namespace iris;

            int main(int argc, char *argv[])
            {
                std::string _parameter;
                for (int i = 1; i < argc; i++)
                {
                    _parameter.append(argv[i]);
                    _parameter.append(" ");
                }
                auto _videoSource = new VideoSource();
                _videoSource->initialize(_parameter);
                _videoSource->run();
                _videoSource->release();
                return 0;
            }

            VideoSource::VideoSource()
            {
            }

            VideoSource::~VideoSource()
            {
                _irisEngine.reset();
                _irisEventHandler.reset();
                _ipcController.reset();
                _ipcSender.reset();
                _parameterParser.reset();
            }

            bool VideoSource::initialize(std::string &parameter)
            {
                _parameterParser.reset(new VideoSourceParamParser());
                _parameterParser->initialize(parameter);

                auto _apiParameter = _parameterParser->getParameter("apiParameter");
                auto _peerId = _parameterParser->getParameter("id");

                if (!_ipcController->initialize(_peerId))
                {
                    return false;
                }

                if (!_ipcController->connect())
                {
                    return false;
                }

                _irisEngine.reset(new IrisEngine());
                _irisEventHandler.reset(new VideoSourceIrisEventhandler(_ipcController.get()));
                _irisEngine->SetEventHandler(_irisEventHandler.get());
                char result[512];
                auto ret = _irisEngine->CallApi(ApiTypeEngine::kEngineInitialize, _apiParameter.c_str(), result);
                if (ret != 0)
                {
                    return false;
                }
                _ipcController->sendMessage(AGORA_IPC_SOURCE_READY, nullptr, 0);
                return true;
            }

            void VideoSource::run()
            {
#ifdef _WIN32
                std::string idstr = _parameterParser->getParameter("pid");
#else
                std::string idstr = _parameterParser->getParameter("fd");
#endif
                _process.reset(INodeProcess::OpenNodeProcess(std::atoi(idstr.c_str())));

                    if (!_process.get()) return;

                _process->Monitor([this](INodeProcess *) {
                    this->exit(false);
                });

                _ipcController->run();
            }

            void VideoSource::release()
            {
                delete this;
            }

            void VideoSource::OnMessage(unsigned int msg, char *payload, unsigned int len)
            {
                if (!_initialize)
                    return;

                switch (msg)
                {
                    case AGORA_IPC_CALL_API:
                    {
                        ApiParameter *parameter = (ApiParameter *)payload;
                        char result[512];
                        _irisEngine->CallApi(parameter->_apiType, parameter->_parameters.c_str(), result);
                    }
                    break;

                    case AGORA_IPC_CALL_API_WITH_BUFFER:
                    {
                        ApiParameter *parameter = (ApiParameter *)payload;
                        char result[512];
                        _irisEngine->CallApi(parameter->_apiType, parameter->_parameters.c_str(), const_cast<char *>(parameter->_buffer.c_str()), result);
                    }
                    break;

                    case AGORA_IPC_DISCONNECT:
                    {
                        this->exit(false);
                    }

                    default:
                        break;
                }
            }

            void VideoSource::exit(bool flag)
            {
                _ipcSender->Disconnect();
                _ipcController->disconnect();
            }
        }
    }
}