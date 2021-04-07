#pragma once

namespace agora {
    namespace rtc {
        namespace electron { 
            class IVideoSourceEventHandler {
                public:
                    virtual ~IVideoSourceEventHandler();

                    virtual void OnVideoSourceEvent(const char* eventName, const char* eventData) = 0;

                    virtual void OnVideoSourceEvent(const char* eventName, const char* eventData, const char* buffer, int length) = 0;

                    virtual void OnVideoSourceExit() = 0;
            };
        }
    }
}