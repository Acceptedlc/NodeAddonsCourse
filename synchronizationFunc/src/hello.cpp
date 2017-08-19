#include <node.h>

void hello(const v8::FunctionCallbackInfo<v8::Value>& args){
    v8::Isolate* isolate = args.GetIsolate();
    args.GetReturnValue().Set(v8::String::NewFromUtf8(isolate, "world"));
}

void init(v8::Local<v8::Object> exports) {
    NODE_SET_METHOD(exports, "hello", hello);
}

NODE_MODULE(hello, init)