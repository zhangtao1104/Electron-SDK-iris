#pragma once
#include <nan.h>
#include <node.h>
#include <string>
#include <memory>

namespace agora
{
    namespace rtc
    {
        namespace electron
        {
            template <typename T>
            using Nan_Persistent = Nan::Persistent<T>;
            template <typename T>
            using Nan_FunctionCallbackInfo = Nan::FunctionCallbackInfo<T>;
            template <typename T>
            using v8_FunctionCallbackInfo = v8::FunctionCallbackInfo<T>;
            template <typename T>
            using v8_Local = v8::Local<T>;
            template <class T>
            using v8_Maybe = v8::Maybe<T>;
            using Nan_Utf8_String = Nan::Utf8String;
            using v8_Object = v8::Object;
            using v8_Value = v8::Value;
            using v8_Context = v8::Context;
            using v8_Isolate = v8::Isolate;
            using v8_FunctionTemplate = v8::FunctionTemplate;
            using v8_String = v8::String;
            using v8_Uint32 = v8::Uint32;
            using v8_Int32 = v8::Int32;
            using v8_Function = v8::Function;
            using v8_HandleScope = v8::HandleScope;
            using v8_External = v8::External;
            using v8_Array = v8::Array;
            using v8_ArrayBuffer = v8::ArrayBuffer;
            using v8_Boolean = v8::Boolean;
            using v8_Uint8Array = v8::Uint8Array;

#define v8_SET_OBJECT_PROP_STRING(isolate, object, name, value)                                                     \
    {                                                                                                               \
        auto propName = v8_String::NewFromUtf8(isolate, name, v8::NewStringType::kInternalized).ToLocalChecked();   \
        auto propValue = v8_String::NewFromUtf8(isolate, value, v8::NewStringType::kInternalized).ToLocalChecked(); \
        auto result = object->Set(isolate->GetCurrentContext(), propName, propValue);                               \
        v8_MAYBE_CHECK_RESULT(result);                                                                              \
    }

#define v8_SET_OBJECT_PROP_UINT32(isolate, object, name, value)                                                   \
    {                                                                                                             \
        auto propName = v8_String::NewFromUtf8(isolate, name, v8::NewStringType::kInternalized).ToLocalChecked(); \
        auto propValue = v8_Uint32::New(isolate, value);                                                          \
        auto result = object->Set(isolate->GetCurrentContext(), propName, propValue);                             \
        v8_MAYBE_CHECK_RESULT(result);                                                                            \
    }

#define v8_SET_OBJECT_PROP_INT32(isolate, object, name, value)                                                    \
    {                                                                                                             \
        auto propName = v8_String::NewFromUtf8(isolate, name, v8::NewStringType::kInternalized).ToLocalChecked(); \
        auto propValue = v8_Int32::New(isolate, value);                                                           \
        auto result = object->Set(isolate->GetCurrentContext(), propName, propValue);                             \
        v8_MAYBE_CHECK_RESULT(result);                                                                            \
    }

#define v8_SET_OBJECT_PROP_BOOL(isolate, obj, name, val)                                                          \
    {                                                                                                             \
        auto propName = v8_String::NewFromUtf8(isolate, name, v8::NewStringType::kInternalized).ToLocalChecked(); \
        auto propVal = v8_Boolean::New(isolate, val);                                                             \
        auto result = obj->Set(isolate->GetCurrentContext(), propName, propVal);                                  \
        v8_MAYBE_CHECK_RESULT(result);                                                                            \
    }

#define v8_SET_OBJECT_PROP_UINT8_ARRAY(isolate, obj, name, buffer, size)                                          \
    {                                                                                                             \
        auto propName = v8_String::NewFromUtf8(isolate, name, v8::NewStringType::kInternalized).ToLocalChecked(); \
        auto arrayBuffer = v8_ArrayBuffer::New(isolate, size);                                                    \
        memcpy(arrayBuffer->GetBackingStore()->Data(), buffer, size);                                             \
        auto uint8Array = v8_Uint8Array::New(arrayBuffer, 0, size);                                               \
        auto result = obj->Set(isolate->GetCurrentContext(), propName, uint8Array);                               \
        v8_MAYBE_CHECK_RESULT(result);                                                                            \
    }

#define v8_MAYBE_CHECK_RESULT(maybe) \
    {                                \
        if (maybe.IsNothing())       \
        {                            \
        }                            \
    }

#define v8_MAYBE_LOCAL_CHECK_RESULT(maybe) \
    {                                      \
        if (maybe.IsEmpty())               \
        {                                  \
        }                                  \
    }

            std::string nan_api_get_value_utf8string_(const v8_Local<v8_Value> &value);
            int nan_api_get_value_int32_(const v8_Local<v8_Value> &value);
            v8_Local<v8_Value> nan_create_local_value_string_(v8_Isolate *isolate, const char *value);
        }
    }
}
