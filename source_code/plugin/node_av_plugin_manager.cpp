// #include "node_base.h"
// #include "node_av_plugin_manager.h"
// #include "IAVFramePlugin.h"
// #include "node_log.h"
// #include <mutex>


// namespace agora {
//     namespace rtc {
//         namespace electron {

//                 using iris::IrisAudioFrameObserver;
//                 using iris::IrisVideoFrameObserver;
                
//                 IAVFramePluginManager::~IAVFramePluginManager()
//                 {
//                     this->release();
//                 }

//                 bool IAVFramePluginManager::OnRecordAudioFrame(AudioFrame &audio_frame)
//                 {
//                     std::lock_guard<std::mutex> lock(_plugin_mutex);

//                     for (auto _iterator = _plugin_list.begin(); _iterator != _plugin_list.end(); _iterator ++) {
//                         (*_iterator)._instance->onPluginRecordAudioFrame((AudioPluginFrame *)&audio_frame);
//                     }
//                     return true;
//                 }

//                 bool IAVFramePluginManager::OnPlaybackAudioFrame(AudioFrame &audio_frame)
//                 {
//                     std::lock_guard<std::mutex> lock(_plugin_mutex);
//                     for (auto _iterator = _plugin_list.begin(); _iterator != _plugin_list.end(); _iterator ++) {
//                         (*_iterator)._instance->onPluginPlaybackAudioFrame((AudioPluginFrame *)&audio_frame);
//                     }
//                     return true;
//                 }

//                 bool IAVFramePluginManager::OnMixedAudioFrame(AudioFrame &audio_frame)
//                 {
//                     std::lock_guard<std::mutex> lock(_plugin_mutex);
//                     for (auto _iterator = _plugin_list.begin(); _iterator != _plugin_list.end(); _iterator ++) {
//                         (*_iterator)._instance->onPluginMixedAudioFrame((AudioPluginFrame *)&audio_frame);
//                     }
//                     return true;
//                 }

//                 bool IAVFramePluginManager::OnPlaybackAudioFrameBeforeMixing(unsigned uid, AudioFrame &audio_frame)
//                 {
//                     std::lock_guard<std::mutex> lock(_plugin_mutex);
//                     for (auto _iterator = _plugin_list.begin(); _iterator != _plugin_list.end(); _iterator ++) {
//                         (*_iterator)._instance->onPluginPlaybackAudioFrameBeforeMixing(uid, (AudioPluginFrame *)&audio_frame);
//                     }
//                     return true;
//                 }

//                 bool IAVFramePluginManager::OnPlaybackAudioFrameBeforeMixingEx(const char *channel_id, unsigned uid, AudioFrame &audio_frame)
//                 {
//                     return false;
//                 }

//                 bool IAVFramePluginManager::OnCaptureVideoFrame(VideoFrame &video_frame)
//                 {
//                     std::lock_guard<std::mutex> lock(_plugin_mutex);
//                     for (auto _iterator = _plugin_list.begin(); _iterator != _plugin_list.end(); _iterator ++) {
//                         (*_iterator)._instance->onPluginCaptureVideoFrame((VideoPluginFrame *)&video_frame);
//                     }
//                     return true;
//                 }

//                 bool IAVFramePluginManager::OnPreEncodeVideoFrame(VideoFrame &video_frame)
//                 {
//                     std::lock_guard<std::mutex> lock(_plugin_mutex);
//                     for (auto _iterator = _plugin_list.begin(); _iterator != _plugin_list.end(); _iterator ++) {
//                         (*_iterator)._instance->onPluginPreEncodeVideoFrame((VideoPluginFrame *)&video_frame);
//                     }
//                     return true;
//                 }

//                 bool IAVFramePluginManager::OnRenderVideoFrame(unsigned uid, VideoFrame &video_frame)
//                 {
//                     std::lock_guard<std::mutex> lock(_plugin_mutex);
//                     for (auto _iterator = _plugin_list.begin(); _iterator != _plugin_list.end(); _iterator ++) {
//                         (*_iterator)._instance->onPluginRenderVideoFrame(uid, (VideoPluginFrame *)&video_frame);
//                     }
//                     return true;
//                 }

