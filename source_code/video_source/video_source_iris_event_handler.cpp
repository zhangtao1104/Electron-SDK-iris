#include "video_source_iris_event_handler.h"

namespace agora {
    namespace rtc {
        namespace electron {
            VideoSourceIrisEventhandler::VideoSourceIrisEventhandler(IAgoraIpc* ipcController)
            {
                _ipcController.reset(ipcController);
            }

            VideoSourceIrisEventhandler::~VideoSourceIrisEventhandler()
            {
                _ipcController.reset();
            }

            void VideoSourceIrisEventhandler::OnEvent(const char *event, const char *data)
            {
                CallbackParameter _parameter;
                _parameter._eventName = event;
                _parameter._eventData = data;
                _ipcController->sendMessage(AGORA_IPC_ON_EVENT, (char *)&_parameter, sizeof(_parameter));
            }

            void VideoSourceIrisEventhandler::OnEvent(const char *event, const char *data, const void *buffer, unsigned int length)
            {
                CallbackParameter _parameter;
                _parameter._eventName = event;
                _parameter._eventData = data;

                char _buffer[1024];
                memset(_buffer, '\0', 1024);
                memcpy(_buffer, buffer, length);
                _parameter._buffer = std::string(_buffer);
                _parameter._length = length;
                _ipcController->sendMessage(AGORA_IPC_ON_EVENT_WITH_BUFFER, (char *)&_parameter, sizeof(_parameter));
            }
        }
    }
}