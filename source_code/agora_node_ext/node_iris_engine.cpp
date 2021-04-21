#include "node_iris_engine.h"

namespace agora
{
    namespace rtc
    {
        namespace electron
        {
            using namespace iris;

            Nan_Persistent<v8_Function> NodeIrisEngine::_constructor;

            NodeIrisEngine::NodeIrisEngine(v8_Isolate *isolate) : _isolate(isolate)
            {
                _iris_event_handler.reset(new NodeIrisEventHandler());
                _iris_engine.reset(new IrisEngine());
                _iris_raw_data.reset(_iris_engine->iris_raw_data());
                _video_source_proxy.reset(new VideoSourceProxy());
                _iris_engine->SetEventHandler(_iris_event_handler.get());
            }

            NodeIrisEngine::~NodeIrisEngine()
            {
                _iris_event_handler.reset();
                _iris_raw_data.reset();
                _iris_engine.reset();
                _video_source_proxy.reset();
                stopLogService();
            }

            void NodeIrisEngine::Init(v8_Local<v8_Object> &_module)
            {
                auto _isolate = _module->GetIsolate();
                auto _context = _isolate->GetCurrentContext();
                auto _template = v8_FunctionTemplate::New(_isolate, CreateInstance);
                _template->SetClassName(Nan::New<v8_String>("NodeIrisEngine").ToLocalChecked());
                _template->InstanceTemplate()->SetInternalFieldCount(5);

                Nan::SetPrototypeMethod(_template, "CallApi", CallApi);
                Nan::SetPrototypeMethod(_template, "CallApiWithBuffer", CallApiWithBuffer);
                
                Nan::SetPrototypeMethod(_template, "OnEvent", OnEvent);
                Nan::SetPrototypeMethod(_template, "GetChannel", GetChannel);
                Nan::SetPrototypeMethod(_template, "GetDeviceManager", GetDeviceManager);
                Nan::SetPrototypeMethod(_template, "GetScreenWindowsInfo", GetScreenWindowsInfo);
                Nan::SetPrototypeMethod(_template, "GetScreenDisplaysInfo", GetScreenDisplaysInfo);
                
                Nan::SetPrototypeMethod(_template, "VideoSourceInitialize", VideoSourceInitialize);
                Nan::SetPrototypeMethod(_template, "VideoSourceCallApi", VideoSourceCallApi);
                Nan::SetPrototypeMethod(_template, "VideoSourceCallApiWithBuffer", VideoSourceCallApiWithBuffer);
                Nan::SetPrototypeMethod(_template, "VideoSourceRelease", VideoSourceRelease);

                Nan::SetPrototypeMethod(_template, "SetAddonLogFile", SetAddonLogFile);
                _constructor.Reset(_template->GetFunction(_context).ToLocalChecked());
                _module->Set(_context, Nan::New<v8_String>("NodeIrisEngine").ToLocalChecked(), _template->GetFunction(_context).ToLocalChecked());

                LOG_F(INFO, "Init");
            }

            void NodeIrisEngine::CreateInstance(const v8_FunctionCallbackInfo<v8_Value> &args)
            {
                auto _isolate = args.GetIsolate();
                if (args.IsConstructCall())
                {
                    LOG_F(INFO, "CreateInstance");
                    auto _iris_engine = new NodeIrisEngine(_isolate);
                    _iris_engine->Wrap(args.This());
                    args.GetReturnValue().Set(args.This());
                }
                else
                {
                    auto cons = v8_Local<v8_Function>::New(_isolate, _constructor);
                    auto _context = _isolate->GetCurrentContext();
                    auto _instance = cons->NewInstance(_context).ToLocalChecked();
                    args.GetReturnValue().Set(_instance);
                }
            }

            void NodeIrisEngine::CallApi(const Nan_FunctionCallbackInfo<v8_Value> &args)
            {
                auto _engine = ObjectWrap::Unwrap<NodeIrisEngine>(args.Holder());
                auto _isolate = args.GetIsolate();
                auto _apiType = nan_api_get_value_int32_(args[0]);
                auto _parameter = nan_api_get_value_utf8string_(args[1]);
                char _result[512];
                memset(_result, '\0', 512);
                LOG_F(INFO, "CallApi parameter: %s", _parameter.c_str());
                auto _ret = _engine->_iris_engine.get()->CallApi((ApiTypeEngine)_apiType, _parameter.c_str(), _result);

                auto _retObj = v8_Object::New(_isolate);
                v8_SET_OBJECT_PROP_UINT32(_isolate, _retObj, "retCode", _ret)
                v8_SET_OBJECT_PROP_STRING(_isolate, _retObj, "result", _result)
                args.GetReturnValue().Set(_retObj);
            }

