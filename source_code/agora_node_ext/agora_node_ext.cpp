/*
 * @Author: zhangtao@agora.io 
 * @Date: 2021-04-22 20:52:36 
 * @Last Modified by:   zhangtao@agora.io 
 * @Last Modified time: 2021-04-22 20:52:36 
 */

#include "node_iris_engine.h"

using namespace agora::rtc::electron;

void Init(v8_Local<v8_Object> module)
{
   // LOG_F(INFO, "NODE_MODULE node_agora_ext Init");
   NodeIrisEngine::Init(module);
   // MyObject::Init(module);
}
NODE_MODULE(NODE_GYP_MODULE_NAME, Init)