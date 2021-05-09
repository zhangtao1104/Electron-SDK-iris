#pragma once
#include <vector>

namespace agora
{
    namespace rtc
    {
        namespace electron
        {
            class IVideoFrameEventHandler
            {
            public:
                typedef std::vector<unsigned char> frame_buffer;

                typedef struct NodeVideoFrame
                {
                    NodeVideoFrame() : left(0), right(0), top(0), bottom(0), width(0), height(0), y_stride(0), u_stride(0), v_stride(0), yBuffer(nullptr), uBuffer(nullptr), vBuffer(nullptr), mirror(false), rotation(0)
                    {
                    }

                    int left;
                    int right;
                    int top;
                    int bottom;
                    int width;
                    int height;
                    int y_stride;
                    int u_stride;
                    int v_stride;
                    void *yBuffer;
                    void *uBuffer;
                    void *vBuffer;
                    bool mirror;
                    int rotation;
                    frame_buffer buffer;
                } NodeVideoFrame;

                typedef struct NodeVideoFrameHeader
                {
                    bool isNewFrame;
                    int width;
                    int height;
                    int left;
                    int right;
                    int top;
                    int bottom;
                    int rotation;
                    int timestamp;
                } NodeVideoFrameHeader;

                virtual void OnVideoFrame(NodeVideoFrame *videoFrame) = 0;
            };
        }
    }
}