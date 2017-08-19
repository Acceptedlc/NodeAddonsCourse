#include <node.h>

void hello(const v8::FunctionCallbackInfo<Value>& args){
    Isolate* isolate = args.GetIsolate();
    args.GetReturnValue().Set(String::NewFromUtf8(isolate, "world"));
}

void init(Local<Object> exports) {
    NODE_SET_METHOD(exports, "hello", hello);
}

NODE_MODULE(addon, init)