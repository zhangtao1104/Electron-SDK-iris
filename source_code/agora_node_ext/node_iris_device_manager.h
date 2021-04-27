/*
 * @Author: zhangtao@agora.io 
 * @Date: 2021-04-22 20:53:33 
 * @Last Modified by:   zhangtao@agora.io 
 * @Last Modified time: 2021-04-22 20:53:33 
 */
#pragma once
#include "nan_api.h"
#include "iris_device_manager.h"

namespace agora {
    namespace rtc {
        namespace electron {
            class NodeIrisDeviceManager : public node::ObjectWrap
            {
            public:
                explicit NodeIrisDeviceManager(v8_Isolate* isolate, iris::IrisDeviceManager* deviceManager);
                virtual ~NodeIrisDeviceManager();

                static v8_Local<v8_Object> Init(v8_Isolate *isolate, iris::IrisDeviceManager* deviceManager);
                static void CreateInstance(const v8_FunctionCallbackInfo<v8_Value> &args);
                static void CallApiAudioDevice(const Nan_FunctionCallbackInfo<v8_Value> &args);
                static void CallApiVideoDevice(const Nan_FunctionCallbackInfo<v8_Value> &args);
                static void Release(void *selfPtr);

            private:
                static Nan_Persistent<v8_Function> _constructor;
                v8_Isolate* _isolate;
                iris::IrisDeviceManager* _deviceManager;
            };  
        }
    }
}