            void NodeIrisEngine::CallApiWithBuffer(const Nan_FunctionCallbackInfo<v8_Value> &args)
            {
                auto _engine = ObjectWrap::Unwrap<NodeIrisEngine>(args.Holder());
                auto _isolate = args.GetIsolate();
                auto _apiType = nan_api_get_value_int32_(args[0]);
                auto _parameter = nan_api_get_value_utf8string_(args[1]);
                char _result[512];
                int _ret = -1;
                memset(_result, '\0', 512);
                auto _retObj = v8_Object::New(_isolate);

                switch (ApiTypeEngine(_apiType))
                {
                case kEngineRegisterPacketObserver:
                {
                    break;
                }
                case kEngineSendStreamMessage:
                {
                    auto _buffer = nan_api_get_value_utf8string_(args[2]);
                    _ret = _engine->_iris_engine.get()->CallApi((ApiTypeEngine)_apiType, _parameter.c_str(), const_cast<char *>(_buffer.c_str()), _result);
                    break;
                }
                case kEngineSendMetadata:
                {
                    break;
                }
                case kMediaPushAudioFrame:
                {
                    break;
                }
                case kMediaPullAudioFrame:
                {
                    break;
                }
                case kMediaPushVideoFrame:
                {
                    break;
                }
                default:
                {
                    break;
                }
                }

                v8_SET_OBJECT_PROP_UINT32(_isolate, _retObj, "retCode", _ret)
                v8_SET_OBJECT_PROP_STRING(_isolate, _retObj, "result", _result)
                args.GetReturnValue().Set(_retObj);
            }

            void NodeIrisEngine::OnEvent(const Nan_FunctionCallbackInfo<v8_Value> &args)
            {
                auto _engine = ObjectWrap::Unwrap<NodeIrisEngine>(args.Holder());
                auto _parameter = nan_api_get_value_utf8string_(args[0]);
                auto _callback = args[1].As<v8_Function>();
                Nan_Persistent<v8_Function> _persist;
                _persist.Reset(_callback);

                auto _obj = args.This();
                Nan_Persistent<v8_Object> _persistObj;
                _persistObj.Reset(_obj);
                _engine->_iris_event_handler->addEvent(_parameter, _persistObj, _persist);
            }

            void NodeIrisEngine::GetChannel(const Nan_FunctionCallbackInfo<v8_Value> &args)
            {
                LOG_F(INFO, " NodeIrisEngine::GetChannel");
                auto _engine = ObjectWrap::Unwrap<NodeIrisEngine>(args.Holder());
                auto _isolate = args.GetIsolate();

                auto _iris_channel = _engine->_iris_engine->iris_channel();
                auto _js_channel = NodeIrisChannel::Init(_isolate, _iris_channel);
                args.GetReturnValue().Set(_js_channel);
            }

            void NodeIrisEngine::GetDeviceManager(const Nan_FunctionCallbackInfo<v8_Value> &args)
            {
                auto _engine = ObjectWrap::Unwrap<NodeIrisEngine>(args.Holder());
                auto _isolate = args.GetIsolate();

                auto _device_manager = _engine->_iris_engine->iris_device_manager();
                auto _js_device_manager = NodeIrisDeviceManager::Init(_isolate, _device_manager);
                args.GetReturnValue().Set(_js_device_manager);
            }

            void NodeIrisEngine::GetScreenWindowsInfo(const Nan_FunctionCallbackInfo<v8_Value> &args)
            {
                auto _isolate = args.GetIsolate();
                auto _context = _isolate->GetCurrentContext();

                auto _screenWindowInfoArray = v8_Array::New(_isolate);
                auto _allWindows = getAllWindowInfo();
                for (auto i = 0; i < _allWindows.size(); i++)
                {
                    auto _windowInfo = _allWindows[i];
                    auto obj = v8_Object::New(_isolate);
#ifdef _WIN32
                    UINT32 windowId = (UINT32)_windowInfo.windowId;
#elif defined(__APPLE__)
                    unsigned int windowId = _windowInfo.windowId;
#endif
                    v8_SET_OBJECT_PROP_UINT32(_isolate, obj, "windowId", windowId)
                    v8_SET_OBJECT_PROP_STRING(_isolate, obj, "name", _windowInfo.name.c_str())
                    v8_SET_OBJECT_PROP_STRING(_isolate, obj, "ownerName", _windowInfo.ownerName.c_str())
                    v8_SET_OBJECT_PROP_BOOL(_isolate, obj, "isOnScreen", _windowInfo.isOnScreen)
                    v8_SET_OBJECT_PROP_UINT32(_isolate, obj, "width", _windowInfo.width)
                    v8_SET_OBJECT_PROP_UINT32(_isolate, obj, "height", _windowInfo.height)
                    v8_SET_OBJECT_PROP_UINT32(_isolate, obj, "originWidth", _windowInfo.originWidth)
                    v8_SET_OBJECT_PROP_UINT32(_isolate, obj, "originHeight", _windowInfo.originHeight)

                    if (_windowInfo.imageData)
                    {
                        v8_SET_OBJECT_PROP_UINT8_ARRAY(_isolate, obj, "image", _windowInfo.imageData, _windowInfo.imageDataLength)
                        free(_windowInfo.imageData);
                    }
                    auto resultObj = _screenWindowInfoArray->Set(_context, i, obj);
                    v8_MAYBE_CHECK_RESULT(resultObj);
                }
                args.GetReturnValue().Set(_screenWindowInfoArray);
            }

