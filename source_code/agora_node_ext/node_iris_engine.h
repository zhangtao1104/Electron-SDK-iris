/*
 * @Author: zhangtao@agora.io 
 * @Date: 2021-04-22 20:53:44 
 * @Last Modified by: zhangtao@agora.io
 * @Last Modified time: 2021-04-26 22:02:06
 */
#pragma once
#include "iris_engine.h"
#include "iris_raw_data_plugin.h"
#include "nan_api.h"
#include "node_iris_event_handler.h"
#include "node_iris_channel.h"
#include "node_iris_device_manager.h"
#include "node_screen_window_info.h"
#include "video_source_proxy.h"
#include "node_log.h"
#include "node_base.h"
#include "loguru.hpp"

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
                static void VideoSourceRelease(const Nan_FunctionCallbackInfo<v8_Value> &args);

                static void SetAddonLogFile(const Nan_FunctionCallbackInfo<v8_Value> &args);
                void OnApiError(int apiType, const char *errorMessage);
                
                static void PluginCallApi(const Nan_FunctionCallbackInfo<v8_Value> &args);

            private:
                v8_Isolate *_isolate;
                static Nan_Persistent<v8_Function> _constructor;
                std::unique_ptr<iris::IrisEngine> _iris_engine;
                std::shared_ptr<iris::IrisRawData> _iris_raw_data;
                std::shared_ptr<NodeIrisEventHandler> _iris_event_handler;
                std::unique_ptr<VideoSourceProxy> _video_source_proxy;
                std::shared_ptr<iris::IrisRawDataPluginManager> _iris_raw_data_plugin_manager;
            };
        }
    }
}
