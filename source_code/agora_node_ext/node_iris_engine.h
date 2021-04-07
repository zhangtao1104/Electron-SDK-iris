#pragma once
#include "iris_engine.h"
#include "nan_api.h"
#include "agora_iris_event_handler.h"
#include "node_iris_channel.h"
#include "node_iris_device_manager.h"
#include "node_screen_window_info.h"
#include "video_source_proxy.h"

namespace agora
{
    namespace rtc
    {
        namespace electron
        {
            class NodeIrisEngine : public node::ObjectWrap
            {
            public:
                explicit NodeIrisEngine(v8_Isolate *isolate);
                virtual ~NodeIrisEngine();

                static void Init(v8_Local<v8_Object> &module);
                static void CreateInstance(const v8_FunctionCallbackInfo<v8_Value> &args);
                static void CallApi(const Nan_FunctionCallbackInfo<v8_Value> &args);
                static void CallApiWithBuffer(const Nan_FunctionCallbackInfo<v8_Value> &args);
                static void OnEvent(const Nan_FunctionCallbackInfo<v8_Value> &args);
                
                static void GetChannel(const Nan_FunctionCallbackInfo<v8_Value> &args);
                static void GetDeviceManager(const Nan_FunctionCallbackInfo<v8_Value> &args);
                static void GetScreenWindowsInfo(const Nan_FunctionCallbackInfo<v8_Value> &args);
                static void GetScreenDisplaysInfo(const Nan_FunctionCallbackInfo<v8_Value> &args);

                static void VideoSourceInitialize(const Nan_FunctionCallbackInfo<v8_Value> &args);
                static void VideoSourceCallApi(const Nan_FunctionCallbackInfo<v8_Value> &args);
                static void VideoSourceCallApiWithBuffer(const Nan_FunctionCallbackInfo<v8_Value> &args);
                

            private:
                v8_Isolate *_isolate;
                static Nan_Persistent<v8_Function> _constructor;
                std::unique_ptr<iris::IrisEngine> _iris_engine;
                std::shared_ptr<AgoraIrisEventHandler> _iris_event_handler;
                std::unique_ptr<VideoSourceProxy> _video_source_proxy;
            };
        }
    }
}
