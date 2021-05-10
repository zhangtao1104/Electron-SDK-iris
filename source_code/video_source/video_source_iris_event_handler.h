#pragma once
#include <memory>
#include "iris_event_handler.h"
#include "video_source_ipc.h"
#include "loguru.hpp"
#include "i_video_frame_event_handler_base.h"

namespace agora
{
    namespace rtc
    {
        namespace electron
        {
            class VideoSourceIrisEventhandler : public iris::IrisEventHandler
            {
            public:
                VideoSourceIrisEventhandler(IAgoraIpc *ipcController);
                virtual ~VideoSourceIrisEventhandler();

                virtual void OnEvent(const char *event, const char *data) override;

                virtual void OnEvent(const char *event, const char *data, const void *buffer, unsigned int length) override;

            private:
                std::shared_ptr<IAgoraIpc> _ipcController;
            };
        }
    }
}