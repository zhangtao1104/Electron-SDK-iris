#pragma once
#include <string>
#include <memory>
#include "iris_engine.h"
#include "video_source_ipc.h"
#include "video_source_iris_event_handler.h"
#include "video_source_param_parser.h"
#include "node_process.h"



namespace agora {
    namespace rtc {
        namespace electron {
            class VideoSource : public AgoraIpcListener{
                public:
                    VideoSource();
                    ~VideoSource();

                    virtual void OnMessage(unsigned int msg, char* payload, unsigned int len);

                    bool initialize(std::string& parameter);
                    void run();
                    void release();
                    void exit(bool flag);

                private:
                    std::unique_ptr<VideoSourceIrisEventhandler> _irisEventHandler;
                    std::shared_ptr<IAgoraIpc> _ipcController;
                    std::unique_ptr<AgoraIpcDataSender> _ipcSender;
                    std::unique_ptr<iris::IrisEngine> _irisEngine;
                    std::unique_ptr<VideoSourceParamParser> _parameterParser;
                    std::unique_ptr<INodeProcess> _process;
                    std::atomic_bool _initialize {false};
            };  
        }
    }
}