            void NodeIrisEngine::GetScreenDisplaysInfo(const Nan_FunctionCallbackInfo<v8_Value> &args)
            {
                auto _isolate = args.GetIsolate();
                auto _context = _isolate->GetCurrentContext();
                auto _allDisplayInfoArray = v8_Array::New(_isolate);
                auto _allDisplays = getAllDisplayInfo();

                for (auto i = 0; i < _allDisplays.size(); i++)
                {
                    auto _displayInfo = _allDisplays[i];
                    auto _obj = v8_Object::New(_isolate);
                    auto _displayId = _displayInfo.displayId;
                    auto _displayIdObj = v8_Object::New(_isolate);
#ifdef _WIN32
                    v8_SET_OBJECT_PROP_UINT32(_isolate, _displayIdObj, "x", _displayId.x)
                    v8_SET_OBJECT_PROP_UINT32(_isolate, _displayIdObj, "y", _displayId.y)
                    v8_SET_OBJECT_PROP_UINT32(_isolate, _displayIdObj, "width", _displayId.width)
                    v8_SET_OBJECT_PROP_UINT32(_isolate, _displayIdObj, "height", _displayId.height)
#elif defined(__APPLE__)
                    v8_SET_OBJECT_PROP_UINT32(_isolate, _displayIdObj, "id", _displayId.idVal)
#endif
                    auto propName = v8_String::NewFromUtf8(_isolate, "displayId", v8::NewStringType::kInternalized).ToLocalChecked();
                    auto resultObj = _obj->Set(_context, propName, _displayIdObj);
                    v8_MAYBE_CHECK_RESULT(resultObj);

                    v8_SET_OBJECT_PROP_UINT32(_isolate, _obj, "width", _displayInfo.width)
                    v8_SET_OBJECT_PROP_UINT32(_isolate, _obj, "height", _displayInfo.height)
                    v8_SET_OBJECT_PROP_BOOL(_isolate, _obj, "isMain", _displayInfo.isMain)
                    v8_SET_OBJECT_PROP_BOOL(_isolate, _obj, "isActive", _displayInfo.isActive)
                    v8_SET_OBJECT_PROP_BOOL(_isolate, _obj, "isBuiltin", _displayInfo.isBuiltin) if (_displayInfo.imageData)
                    {
                        v8_SET_OBJECT_PROP_UINT8_ARRAY(_isolate, _obj, "image", _displayInfo.imageData, _displayInfo.imageDataLength)
                        free(_displayInfo.imageData);
                    }
                    auto result = _allDisplayInfoArray->Set(_context, i, _obj);
                    v8_MAYBE_CHECK_RESULT(result);
                    args.GetReturnValue().Set(_allDisplayInfoArray);
                }
            }

            void NodeIrisEngine::VideoSourceInitialize(const Nan_FunctionCallbackInfo<v8_Value> &args)
            {
                auto _engine = ObjectWrap::Unwrap<NodeIrisEngine>(args.Holder());
                auto _isolate = args.GetIsolate();
                auto _parameter = nan_api_get_value_utf8string_(args[0]);
                auto _result = -1;
                if (_engine->_video_source_proxy->Initialize(_engine->_iris_event_handler.get(), _parameter))
                    _result = 0;

                auto _retObj = v8_Object::New(_isolate);
                v8_SET_OBJECT_PROP_INT32(_isolate, _retObj, "retCode", _result)
                v8_SET_OBJECT_PROP_STRING(_isolate, _retObj, "result", "")
                args.GetReturnValue().Set(_retObj);
            }

