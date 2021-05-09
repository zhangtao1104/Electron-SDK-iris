#pragma once
#include <string>
#include <memory>
#include "iris_rtc_engine.h"
#include "video_source_ipc.h"
#include "video_source_iris_event_handler.h"
#include "video_source_param_parser.h"
#include "node_process.h"
#include "iris_rtc_raw_data.h"
#include "iris_rtc_raw_data_plugin_manager.h"
#include "iris_rtc_base.h"
#include "loguru.hpp"



namespace agora {
    namespace rtc {
        namespace electron {
            class VideoSource : public AgoraIpcListener{
                public:
                    VideoSource();
                    ~VideoSource();

                    virtual void OnMessage(unsigned int msg, char* payload, unsigned int len) override;

                    bool Initialize(std::string& parameter);
                    void Run();
                    void Release();
                    void Exit(bool flag);
                    void OnApiError(const char* event, const char* data);

                private:
                    std::unique_ptr<VideoSourceIrisEventhandler> _iris_event_handler;
                    std::unique_ptr<IAgoraIpc> _ipc_controller;
                    std::unique_ptr<AgoraIpcDataSender> _ipc_sender;
                    std::unique_ptr<iris::rtc::IrisRtcEngine> _iris_engine;
                    std::shared_ptr<iris::rtc::IrisRtcRawData> _iris_raw_data;
                    std::shared_ptr<iris::rtc::IrisRtcRawDataPluginManager> _iris_raw_data_plugin_manager;
                    std::unique_ptr<VideoSourceParamParser> _parameter_parser;
                    std::unique_ptr<INodeProcess> _process;
                    std::atomic_bool _initialize {false};
            };  
        }
    }
}