#include "node_iris_engine.h"

using namespace agora::rtc::electron;

void Init(v8_Local<v8_Object> module)
{
   NodeIrisEngine::Init(module);
}
NODE_MODULE(NODE_GYP_MODULE_NAME, Init)