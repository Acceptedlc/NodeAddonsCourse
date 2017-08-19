#include <string>
#include <iostream>
#include "include/libplatform/libplatform.h"
#include "include/v8.h"

using namespace v8;
std::string MainSource(){
    using namespace std;
    string bootstrap_js_code = "(function(process){process.log('hello world');})";
    return bootstrap_js_code; 
}

int main(int argc, char *argv[]) {
    V8::InitializeICUDefaultLocation(argv[0]);
    V8::InitializeExternalStartupData(argv[0]);
    Platform* platform = platform::CreateDefaultPlatform();
    V8::InitializePlatform(platform);
    V8::Initialize();

    Isolate::CreateParams create_params;
    create_params.array_buffer_allocator =
        v8::ArrayBuffer::Allocator::NewDefaultAllocator();
    Isolate* isolate = Isolate::New(create_params); 
    {
        Isolate::Scope isolate_scope(isolate);
        HandleScope handle_scope(isolate);
        v8::Local<v8::Context> context = Context::New(isolate);
        Context::Scope context_scope(context);
        Local<String> source = String::NewFromUtf8(isolate, MainSource().c_str(), NewStringType::kNormal, MainSource().size()).ToLocalChecked();
        Local<Script> script = Script::Compile(context, source).ToLocalChecked();
        Local<Value> result = script->Run(context).ToLocalChecked();
        std::cout << result->IsFunction() << std::endl;
    }
    isolate->Dispose();
    V8::Dispose();
    V8::ShutdownPlatform();
    delete platform;
    delete create_params.array_buffer_allocator;
    return 0;
}

