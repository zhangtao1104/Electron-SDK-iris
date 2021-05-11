#pragma once
#include <memory>
#include <string>
#include <thread>
#ifdef _WIN32
#include <Rpc.h>
#include <RpcDce.h>
#else
#include <uuid/uuid.h>
#endif
#include "video_source_event_handler.h"
#include "video_source_ipc.h"
#include "iris_rtc_engine.h"
#include "node_process.h"
#include "node_event.h"
#include <iosfwd>
#include <sstream>
#include <fstream>
#include "loguru.hpp"



namespace agora {
    namespace rtc {
        namespace electron {
            class VideoSourceProxy: public AgoraIpcListener {
            private:
                std::atomic_bool _initialized {false};
                std::shared_ptr<IVideoSourceEventHandler> _videoSourceEventHandlerPtr;
                std::string _peerId;
                std::unique_ptr<IAgoraIpc> _iAgoraIpc;
                std::unique_ptr<AgoraIpcDataReceiver> _iAgoraIpcDataReceiver;
                std::unique_ptr<INodeProcess> _iNodeProcess;
                std::thread _messageThread;
                NodeEvent _statusEvent;

            public:
                VideoSourceProxy();
                ~VideoSourceProxy();

                virtual void OnMessage(unsigned int msg, char* payload, unsigned int len) override; 
                bool Initialize(IVideoSourceEventHandler *videoSourceEventHandler, std::string& parameter); 
                void LoopMessage();
                int CallApi(ApiTypeEngine apiType, const char* parameter, char* result);
                int CallApi(ApiTypeEngine apiType, const char* parameter, const char* buffer, int length, char* result);
                int PluginCallApi(ApiTypeRawDataPlugin apiType, const char* parameter, char* result);
                int Release();
                int EnableVideoFrameCache(const char *channelId, unsigned int uid, int width, int height);
                int DisableVideoFrameCache(const char *channelId, unsigned int uid);
                void Clear();
                void OnApiError(const char* event, const char* data);
            };
        }
    }
}