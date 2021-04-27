/*
 * @Author: zhangtao@agora.io 
 * @Date: 2021-04-22 20:53:49 
 * @Last Modified by: zhangtao@agora.io
 * @Last Modified time: 2021-04-26 17:08:55
 */
#include "node_iris_event_handler.h"
#include "loguru.hpp"

namespace agora
{
    namespace rtc
    {
        namespace electron
        {
            NodeIrisEventHandler::~NodeIrisEventHandler()
            {
                _callbacks.clear();
            }

            void NodeIrisEventHandler::OnEvent(const char *event, const char *data)
            {
                std::string _eventName(event);
                std::string _eventData(data);
                node_async_call::async_call([this, _eventName, _eventData] {
                    LOG_F(INFO, "OnEvent  event: %s", _eventName.c_str());
                    auto it = _callbacks.find("call_back");
                    if (it != _callbacks.end())
                    {
                        auto _isolate = v8_Isolate::GetCurrent();
                        v8_HandleScope scope(_isolate);
                        auto context = _isolate->GetCurrentContext();
                        v8_Local<v8_Value> argv[2]{
                            nan_create_local_value_string_(_isolate, _eventName.c_str()),
                            nan_create_local_value_string_(_isolate, _eventData.c_str()),
                        };
                        EventCallback &cb = *it->second;
                        auto _result = cb.callback.Get(_isolate)->Call(context, cb.js_this.Get(_isolate), 2, argv);
                        v8_MAYBE_LOCAL_CHECK_RESULT(_result);
                    }
                });
            }

            void NodeIrisEventHandler::OnEvent(const char *event, const char *data, const void *buffer, unsigned length)
            {
                LOG_F(INFO, "NodeIrisEventHandler::OnEvent(const char *event, const char *data, const void *buffer, unsigned length) , %s", event);
                std::string _eventName(event);
                std::string _eventData(data);
                char stringData[length + 1];
                stringData[length] = '\0';
                memcpy(stringData, buffer, length);
                std::string _eventBuffer(stringData);
                node_async_call::async_call([this, _eventName, _eventData, _eventBuffer] {
                    auto it = _callbacks.find("call_back_with_buffer");
                    if (it != _callbacks.end())
                    {
                        auto _isolate = v8_Isolate::GetCurrent();
                        v8_HandleScope scope(_isolate);
                        auto context = _isolate->GetCurrentContext();
                        v8_Local<v8_Value> argv[3]{
                            nan_create_local_value_string_(_isolate, _eventName.c_str()),
                            nan_create_local_value_string_(_isolate, _eventData.c_str()),
                            nan_create_local_value_string_(_isolate, _eventBuffer.c_str()),
                        };
                        EventCallback &cb = *it->second;
                        auto _result = cb.callback.Get(_isolate)->Call(context, cb.js_this.Get(_isolate), 2, argv);
                        v8_MAYBE_LOCAL_CHECK_RESULT(_result);
                    }
                });
            }

            void NodeIrisEventHandler::OnVideoSourceEvent(const char* eventName, const char* eventData)
            {
                LOG_F(INFO, "NodeIrisEventHandler::OnVideoSourceEvent(const char* eventName, const char* eventData)");
                
                if (!eventName || !eventData) {
                    return;
                }
                
                std::string _eventName(eventName);
                std::string _eventData(eventData);
                LOG_F(INFO, "111NodeIrisEventHandler::OnVideoSourceEvent(const char* eventName, const char* eventData)");
              
                node_async_call::async_call([this, _eventName, _eventData] {
                    LOG_F(INFO, "22NodeIrisEventHandler::OnVideoSourceEvent(const char* eventName, const char* eventData)");
              
                    auto it = _callbacks.find("video_source_call_back");
                    if (it != _callbacks.end())
                    {
                        auto _isolate = v8_Isolate::GetCurrent();
                        v8_HandleScope scope(_isolate);
                        auto context = _isolate->GetCurrentContext();
                        v8_Local<v8_Value> argv[2]{
                            nan_create_local_value_string_(_isolate, _eventName.c_str()),
                            nan_create_local_value_string_(_isolate, _eventData.c_str()),
                        };
                        EventCallback &cb = *it->second;
                        auto _result = cb.callback.Get(_isolate)->Call(context, cb.js_this.Get(_isolate), 2, argv);
                        v8_MAYBE_LOCAL_CHECK_RESULT(_result);
                        LOG_F(INFO, "33NodeIrisEventHandler::OnVideoSourceEvent(const char* eventName, const char* eventData)");
              
                    }
                    LOG_F(INFO, "44NodeIrisEventHandler::OnVideoSourceEvent(const char* eventName, const char* eventData)");
              
                });
            }

            void NodeIrisEventHandler::OnVideoSourceEvent(const char* eventName, const char* eventData, const char* buffer, int length)
            {   
                return;

                std::string _eventName(eventName);
                std::string _eventData(eventData);
                char stringData[length + 1];
                stringData[length] = '\0';
                memcpy(stringData, buffer, length);
                std::string _eventBuffer(stringData);
                node_async_call::async_call([this, _eventName, _eventData, _eventBuffer] {
                    auto it = _callbacks.find("video_source_call_back_with_buffer");
                    if (it != _callbacks.end())
                    {
                        auto _isolate = v8_Isolate::GetCurrent();
                        v8_HandleScope scope(_isolate);
                        auto context = _isolate->GetCurrentContext();
                        v8_Local<v8_Value> argv[3]{
                            nan_create_local_value_string_(_isolate, _eventName.c_str()),
                            nan_create_local_value_string_(_isolate, _eventData.c_str()),
                            nan_create_local_value_string_(_isolate, _eventBuffer.c_str()),
                        };
                        EventCallback &cb = *it->second;
                        auto _result = cb.callback.Get(_isolate)->Call(context, cb.js_this.Get(_isolate), 2, argv);
                        v8_MAYBE_LOCAL_CHECK_RESULT(_result);
                    }
                });
            }

            void NodeIrisEventHandler::OnVideoSourceExit()
            {
                LOG_F(INFO, "OnVideoSourceExit");
            }

            void NodeIrisEventHandler::addEvent(const std::string &eventName, Nan_Persistent<v8_Object> &obj, Nan_Persistent<v8_Function> &callback)
            {
                auto _callback = new EventCallback();
                _callback->js_this.Reset(obj);
                _callback->callback.Reset(callback);
                _callbacks.emplace(eventName, _callback);
            }
        }
    }
}