//                 iris::IrisVideoFrameObserver::VideoFrameType IAVFramePluginManager::GetVideoFormatPreference()
//                 {
//                     return kFrameTypeYUV420;
//                 }

//                 bool IAVFramePluginManager::GetRotationApplied()
//                 {
//                     return true;
//                 }

//                 bool IAVFramePluginManager::GetMirrorApplied()
//                 {
//                     return true;
//                 }

//                 bool IAVFramePluginManager::GetSmoothRenderingEnabled()
//                 {
//                     return false;
//                 }

//                 uint32_t IAVFramePluginManager::GetObservedFramePosition()
//                 {
//                     return static_cast<uint32_t>(kPositionPostCapturer | kPositionPreRenderer);
//                 }

//                 bool IAVFramePluginManager::OnRenderVideoFrameEx(const char *channel_id, unsigned uid, VideoFrame &video_frame)
//                 {
//                     return false;
//                 }

//                 int IAVFramePluginManager::registerPlugin(const char *id, const char *path, int priority)
//                 {
//                     std::lock_guard<std::mutex> lock(_plugin_mutex);
//                     agora_plugin_info _plugin_info(id, priority);
//                     auto _ret = this->loadLibrary(_plugin_info, path);
//                     if (_ret == ERROR_OK) {
//                         this->addPluginToList(_plugin_info);
//                     }
//                     return _ret;
//                 }

//                 int IAVFramePluginManager::unRegisterPlugin(const char *id)
//                 {
//                     std::lock_guard<std::mutex> lock(_plugin_mutex);
//                     auto _ret = this->unLoadLibrary(id);
//                     this->removePluginFromList(id);
//                     return _ret;
//                 }

//                 void IAVFramePluginManager::addPluginToList(agora_plugin_info& pluginInfo)
//                 {
//                     if (_plugin_list.empty()) {
//                         _plugin_list.push_back(pluginInfo);
//                         return;
//                     }

//                     for (auto _iterator = _plugin_list.begin(); _iterator != _plugin_list.end(); ++_iterator) {
//                         if ((*_iterator)._priority < pluginInfo._priority) {
//                             _plugin_list.insert(_iterator, pluginInfo);
//                             return;
//                         }
//                     }

//                     _plugin_list.push_back(pluginInfo);
//                     return;   
//                 }

//                 void IAVFramePluginManager::removePluginFromList(const char *id)
//                 {
//                     for (auto _iterator = _plugin_list.begin(); _iterator != _plugin_list.end(); ++_iterator) {
//                         if ((*_iterator)._id == id) {
//                             _plugin_list.erase(_iterator);
//                         }
//                     }
//                 }

//                 bool IAVFramePluginManager::hasPlugin(const char *id)
//                 {
//                     for (auto _iterator = _plugin_list.begin(); _iterator != _plugin_list.end(); _iterator ++) {
//                         if ((*_iterator)._id == id) {
//                             return true;
//                         }
//                     }
//                     return false;
//                 }

//                 bool IAVFramePluginManager::enablePlugin(const char *id, bool enable)
//                 {
//                     for (auto _iterator = _plugin_list.begin(); _iterator != _plugin_list.end(); _iterator ++) {
//                         if ((*_iterator)._id == id) {
//                             (*_iterator)._enabled = enable;
//                             return true;
//                         }
//                     }
//                     return false;
//                 }

//                 bool IAVFramePluginManager::getPlugin(const char *id, agora_plugin_info& pluginInfo)
//                 {
//                     for (auto _iterator = _plugin_list.begin(); _iterator != _plugin_list.end(); _iterator ++) {
//                         if ((*_iterator)._id == id) {
//                             strncpy(pluginInfo._id, (*_iterator)._id, MAX_PLUGIN_ID);
//                             pluginInfo._enabled = (*_iterator)._enabled;
//                             pluginInfo._instance = (*_iterator)._instance;
//                             pluginInfo._plugin_module = (*_iterator)._plugin_module;
//                             pluginInfo._priority = (*_iterator)._priority;
//                             return true;
//                         }
//                     }
//                     return false;
//                 }

