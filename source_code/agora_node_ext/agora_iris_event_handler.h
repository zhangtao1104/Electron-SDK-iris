#pragma once
#include "iris_event_handler.h"
#include "nan_api.h"
#include <unordered_map>
#include "node_async_queue.h"
#include "video_source_event_handler.h"

namespace agora
{
    namespace rtc
    {
        namespace electron
        {
            class AgoraIrisEventHandler : public iris::IrisEventHandler, public IVideoSourceEventHandler
            {
            public:
                typedef struct NodeEventCallback
                {
                    Nan_Persistent<v8_Function> callback;
                    Nan_Persistent<v8_Object> js_this;
                } EventCallback;

            public:
                virtual ~AgoraIrisEventHandler();

                virtual void OnEvent(const char *event, const char *data) override;

                virtual void OnEvent(const char *event, const char *data, const void *buffer, unsigned length) override;

                virtual void OnVideoSourceEvent(const char* eventName, const char* eventData) override;

                virtual void OnVideoSourceEvent(const char* eventName, const char* eventData, const char* buffer, int length) override;

                virtual void OnVideoSourceExit() override;

                void addEvent(const std::string &eventName, Nan_Persistent<v8_Object> &obj, Nan_Persistent<v8_Function> &callback);

            private:
                std::unordered_map<std::string, EventCallback *> _callbacks;
            };
        }
    }
}