
// #pragma once
// #include "iris_event_handler.h"
// #include <string>
// #include <vector>
// #include <list>
// #include <mutex>
// #ifdef _WIN32
// #include <Windows.h>
// #elif defined(__APPLE__)
// #include <dlfcn.h>
// #endif

// #define MAX_PLUGIN_ID   512

// class IAVFramePlugin;

// namespace agora {
//     namespace rtc {
//         namespace electron {

//             struct agora_plugin_info {
//             public:
//                 agora_plugin_info (const char *id, int priority):_priority(priority),_plugin_module(nullptr),_instance(nullptr) {
//                     strncpy(_id, id, MAX_PLUGIN_ID);
//                 }

//                 char _id[MAX_PLUGIN_ID];
//                 void *_plugin_module;
//                 IAVFramePlugin *_instance;
//                 bool _enabled;
//                 int _priority;
//             };

//             class IAVFramePluginManager : public iris::IrisAudioFrameObserver, public iris::IrisVideoFrameObserver {
//             public:
//                 IAVFramePluginManager() = default;
//                 virtual ~IAVFramePluginManager();

//                 virtual bool OnRecordAudioFrame(iris::IrisAudioFrameObserver::AudioFrame &audio_frame) override;

//                 virtual bool OnPlaybackAudioFrame(iris::IrisAudioFrameObserver::AudioFrame &audio_frame) override;

//                 virtual bool OnMixedAudioFrame(iris::IrisAudioFrameObserver::AudioFrame &audio_frame) override;

//                 virtual bool OnPlaybackAudioFrameBeforeMixing(unsigned uid, iris::IrisAudioFrameObserver::AudioFrame &audio_frame) override;

//                 virtual bool OnPlaybackAudioFrameBeforeMixingEx(const char *channel_id, unsigned uid, iris::IrisAudioFrameObserver::AudioFrame &audio_frame) override;

//                 virtual bool OnCaptureVideoFrame(iris::IrisVideoFrameObserver::VideoFrame &video_frame) override;

//                 virtual bool OnPreEncodeVideoFrame(iris::IrisVideoFrameObserver::VideoFrame &video_frame) override;

//                 virtual bool OnRenderVideoFrame(unsigned uid, iris::IrisVideoFrameObserver::VideoFrame &video_frame) override;

//                 virtual iris::IrisVideoFrameObserver::VideoFrameType GetVideoFormatPreference() override;

//                 virtual bool GetRotationApplied() override;

//                 virtual bool GetMirrorApplied() override;

//                 virtual bool GetSmoothRenderingEnabled() override;

//                 virtual uint32_t GetObservedFramePosition() override;

//                 virtual bool OnRenderVideoFrameEx(const char *channel_id, unsigned uid, iris::IrisVideoFrameObserver::VideoFrame &video_frame) override;

               
//                 int loadLibrary(agora_plugin_info& pluginInfo, const char *path);

//                 int unLoadLibrary(const char *id);

//                 int registerPlugin(const char *id, const char *path, int priority);

//                 int unRegisterPlugin(const char *id);

//                 bool hasPlugin(const char *id);

//                 bool enablePlugin(const char *id, bool enable);

//                 bool getPlugin(const char *id, agora_plugin_info& pluginInfo);

//                 void addPluginToList(agora_plugin_info& pluginInfo);

//                 void removePluginFromList(const char *id);

//                 std::vector<std::string> getPlugins();

//                 int release();

//             private:
//                 std::list<agora_plugin_info> _plugin_list;
//                 std::mutex _plugin_mutex;
//             };
//         }
//     }
// }