//                 std::vector<std::string> IAVFramePluginManager::getPlugins()
//                 {
//                     std::vector<std::string> result;
//                     for (auto _iterator = _plugin_list.begin(); _iterator != _plugin_list.end(); _iterator ++) {
//                         result.push_back(std::string((*_iterator)._id));
//                     }
//                     return result;
//                 }

//                 int IAVFramePluginManager::loadLibrary(agora_plugin_info& _plugin_info, const char *path)
//                 {
//                     std::string _plugin_folder_path, _plugin_path = path;
//                     const size_t _last_slash_idx = _plugin_path.find_last_of("\\/");

//                     if (std::string::npos != _last_slash_idx)
//                     {
//                         _plugin_folder_path = _plugin_path.substr(0, _last_slash_idx + 1);
//                     }

//                     #ifdef WIN32
//                     char* wPluginFilePath = U2G(_plugin_path.c_str());
//                     _plugin_info._plugin_module = LoadLibraryEx(wPluginFilePath, NULL, LOAD_WITH_ALTERED_SEARCH_PATH);
//                     delete[] wPluginFilePath;
//                     DWORD error = GetLastError();
//                     LOG_ERROR("LoadLibrary Error :%ld", error);
                    
//                     if (!_plugin_info._plugin_module)
//                         return error;

//                     createAgoraAVFramePlugin _createPlugin = (createAgoraAVFramePlugin)GetProcAddress((HMODULE)_plugin_info._plugin_module, "createAVFramePlugin");
//                     if (!_createPlugin) {
//                         FreeLibrary((HMODULE)_plugin_info._plugin_module);
//                         _plugin_info._plugin_module = NULL;
//                         return ERROR_PARAMETER_2;
//                     }
//                     #else
//                     _plugin_info._plugin_module =dlopen(_plugin_path.c_str(), RTLD_LAZY);

//                     if (!_plugin_info._plugin_module)
//                         return ERROR_PARAMETER_1;

//                     createAgoraAVFramePlugin _createPlugin = (createAgoraAVFramePlugin)dlsym(_plugin_info._plugin_module, "createAVFramePlugin"); 
//                     if (!_createPlugin) {
//                         dlclose(_plugin_info._plugin_module);
//                         _plugin_info._plugin_module = NULL;
//                         return ERROR_PARAMETER_2;
//                     }
//                     #endif

//                     _plugin_info._instance = _createPlugin();
//                     if (!_plugin_info._instance)
//                         return ERROR_PARAMETER_3;

//                     #ifdef WIN32
//                     char* wPluginFolderPath = U2G(_plugin_folder_path.c_str());
//                     if (_plugin_info._instance->load(wPluginFolderPath) != 0) {
//                         return ERROR_PARAMETER_4;
//                     }
//                     delete[] wPluginFolderPath;
//                     #else
//                     if (_plugin_info._instance->load(_plugin_folder_path.c_str()) != 0) {
//                         return ERROR_PARAMETER_4;
//                     }
//                     #endif

//                     _plugin_info._enabled = false;
//                     return ERROR_OK;
//                 }

//                 int IAVFramePluginManager::unLoadLibrary(const char *id)
//                 {
                    
//                     for (auto _iterator = _plugin_list.begin(); _iterator != _plugin_list.end(); _iterator ++) {
//                         if ((*_iterator)._id == id) {
//                             (*_iterator)._instance->release();

//                             if ((*_iterator)._plugin_module) {
//                             #ifdef WIN32
//                                 FreeLibrary((HMODULE)((*_iterator)._plugin_module));
//                             #else
//                                 dlclose((*_iterator)._plugin_module);
//                             #endif
//                             }
//                         }
//                     }
//                     return ERROR_OK;
//                 }

//                 int IAVFramePluginManager::release()
//                 {
//                     std::lock_guard<std::mutex> lock(_plugin_mutex);
//                     for (auto _iterator = _plugin_list.begin(); _iterator != _plugin_list.end(); _iterator ++) {
//                         this->unLoadLibrary((*_iterator)._id);
//                         this->removePluginFromList((*_iterator)._id);
//                     }
//                     _plugin_list.clear();
//                     return ERROR_OK;
//                 }
//         }
//     }
// }