            void NodeIrisEngine::VideoSourceRelease(const Nan_FunctionCallbackInfo<v8_Value> &args)
            {
                auto _engine = ObjectWrap::Unwrap<NodeIrisEngine>(args.Holder());
                auto _isolate = args.GetIsolate();
                auto _result = _engine->_video_source_proxy->Release();
                auto _retObj = v8_Object::New(_isolate);
                v8_SET_OBJECT_PROP_INT32(_isolate, _retObj, "retCode", _result)
                v8_SET_OBJECT_PROP_STRING(_isolate, _retObj, "result", "")
                args.GetReturnValue().Set(_retObj);
            }

            void NodeIrisEngine::VideoSourceCallApi(const Nan_FunctionCallbackInfo<v8_Value> &args)
            {
                auto _engine = ObjectWrap::Unwrap<NodeIrisEngine>(args.Holder());
                auto _isolate = args.GetIsolate();
                auto _apiType = nan_api_get_value_int32_(args[0]);
                auto _parameter = nan_api_get_value_utf8string_(args[1]);
                char _result[512];
                memset(_result, '\0', 512);
                auto _ret = _engine->_video_source_proxy->CallApi((ApiTypeEngine)_apiType, _parameter.c_str(), _result);
                auto _retObj = v8_Object::New(_isolate);
                v8_SET_OBJECT_PROP_UINT32(_isolate, _retObj, "retCode", _ret)
                v8_SET_OBJECT_PROP_STRING(_isolate, _retObj, "result", _result)
                args.GetReturnValue().Set(_retObj);
            }

            void NodeIrisEngine::VideoSourceCallApiWithBuffer(const Nan_FunctionCallbackInfo<v8_Value> &args)
            {
                auto _engine = ObjectWrap::Unwrap<NodeIrisEngine>(args.Holder());
                auto _isolate = args.GetIsolate();
                auto _apiType = nan_api_get_value_int32_(args[0]);
                auto _parameter = nan_api_get_value_utf8string_(args[1]);
                auto _length = nan_api_get_value_int32_(args[2]);
                auto _buffer = nan_api_get_value_utf8string_(args[3]);
                char _result[512];
                memset(_result, '\0', 512);
                auto _ret = _engine->_video_source_proxy->CallApi((ApiTypeEngine)_apiType, _parameter.c_str(), _buffer.c_str(), _length, _result);
                auto _retObj = v8_Object::New(_isolate);
                v8_SET_OBJECT_PROP_UINT32(_isolate, _retObj, "retCode", _ret)
                v8_SET_OBJECT_PROP_STRING(_isolate, _retObj, "result", _result)
                args.GetReturnValue().Set(_retObj);
            }

            void NodeIrisEngine::SetAddonLogFile(const Nan_FunctionCallbackInfo<v8_Value> &args)
            {
                auto _isolate = args.GetIsolate();
                auto _filePath = nan_api_get_value_utf8string_(args[0]);
                bool _ret = startLogService(_filePath.c_str());
                auto _result = ERROR_CODE::ERROR_OK;
                auto _retObj = v8_Object::New(_isolate);
                v8_SET_OBJECT_PROP_BOOL(_isolate, _retObj, "retCode", _ret)
                v8_SET_OBJECT_PROP_INT32(_isolate, _retObj, "result", _result)
                args.GetReturnValue().Set(_retObj);
            }

            void NodeIrisEngine::OnApiError(const char *errorMessage)
            {
                _iris_event_handler->OnEvent("apiError", errorMessage);
            }

            void NodeIrisEngine::InitializePluginManager(const Nan_FunctionCallbackInfo<v8_Value> &args)
            {
                
            }

            void NodeIrisEngine::ReleasePluginManager(const Nan_FunctionCallbackInfo<v8_Value> &args)
            {

            }

            void NodeIrisEngine::RegisterPlugin(const Nan_FunctionCallbackInfo<v8_Value> &args)
            {

            }

            void NodeIrisEngine::UnregisterPlugin(const Nan_FunctionCallbackInfo<v8_Value> &args)
            {

            }

            void NodeIrisEngine::EnablePlugin(const Nan_FunctionCallbackInfo<v8_Value> &args)
            {

            }

            void NodeIrisEngine::GetPlugins(const Nan_FunctionCallbackInfo<v8_Value> &args)
            {

            }

            void NodeIrisEngine::SetPluginParameter(const Nan_FunctionCallbackInfo<v8_Value> &args)
            {

            }

            void NodeIrisEngine::GetPluginParameter(const Nan_FunctionCallbackInfo<v8_Value> &args)
            {

            }
        }
    }
}