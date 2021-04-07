#include "nan_api.h"

namespace agora
{
    namespace rtc
    {
        namespace electron
        {
            std::string nan_api_get_value_utf8string_(const v8_Local<v8_Value> &value)
            {
                Nan_Utf8_String _dataValue(value);
                return std::string(*_dataValue);
            }

            int nan_api_get_value_int32_(const v8_Local<v8_Value> &value)
            {
                return Nan::To<v8_Int32>(value).ToLocalChecked()->Value();
            }

            v8_Local<v8_Value> nan_create_local_value_string_(v8_Isolate *isolate, const char *value)
            {
                return v8_String::NewFromUtf8(isolate, value ? value : "", v8::NewStringType::kInternalized).ToLocalChecked();
            }
        }
    }
}