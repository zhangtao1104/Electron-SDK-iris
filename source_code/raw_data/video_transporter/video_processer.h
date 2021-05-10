#pragma once
#include <memory>
#include <thread>
#include "iris_rtc_renderer.h"
#include "iris_rtc_engine.h"
#include "iris_rtc_raw_data.h"
#include "iris_rtc_raw_data_plugin.h"
#include "i_video_frame_event_handler_base.h"

namespace agora
{
    namespace rtc
    {
        namespace electron
        {

            class VideoProcesser
            {
            private:
                std::mutex _video_frame_mutex;
                std::shared_ptr<iris::rtc::IrisRtcEngine> _iris_rtc_engine;
                std::shared_ptr<iris::rtc::IrisRtcRawData> _iris_rtc_raw_data;
                std::shared_ptr<iris::rtc::IrisRtcRenderer> _iris_rtc_renderer;
                std::shared_ptr<std::thread> _video_frame_update_thread;
                std::shared_ptr<IVideoFrameEventHandler> _video_frame_event_handler;

            public:
                explicit VideoProcesser(iris::rtc::IrisRtcEngine *irisRtcEngine);
                ~VideoProcesser();

                void EnableVideoFrameCache(const iris::rtc::IrisRtcRendererCacheConfig &cache_config,
                                           unsigned int uid, const char *channel_id = "");

                void DisableVideoFrameCache(unsigned int uid = -1,
                                            const char *channel_id = "");

                bool GetVideoFrame(iris::rtc::IrisRtcVideoFrameObserver::VideoFrame &video_frame,
                                   bool &is_new_frame, unsigned int uid,
                                   const char *channel_id = "");
            };
        }
    }
}