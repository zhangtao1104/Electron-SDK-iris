#include "node_iris_engine.h"

using namespace agora::rtc::electron;

void Init(v8_Local<v8_Object> module)
{
   // LOG_F(INFO, "NODE_MODULE node_agora_ext Init");
   NodeIrisEngine::Init(module);
   // MyObject::Init(module);
}
NODE_MODULE(NODE_GYP_MODULE_NAME, Init)