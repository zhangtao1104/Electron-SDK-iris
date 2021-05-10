/*
 * @Author: zhangtao@agora.io 
 * @Date: 2021-04-22 20:53:37 
 * @Last Modified by: zhangtao@agora.io
 * @Last Modified time: 2021-05-10 15:12:55
 */
#include "node_iris_rtc_engine.h"

namespace agora
{
    namespace rtc
    {
        namespace electron
        {
            using namespace iris::rtc;

            Nan_Persistent<v8_Function> NodeIrisRtcEngine::_constructor;

            NodeIrisRtcEngine::NodeIrisRtcEngine(v8_Isolate *isolate) : _isolate(isolate)
            {
                _iris_event_handler.reset(new NodeIrisEventHandler());
                _iris_engine.reset(new IrisRtcEngine());
                _iris_raw_data.reset(_iris_engine->raw_data());
                _iris_raw_data_plugin_manager.reset(_iris_raw_data.get()->plugin_manager());
                _video_processer.reset(new VideoProcesser(_iris_engine.get()));
                _video_source_proxy.reset(new VideoSourceProxy());
                _iris_engine->SetEventHandler(_iris_event_handler.get());
            }

            NodeIrisRtcEngine::~NodeIrisRtcEngine()
            {
                _video_processer.reset();
                _iris_event_handler.reset();
                _iris_raw_data_plugin_manager.reset();
                _iris_raw_data.reset();
                _iris_engine.reset();
                _video_source_proxy.reset();
                stopLogService();
            }

            void NodeIrisRtcEngine::Init(v8_Local<v8_Object> &_module)
            {
                auto _isolate = _module->GetIsolate();
                auto _context = _isolate->GetCurrentContext();
                auto _template = v8_FunctionTemplate::New(_isolate, CreateInstance);
                _template->SetClassName(Nan::New<v8_String>("NodeIrisRtcEngine").ToLocalChecked());
                _template->InstanceTemplate()->SetInternalFieldCount(5);

                Nan::SetPrototypeMethod(_template, "CallApi", CallApi);
                Nan::SetPrototypeMethod(_template, "CallApiWithBuffer", CallApiWithBuffer);
                
                Nan::SetPrototypeMethod(_template, "OnEvent", OnEvent);
                Nan::SetPrototypeMethod(_template, "CreateChannel", CreateChannel);
                Nan::SetPrototypeMethod(_template, "GetDeviceManager", GetDeviceManager);
                Nan::SetPrototypeMethod(_template, "GetScreenWindowsInfo", GetScreenWindowsInfo);
                Nan::SetPrototypeMethod(_template, "GetScreenDisplaysInfo", GetScreenDisplaysInfo);
                
                Nan::SetPrototypeMethod(_template, "VideoSourceInitialize", VideoSourceInitialize);
                Nan::SetPrototypeMethod(_template, "VideoSourceCallApi", VideoSourceCallApi);
                Nan::SetPrototypeMethod(_template, "VideoSourceCallApiWithBuffer", VideoSourceCallApiWithBuffer);
                Nan::SetPrototypeMethod(_template, "VideoSourceRelease", VideoSourceRelease);

                Nan::SetPrototypeMethod(_template, "SetAddonLogFile", SetAddonLogFile);
                Nan::SetPrototypeMethod(_template, "PluginCallApi", PluginCallApi);
                Nan::SetPrototypeMethod(_template, "VideoSourcePluginCallApi", VideoSourcePluginCallApi);

                Nan::SetPrototypeMethod(_template, "EnableVideoFrameCache", EnableVideoFrameCache);
                Nan::SetPrototypeMethod(_template, "DisableVideoFrameCache", DisableVideoFrameCache);
                Nan::SetPrototypeMethod(_template, "GetVideoStreamData", GetVideoStreamData);

                _constructor.Reset(_template->GetFunction(_context).ToLocalChecked());
                _module->Set(_context, Nan::New<v8_String>("NodeIrisRtcEngine").ToLocalChecked(), _template->GetFunction(_context).ToLocalChecked());

                LOG_F(INFO, "Init");
            }

