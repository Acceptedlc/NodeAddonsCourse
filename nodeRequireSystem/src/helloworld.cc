#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include "include/libplatform/libplatform.h"
#include "include/v8.h"

using namespace v8;

const char* ToCString(const String::Utf8Value& value) {
    return *value ? *value : "<string conversion failed>";
}

static void LogCallback(const v8::FunctionCallbackInfo<v8::Value>& args){
    if (args.Length() < 1) return;
    HandleScope scope(args.GetIsolate());
    Local<Value> arg = args[0];
    String::Utf8Value value(arg);
    const char* name = ToCString(value);
    printf("%s\n",name);
}

int main(int argc, char *argv[]) {
    // Initialize V8
    V8::InitializeICUDefaultLocation(argv[0]);
    V8::InitializeExternalStartupData(argv[0]);
    Platform* platform = platform::CreateDefaultPlatform();
    V8::InitializePlatform(platform);
    V8::Initialize();

    // Create a new Isolate and make it the current one.
    Isolate::CreateParams create_params;
    create_params.array_buffer_allocator =
              v8::ArrayBuffer::Allocator::NewDefaultAllocator();
    Isolate* isolate = Isolate::New(create_params); 
    {
        Isolate::Scope isolate_scope(isolate); 
        HandleScope handle_scope(isolate);
        Local<ObjectTemplate> global = ObjectTemplate::New(isolate);
        global->Set(
                String::NewFromUtf8(isolate, "log", NewStringType::kNormal).ToLocalChecked(),
                FunctionTemplate::New(isolate, LogCallback)
        );
        //Local<Context> context = Context::New(isolate);
        v8::Local<v8::Context> context = Context::New(isolate, NULL, global);
        Context::Scope context_scope(context);
        Local<String> source = String::NewFromUtf8(isolate, "log('hello world')", NewStringType::kNormal, 18).ToLocalChecked();
        Local<Script> script = Script::Compile(context, source).ToLocalChecked();
        Local<Value> result = script->Run(context).ToLocalChecked();
    }

    isolate->Dispose();
    V8::Dispose();
    V8::ShutdownPlatform();
    delete platform;
    delete create_params.array_buffer_allocator;
    return 0;
}










        //            String::NewFromUtf8(isolate, "'吾皇万岁万岁，万万岁'",
        //                                                NewStringType::kNormal).ToLocalChecked();
        //String::Utf8Value utf8(result);
        //printf("%s\n", *utf8);
