#pragma once
#include "iris_raw_data.h"

namespace agora {
    namespace rtc {
        namespace electron {
            class VideoFrameObserver : public iris::IrisVideoFrameObserver {
            public:
                explicit VideoFrameObserver();
                virtual ~VideoFrameObserver();

                virtual bool OnCaptureVideoFrame(iris::IrisVideoFrameObserver::VideoFrame &video_frame) override;

                virtual bool OnPreEncodeVideoFrame(iris::IrisVideoFrameObserver::VideoFrame &video_frame) override;

                virtual bool OnRenderVideoFrame(unsigned uid, iris::IrisVideoFrameObserver::VideoFrame &video_frame) override;

                virtual iris::IrisVideoFrameObserver::VideoFrameType GetVideoFormatPreference() override;

                virtual bool GetRotationApplied() override;

                virtual bool GetMirrorApplied() override;

                virtual bool GetSmoothRenderingEnabled() override;

                virtual uint32_t GetObservedFramePosition() override;

                virtual bool IsMultipleChannelFrameWanted() override;

                virtual bool OnRenderVideoFrameEx(const char *channel_id, unsigned uid, iris::IrisVideoFrameObserver::VideoFrame &video_frame) override;
            };
        }
    }
}