            void NodeIrisRtcEngine::CreateInstance(const v8_FunctionCallbackInfo<v8_Value> &args)
            {
                auto _isolate = args.GetIsolate();
                if (args.IsConstructCall())
                {
                    LOG_F(INFO, "CreateInstance");
                    auto _iris_engine = new NodeIrisRtcEngine(_isolate);
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

            void NodeIrisRtcEngine::CallApi(const Nan_FunctionCallbackInfo<v8_Value> &args)
            {
                auto _engine = ObjectWrap::Unwrap<NodeIrisRtcEngine>(args.Holder());
                auto _isolate = args.GetIsolate();
                auto _apiType = nan_api_get_value_int32_(args[0]);
                auto _parameter = nan_api_get_value_utf8string_(args[1]);
                char _result[512];
                memset(_result, '\0', 512);
                LOG_F(INFO, "CallApi parameter: %s", _parameter.c_str());
                int _ret = ERROR_PARAMETER_1;
                try {
                    _ret = _engine->_iris_engine.get()->CallApi((ApiTypeEngine)_apiType, _parameter.c_str(), _result);
                } catch(std::exception& e) {
                    _engine->OnApiError(e.what());
                }
                auto _retObj = v8_Object::New(_isolate);
                v8_SET_OBJECT_PROP_UINT32(_isolate, _retObj, "retCode", _ret)
                v8_SET_OBJECT_PROP_STRING(_isolate, _retObj, "result", _result)
                args.GetReturnValue().Set(_retObj);
            }

            void NodeIrisRtcEngine::CallApiWithBuffer(const Nan_FunctionCallbackInfo<v8_Value> &args)
            {
                auto _engine = ObjectWrap::Unwrap<NodeIrisRtcEngine>(args.Holder());
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

            void NodeIrisRtcEngine::OnEvent(const Nan_FunctionCallbackInfo<v8_Value> &args)
            {
                auto _engine = ObjectWrap::Unwrap<NodeIrisRtcEngine>(args.Holder());
                auto _parameter = nan_api_get_value_utf8string_(args[0]);
                auto _callback = args[1].As<v8_Function>();
                Nan_Persistent<v8_Function> _persist;
                _persist.Reset(_callback);

                auto _obj = args.This();
                Nan_Persistent<v8_Object> _persistObj;
                _persistObj.Reset(_obj);
                _engine->_iris_event_handler->addEvent(_parameter, _persistObj, _persist);
            }

            void NodeIrisRtcEngine::CreateChannel(const Nan_FunctionCallbackInfo<v8_Value> &args)
            {
                LOG_F(INFO, " NodeIrisRtcEngine::CreateChannel");
                auto _engine = ObjectWrap::Unwrap<NodeIrisRtcEngine>(args.Holder());
                auto _isolate = args.GetIsolate();
                auto _channelId = nan_api_get_value_utf8string_(args[0]);
                auto _iris_channel = _engine->_iris_engine->channel();
                auto _js_channel = NodeIrisRtcChannel::Init(_isolate, _iris_channel, _channelId.c_str());
                args.GetReturnValue().Set(_js_channel);
            }

            void NodeIrisRtcEngine::GetDeviceManager(const Nan_FunctionCallbackInfo<v8_Value> &args)
            {
                auto _engine = ObjectWrap::Unwrap<NodeIrisRtcEngine>(args.Holder());
                auto _isolate = args.GetIsolate();

                auto _device_manager = _engine->_iris_engine->device_manager();
                auto _js_device_manager = NodeIrisRtcDeviceManager::Init(_isolate, _device_manager);
                args.GetReturnValue().Set(_js_device_manager);
            }

            void NodeIrisRtcEngine::GetScreenWindowsInfo(const Nan_FunctionCallbackInfo<v8_Value> &args)
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

            void NodeIrisRtcEngine::GetScreenDisplaysInfo(const Nan_FunctionCallbackInfo<v8_Value> &args)
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

            void NodeIrisRtcEngine::VideoSourceInitialize(const Nan_FunctionCallbackInfo<v8_Value> &args)
            {
                auto _engine = ObjectWrap::Unwrap<NodeIrisRtcEngine>(args.Holder());
                auto _isolate = args.GetIsolate();
                auto _parameter = nan_api_get_value_utf8string_(args[0]);
                auto _result = -1;
                LOG_F(INFO, "VideoSourceInitialize parameter: %s", _parameter.c_str());
                
            
                if (_engine->_video_source_proxy->Initialize(_engine->_iris_event_handler.get(), _parameter))
                    _result = 0;

                auto _retObj = v8_Object::New(_isolate);
                v8_SET_OBJECT_PROP_INT32(_isolate, _retObj, "retCode", _result)
                v8_SET_OBJECT_PROP_STRING(_isolate, _retObj, "result", "")
                args.GetReturnValue().Set(_retObj);
            }

            void NodeIrisRtcEngine::VideoSourceRelease(const Nan_FunctionCallbackInfo<v8_Value> &args)
            {
                auto _engine = ObjectWrap::Unwrap<NodeIrisRtcEngine>(args.Holder());
                auto _isolate = args.GetIsolate();
                auto _result = _engine->_video_source_proxy->Release();
                auto _retObj = v8_Object::New(_isolate);
                v8_SET_OBJECT_PROP_INT32(_isolate, _retObj, "retCode", _result)
                v8_SET_OBJECT_PROP_STRING(_isolate, _retObj, "result", "")
                args.GetReturnValue().Set(_retObj);
            }

            void NodeIrisRtcEngine::VideoSourceCallApi(const Nan_FunctionCallbackInfo<v8_Value> &args)
            {
                auto _engine = ObjectWrap::Unwrap<NodeIrisRtcEngine>(args.Holder());
                auto _isolate = args.GetIsolate();
                auto _apiType = nan_api_get_value_int32_(args[0]);
                auto _parameter = nan_api_get_value_utf8string_(args[1]);
                char _result[512];
                memset(_result, '\0', 512);
                int _ret = ERROR_PARAMETER_1;
                try {
                    _ret = _engine->_video_source_proxy->CallApi((ApiTypeEngine)_apiType, _parameter.c_str(), _result);
                 } catch (std::exception& e) {
                    _engine->OnApiError(e.what());
                }
                auto _retObj = v8_Object::New(_isolate);
                v8_SET_OBJECT_PROP_UINT32(_isolate, _retObj, "retCode", _ret)
                v8_SET_OBJECT_PROP_STRING(_isolate, _retObj, "result", _result)
                args.GetReturnValue().Set(_retObj);
            }

            void NodeIrisRtcEngine::VideoSourceCallApiWithBuffer(const Nan_FunctionCallbackInfo<v8_Value> &args)
            {
                auto _engine = ObjectWrap::Unwrap<NodeIrisRtcEngine>(args.Holder());
                auto _isolate = args.GetIsolate();
                auto _apiType = nan_api_get_value_int32_(args[0]);
                auto _parameter = nan_api_get_value_utf8string_(args[1]);
                auto _length = nan_api_get_value_int32_(args[2]);
                auto _buffer = nan_api_get_value_utf8string_(args[3]);
                char _result[512];
                memset(_result, '\0', 512);
                int _ret = ERROR_PARAMETER_1;
                try {
                    _ret = _engine->_video_source_proxy->CallApi((ApiTypeEngine)_apiType, _parameter.c_str(), _buffer.c_str(), _length, _result);
                } catch (std::exception& e) {
                    LOG_F(INFO, "VideoSourceCallApiWithBuffer catch exception: %s", e.what());
                    _engine->OnApiError(e.what());
                }
                auto _retObj = v8_Object::New(_isolate);
                v8_SET_OBJECT_PROP_UINT32(_isolate, _retObj, "retCode", _ret)
                v8_SET_OBJECT_PROP_STRING(_isolate, _retObj, "result", _result)
                args.GetReturnValue().Set(_retObj);
            }

            void NodeIrisRtcEngine::SetAddonLogFile(const Nan_FunctionCallbackInfo<v8_Value> &args)
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

            void NodeIrisRtcEngine::OnApiError(const char *errorMessage)
            {
                _iris_event_handler->OnEvent("apiError", errorMessage);
            }

            void NodeIrisRtcEngine::PluginCallApi(const Nan_FunctionCallbackInfo<v8_Value> &args)
            {
                auto _engine = ObjectWrap::Unwrap<NodeIrisRtcEngine>(args.Holder());
                auto _isolate = args.GetIsolate();
                auto _apiType = nan_api_get_value_int32_(args[0]);
                auto _parameter = nan_api_get_value_utf8string_(args[1]);
                char _result[512];
                memset(_result, '\0', 512);
                LOG_F(INFO, "CallApi parameter: %s", _parameter.c_str());
                int _ret = ERROR_PARAMETER_1;
                try {
                    _ret = _engine->_iris_raw_data_plugin_manager.get()->CallApi((ApiTypeRawDataPlugin)_apiType, _parameter.c_str(), _result);
                } catch(std::exception& e) {
                    LOG_F(INFO, "PluginCallApi catch exception %s", e.what());
                    _engine->OnApiError(e.what());
                }
                auto _retObj = v8_Object::New(_isolate);
                v8_SET_OBJECT_PROP_UINT32(_isolate, _retObj, "retCode", _ret)
                v8_SET_OBJECT_PROP_STRING(_isolate, _retObj, "result", _result)
                args.GetReturnValue().Set(_retObj);
            }

            void NodeIrisRtcEngine::VideoSourcePluginCallApi(const Nan_FunctionCallbackInfo<v8_Value> &args)
            {
                auto _engine = ObjectWrap::Unwrap<NodeIrisRtcEngine>(args.Holder());
                auto _isolate = args.GetIsolate();
                auto _apiType = nan_api_get_value_int32_(args[0]);
                auto _parameter = nan_api_get_value_utf8string_(args[1]);
                char _result[512];
                memset(_result, '\0', 512);
                int _ret = ERROR_PARAMETER_1;
                try {
                    _ret = _engine->_video_source_proxy->PluginCallApi((ApiTypeRawDataPlugin)_apiType, _parameter.c_str(), _result);
                } catch (std::exception& e) {
                    _engine->OnApiError(e.what());
                }
                auto _retObj = v8_Object::New(_isolate);
                v8_SET_OBJECT_PROP_UINT32(_isolate, _retObj, "retCode", _ret)
                v8_SET_OBJECT_PROP_STRING(_isolate, _retObj, "result", _result)
                args.GetReturnValue().Set(_retObj);
            }

            void NodeIrisRtcEngine::EnableVideoFrameCache(const Nan_FunctionCallbackInfo<v8_Value> &args)
            {
                auto _engine = ObjectWrap::Unwrap<NodeIrisRtcEngine>(args.Holder());
                auto _isolate = args.GetIsolate();
                v8_Local<v8_Object> _obj = nan_api_get_value_object_(_isolate, args[0]);
                auto _uid = nan_api_get_object_uint32_(_isolate, _obj, "uid");
                auto _channelId = nan_api_get_object_utf8string_(_isolate, _obj, "channelId");
                auto _width = nan_api_get_object_int32_(_isolate, _obj, "width");
                auto _height = nan_api_get_object_int32_(_isolate, _obj, "height");

                IrisRtcRendererCacheConfig config(IrisRtcVideoFrameObserver::VideoFrameType::kFrameTypeYUV420, nullptr, _width, _height);
                _engine->_video_processer->EnableVideoFrameCache(config, _uid, _channelId.c_str());
            }
                
            void NodeIrisRtcEngine::DisableVideoFrameCache(const Nan_FunctionCallbackInfo<v8_Value> &args)
            {
                auto _engine = ObjectWrap::Unwrap<NodeIrisRtcEngine>(args.Holder());
                auto _isolate = args.GetIsolate();
                auto _obj = nan_api_get_value_object_(_isolate, args[0]);
                auto _uid = nan_api_get_object_uint32_(_isolate, _obj, "uid");
                auto _channelId = nan_api_get_object_utf8string_(_isolate, _obj, "channelId");

                _engine->_video_processer->DisableVideoFrameCache(_uid, _channelId.c_str());
            }
            
            void NodeIrisRtcEngine::GetVideoStreamData(const Nan_FunctionCallbackInfo<v8_Value> &args)
            {
                auto _engine = ObjectWrap::Unwrap<NodeIrisRtcEngine>(args.Holder());
                auto _isolate = args.GetIsolate();

                auto _videoStreamObj = nan_api_get_value_object_(_isolate, args[0]);
                auto _uid = nan_api_get_object_uint32_(_isolate, _videoStreamObj, "uid");
                auto _channelId = nan_api_get_object_utf8string_(_isolate, _videoStreamObj, "channelId");
                auto _yBufferVal = nan_api_get_object_property_value_(_isolate, _videoStreamObj, "yBuffer");
                auto _uBufferVal = nan_api_get_object_property_value_(_isolate, _videoStreamObj, "uBuffer");
                auto _vBufferVal = nan_api_get_object_property_value_(_isolate, _videoStreamObj, "vBuffer");
                auto _frameHeaderVal = nan_api_get_object_property_value_(_isolate, _videoStreamObj, "frameHeader");

                auto _yBuffer = node::Buffer::Data(_yBufferVal);
                auto _uBuffer = node::Buffer::Data(_uBufferVal);
                auto _vBuffer = node::Buffer::Data(_vBufferVal);
                // auto _frameHeaderBuffer = node::Buffer::Data(_frameHeaderVal);

                // auto _nodeVideoFrameHeaderPtr = (IVideoFrameEventHandler::NodeVideoFrameHeader *)_frameHeaderBuffer;
                IrisRtcVideoFrameObserver::VideoFrame _videoFrame;
                _videoFrame.y_buffer = _yBuffer;
                _videoFrame.u_buffer = _uBuffer;
                _videoFrame.v_buffer = _vBuffer;
                
                bool isFresh = false;
                auto ret =  _engine->_video_processer->GetVideoFrame(_videoFrame, isFresh, _uid, _channelId.c_str());
                // _nodeVideoFrameHeaderPtr->isNewFrame = (uint8_t)(isFresh ? 1 : 0);
                // _nodeVideoFrameHeaderPtr->width = htons((uint16_t)_videoFrame.width);
                // _nodeVideoFrameHeaderPtr->height = htons((uint16_t)_videoFrame.height);
                // _nodeVideoFrameHeaderPtr->left = htons((uint16_t)(_videoFrame.y_stride - _videoFrame.width) / 2);
                // _nodeVideoFrameHeaderPtr->right = htons((uint16_t)(_videoFrame.y_stride - _videoFrame.width) / 2);
                // _nodeVideoFrameHeaderPtr->top = htons((uint16_t)0);
                // _nodeVideoFrameHeaderPtr->bottom = htons((uint16_t)0);
                // _nodeVideoFrameHeaderPtr->rotation = htons((uint16_t)_videoFrame.rotation);
                // _nodeVideoFrameHeaderPtr->timestamp = (uint32_t)_videoFrame.render_time_ms;
                
                auto _retObj = v8_Object::New(_isolate);
                v8_SET_OBJECT_PROP_BOOL(_isolate, _retObj, "ret", ret);
                v8_SET_OBJECT_PROP_BOOL(_isolate, _retObj, "isNewFrame", isFresh);
                v8_SET_OBJECT_PROP_UINT32(_isolate, _retObj, "width", _videoFrame.width);
                v8_SET_OBJECT_PROP_UINT32(_isolate, _retObj, "height", _videoFrame.height);
                v8_SET_OBJECT_PROP_UINT32(_isolate, _retObj, "left", (_videoFrame.y_stride - _videoFrame.width) / 2);
                v8_SET_OBJECT_PROP_UINT32(_isolate, _retObj, "right", (_videoFrame.y_stride - _videoFrame.width) / 2);
                v8_SET_OBJECT_PROP_UINT32(_isolate, _retObj, "top", 0);
                v8_SET_OBJECT_PROP_UINT32(_isolate, _retObj, "bottom", 0);
                v8_SET_OBJECT_PROP_UINT32(_isolate, _retObj, "rotation", _videoFrame.rotation);
                v8_SET_OBJECT_PROP_UINT32(_isolate, _retObj, "timestamp", _videoFrame.render_time_ms);
                // LOG_F(INFO, "GetVideoStreamData  isNewStream: %d, width: %d, height: %d, left: %d, right: %d, top: %d, bottom: %d, rotation: %d, timeStamp: %d",  _nodeVideoFrameHeaderPtr->isNewFrame,  _nodeVideoFrameHeaderPtr->width,  _nodeVideoFrameHeaderPtr->height,  _nodeVideoFrameHeaderPtr->left,  _nodeVideoFrameHeaderPtr->right,  _nodeVideoFrameHeaderPtr->top,  _nodeVideoFrameHeaderPtr->bottom,  _nodeVideoFrameHeaderPtr->rotation,  _nodeVideoFrameHeaderPtr->timestamp);
                args.GetReturnValue().Set(_retObj);
            }
        }
    }
}