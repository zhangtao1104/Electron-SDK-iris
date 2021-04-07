#include "node_iris_device_manager.h"


namespace agora {
    namespace rtc {
        namespace electron {
            using namespace iris;

            NodeIrisDeviceManager::NodeIrisDeviceManager(v8_Isolate* isolate, IrisDeviceManager* deviceManager):_isolate(isolate), _deviceManager(deviceManager)
            {
                node::AddEnvironmentCleanupHook(isolate, Release, this);
            }

            NodeIrisDeviceManager::~NodeIrisDeviceManager()
            {
                _deviceManager = nullptr;
                _isolate = nullptr;
            }

            v8_Local<v8_Object> NodeIrisDeviceManager::Init(v8_Isolate *isolate, IrisDeviceManager* deviceManager)
            {
                auto _context = isolate->GetCurrentContext();
                auto _template = v8_FunctionTemplate::New(isolate, CreateInstance);
                _template->SetClassName(Nan::New<v8_String>("NodeIrisDeviceManager").ToLocalChecked());
                _template->InstanceTemplate()->SetInternalFieldCount(5);

                Nan::SetPrototypeMethod(_template, "CallApiAudioDevice", CallApiAudioDevice);
                Nan::SetPrototypeMethod(_template, "CallApiVideoDevice", CallApiVideoDevice);
                _constructor.Reset(_template->GetFunction(_context).ToLocalChecked());

                v8_Local<v8_Function> cons = _template->GetFunction(_context).ToLocalChecked();
                v8_Local<v8_External> argDeviceManager = v8_Local<v8_External>::New(isolate, v8_External::New(isolate, deviceManager));
                v8_Local<v8_Value> argv[1] = {argDeviceManager};
                v8_Local<v8_Object> jsDeviceManager = cons->NewInstance(_context, 1, argv).ToLocalChecked();
                return jsDeviceManager;   
            }
            
            void NodeIrisDeviceManager::CreateInstance(const v8_FunctionCallbackInfo<v8_Value> &args)
            {
                v8_Isolate *_isolate = args.GetIsolate();
                v8_Local<v8_External> _argDeviceManager = v8_Local<v8_External>::Cast(args[0]);
                IrisDeviceManager *_irisDeviceManager = static_cast<IrisDeviceManager *>(_argDeviceManager->Value());
                NodeIrisDeviceManager *_deviceManager = new NodeIrisDeviceManager(_isolate, _irisDeviceManager);
                _deviceManager->Wrap(args.This());
                args.GetReturnValue().Set(args.This());
            }
            
            void NodeIrisDeviceManager::CallApiAudioDevice(const Nan_FunctionCallbackInfo<v8_Value> &args)
            {
                auto _deviceManager = ObjectWrap::Unwrap<NodeIrisDeviceManager>(args.Holder());
                auto _isolate = args.GetIsolate();

                auto _apiType = nan_api_get_value_int32_(args[0]);
                auto _parameter = nan_api_get_value_utf8string_(args[1]);

                char _result[512];
                memset(_result, '\0', 512);
                auto _ret = _deviceManager->_deviceManager->CallApi((ApiTypeAudioDeviceManager)_apiType, _parameter.c_str(), _result);

                auto _retObj = v8_Object::New(_isolate);
                v8_SET_OBJECT_PROP_UINT32(_isolate, _retObj, "retCode", _ret)
                v8_SET_OBJECT_PROP_STRING(_isolate, _retObj, "result", _result)
                args.GetReturnValue().Set(_retObj);
            }
            
            void NodeIrisDeviceManager::CallApiVideoDevice(const Nan_FunctionCallbackInfo<v8_Value> &args)
            {
                auto _deviceManager = ObjectWrap::Unwrap<NodeIrisDeviceManager>(args.Holder());
                auto _isolate = args.GetIsolate();

                auto _apiType = nan_api_get_value_int32_(args[0]);
                auto _parameter = nan_api_get_value_utf8string_(args[1]);

                char _result[512];
                memset(_result, '\0', 512);
                auto _ret = _deviceManager->_deviceManager->CallApi((ApiTypeVideoDeviceManager)_apiType, _parameter.c_str(), _result);

                auto _retObj = v8_Object::New(_isolate);
                v8_SET_OBJECT_PROP_UINT32(_isolate, _retObj, "retCode", _ret)
                v8_SET_OBJECT_PROP_STRING(_isolate, _retObj, "result", _result)
                args.GetReturnValue().Set(_retObj);
            }

            void NodeIrisDeviceManager::Release(void *selfPtr)
            {
                auto _selfPtr = static_cast<NodeIrisDeviceManager *>(selfPtr);
                delete _selfPtr;
                _selfPtr = nullptr;
            }
        }
    }
}