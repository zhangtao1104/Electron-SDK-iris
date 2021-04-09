#include "node_iris_channel.h"

namespace agora
{
    namespace rtc
    {
        namespace electron
        {
            using namespace iris;

            NodeIrisChannel::NodeIrisChannel(v8_Isolate *isolate, IrisChannel *channel)
            {
                node::AddEnvironmentCleanupHook(isolate, Release, this);
                _isolate = isolate;
                _iris_channel.reset(channel);
                _iris_channel_event_handler.reset(new AgoraIrisEventHandler());
                _iris_channel->SetEventHandler(_iris_channel_event_handler.get());
            }

            NodeIrisChannel::~NodeIrisChannel()
            {
                _iris_channel.reset(nullptr);
                _iris_channel_event_handler.reset(nullptr);
            }

            v8_Local<v8_Object> NodeIrisChannel::Init(v8_Isolate *isolate, IrisChannel *irisChannel)
            {
                auto _context = isolate->GetCurrentContext();
                auto _template = v8_FunctionTemplate::New(isolate, CreateInstance);
                _template->SetClassName(Nan::New<v8_String>("NodeIrisChannel").ToLocalChecked());
                _template->InstanceTemplate()->SetInternalFieldCount(5);

                Nan::SetPrototypeMethod(_template, "CallApi", CallApi);
                Nan::SetPrototypeMethod(_template, "CallApiWithBuffer", CallApiWithBuffer);
                Nan::SetPrototypeMethod(_template, "OnEvent", OnEvent);
                _constructor.Reset(_template->GetFunction(_context).ToLocalChecked());

                auto cons = _template->GetFunction(_context).ToLocalChecked();
                auto argChannel = v8_Local<v8_External>::New(isolate, v8_External::New(isolate, irisChannel));
                v8_Local<v8_Value> argv[1] = {argChannel};
                auto jschannel = cons->NewInstance(_context, 1, argv).ToLocalChecked();
                return jschannel;
            }

            void NodeIrisChannel::CreateInstance(const v8_FunctionCallbackInfo<v8_Value> &args)
            {
                auto *_isolate = args.GetIsolate();
                auto _argChannel = v8_Local<v8_External>::Cast(args[0]);
                auto *_irisChannel = static_cast<IrisChannel *>(_argChannel->Value());
                auto *_channel = new NodeIrisChannel(_isolate, _irisChannel);
                _channel->Wrap(args.This());
                args.GetReturnValue().Set(args.This());
            }

            void NodeIrisChannel::CallApi(const Nan_FunctionCallbackInfo<v8_Value> &args)
            {
                auto _channel = ObjectWrap::Unwrap<NodeIrisChannel>(args.Holder());
                auto _isolate = args.GetIsolate();
                auto _apiType = nan_api_get_value_int32_(args[0]);
                auto _parameter = nan_api_get_value_utf8string_(args[1]);
                char _result[512];
                memset(_result, '\0', 512);
                auto _ret = _channel->_iris_channel.get()->CallApi((ApiTypeChannel)_apiType, _parameter.c_str(), _result);

                auto _retObj = v8_Object::New(_isolate);
                v8_SET_OBJECT_PROP_UINT32(_isolate, _retObj, "retCode", _ret)
                v8_SET_OBJECT_PROP_STRING(_isolate, _retObj, "result", _result)
                args.GetReturnValue().Set(_retObj);
            }

            void NodeIrisChannel::CallApiWithBuffer(const Nan_FunctionCallbackInfo<v8_Value> &args)
            {
                auto _channel = ObjectWrap::Unwrap<NodeIrisChannel>(args.Holder());
                auto _isolate = args.GetIsolate();
                auto _apiType = nan_api_get_value_int32_(args[0]);
                auto _parameter = nan_api_get_value_utf8string_(args[1]);
                char _result[512];
                int _ret = -1;
                memset(_result, '\0', 512);
                auto _retObj = v8_Object::New(_isolate);

                switch (ApiTypeChannel(_apiType))
                {
                case ApiTypeChannel::kChannelRegisterPacketObserver:
                {
                    break;
                }
                case ApiTypeChannel::kChannelSendStreamMessage:
                {
                    auto _buffer = nan_api_get_value_utf8string_(args[2]);
                    _ret = _channel->_iris_channel.get()->CallApi((ApiTypeChannel)_apiType, _parameter.c_str(), const_cast<char *>(_buffer.c_str()), _result);
                    break;
                }
                case ApiTypeChannel::kChannelSendMetadata:
                {
                    auto _buffer = nan_api_get_value_utf8string_(args[2]);
                    _ret = _channel->_iris_channel.get()->CallApi((ApiTypeChannel)_apiType, _parameter.c_str(), const_cast<char *>(_buffer.c_str()), _result);
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

            void NodeIrisChannel::OnEvent(const Nan_FunctionCallbackInfo<v8_Value> &args)
            {
                auto _channel = ObjectWrap::Unwrap<NodeIrisChannel>(args.Holder());
                auto _parameter = nan_api_get_value_utf8string_(args[0]);
                auto _callback = args[1].As<v8_Function>();
                Nan_Persistent<v8_Function> _persist;
                _persist.Reset(_callback);

                auto _obj = args.This();
                Nan_Persistent<v8_Object> _persistObj;
                _persistObj.Reset(_obj);

                _channel->_iris_channel_event_handler->addEvent(_parameter, _persistObj, _persist);
            }

            void NodeIrisChannel::Release(void *data)
            {
                NodeIrisChannel *_irisChannel = static_cast<NodeIrisChannel *>(data);
                delete _irisChannel;
                _irisChannel = nullptr;
            }
        }
    }
}