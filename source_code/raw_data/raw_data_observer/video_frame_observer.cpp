#include "video_frame_observer.h"
#include <string>
#include "document.h"
#include <stringbuffer.h>
#include <writer.h>

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

            void VideoFrameObserver::FrameSizeChangedObserver::OnFrame(unsigned int uid, int width, int height) {
                auto _itera = _video_size_map.find(uid);
                // if (_itera == _video_size_map.end()) {

                //     FrameSize size(width, height);                   
                //     _video_size_map[uid] = size;
                //     std::string _data = GetJson(uid, width, height);
                //      node_async_call::async_call([this, _data](){
                //         this->_on_video_size_changed_call_back("onFrameSizeChanged", _data.c_str());
                //      });
                // } else {
                //     if (width != _itera->second._width || height != _itera->second._height) {
                //         FrameSize size(width, height);
                //         _video_size_map[uid] = size;
                //         std::string _data = GetJson(uid, width, height);
                //         node_async_call::async_call([this, _data](){
                //             this->_on_video_size_changed_call_back("onFrameSizeChanged", _data.c_str());
                //         });
                //     }
                // }
            }

            void VideoFrameObserver::FrameSizeChangedObserver::SetOnFrameSizeChanged(std::function<void(const char*, const char*)> call_back) {
                _on_video_size_changed_call_back = call_back;
            }

            std::string VideoFrameObserver::FrameSizeChangedObserver::GetJson(int uid, int width, int height)
            {
                rapidjson::Document _document;
                rapidjson::Value _value(rapidjson::kObjectType); 
                _value.AddMember("uid", uid, _document.GetAllocator());
                _value.AddMember("width", width, _document.GetAllocator());
                _value.AddMember("height", height, _document.GetAllocator());
                rapidjson::StringBuffer _buffer;
                rapidjson::Writer<rapidjson::StringBuffer> _writer(_buffer);
                _value.Accept(_writer);
                return _buffer.GetString();
            }
        }
    }
}