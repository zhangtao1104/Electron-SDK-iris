#include "video_frame_observer.h"


namespace agora {
    namespace rtc {
        namespace electron {
            VideoFrameObserver::VideoFrameObserver()
            {

            }

            VideoFrameObserver::~VideoFrameObserver()
            {

            }

            bool VideoFrameObserver::OnCaptureVideoFrame(iris::IrisVideoFrameObserver::VideoFrame &video_frame)
            {

            }

            bool VideoFrameObserver::OnPreEncodeVideoFrame(iris::IrisVideoFrameObserver::VideoFrame &video_frame)
            {

            }

            bool VideoFrameObserver::OnRenderVideoFrame(unsigned uid, iris::IrisVideoFrameObserver::VideoFrame &video_frame)
            {

            }

            iris::IrisVideoFrameObserver::VideoFrameType VideoFrameObserver::GetVideoFormatPreference()
            {

            }

            bool VideoFrameObserver::GetRotationApplied()
            {

            }

            bool VideoFrameObserver::GetMirrorApplied()
            {

            }

            bool VideoFrameObserver::GetSmoothRenderingEnabled()
            {

            }

            uint32_t VideoFrameObserver::GetObservedFramePosition()
            {

            }

            bool VideoFrameObserver::IsMultipleChannelFrameWanted()
            {

            }

            bool VideoFrameObserver::OnRenderVideoFrameEx(const char *channel_id, unsigned uid, iris::IrisVideoFrameObserver::VideoFrame &video_frame)
            {
                
            }

        }
    }
}