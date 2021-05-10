#include "video_processer.h"
#include "loguru.hpp"

namespace agora
{
    namespace rtc
    {
        namespace electron
        {
            using namespace iris::rtc;

            VideoProcesser::VideoProcesser(iris::rtc::IrisRtcEngine *irisRtcEngine)
            {
                _iris_rtc_engine.reset(irisRtcEngine);
                _iris_rtc_raw_data.reset(_iris_rtc_engine->raw_data());
                _iris_rtc_renderer.reset(_iris_rtc_raw_data->renderer());
            }

            VideoProcesser::~VideoProcesser()
            {
                _iris_rtc_engine.reset();
                _iris_rtc_raw_data.reset();
                _iris_rtc_renderer.reset();
            }

            void VideoProcesser::EnableVideoFrameCache(const IrisRtcRendererCacheConfig &cache_config,
                                                       unsigned int uid, const char *channel_id)
            {
                LOG_F(INFO, "EnableVideoFrameCache uid: %u", uid);
                _iris_rtc_renderer->EnableVideoFrameCache(cache_config, uid, channel_id);
            }

            void VideoProcesser::DisableVideoFrameCache(unsigned int uid,
                                                        const char *channel_id)
            {
                _iris_rtc_renderer->DisableVideoFrameCache(uid, channel_id);
            }

            bool VideoProcesser::GetVideoFrame(IrisRtcVideoFrameObserver::VideoFrame &video_frame,
                                               bool &is_new_frame, unsigned int uid,
                                               const char *channel_id)
            {
                return _iris_rtc_renderer->GetVideoFrame(video_frame, is_new_frame, uid, channel_id);
            }
        }
    }
}