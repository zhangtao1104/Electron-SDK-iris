#pragma once
#include "iris_raw_data.h"
#include "video_processer.h"
#include <functional> 
#include <map>

namespace agora {
    namespace rtc {
        namespace electron {
            class VideoFrameObserver : public iris::IrisVideoFrameObserver {
                private:
                    class FrameSizeChangedObserver {
                    private:
                        typedef struct FrameSize {
                        public:
                            FrameSize(int width, int height):_width(width),_height(height) {}
                            int _width;
                            int _height;
                        } FrameSize;

                        std::function<void(const char*, const char*)> _on_video_size_changed_call_back;
                        std::map<unsigned int, FrameSize> _video_size_map;

                        std::string GetJson(int uid, int width, int height);

                    public: 
                        void SetOnFrameSizeChanged(std::function<void(const char*, const char*)> call_back);

                        void OnFrame(unsigned int uid, int width, int height);
                    };

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

                private:
                    VideoProcesser _video_processer;

            };
        }
    }
}