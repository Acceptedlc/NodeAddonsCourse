## simple Addon


### 1. src/hello.cpp

```
#include <node.h> //引入node的头文件

void hello(const v8::FunctionCallbackInfo<v8::Value>& args){
    v8::Isolate* isolate = args.GetIsolate();//获取v8虚拟机
    args.GetReturnValue().Set(v8::String::NewFromUtf8(isolate, "world"));//设置返回值
}

void init(v8::Local<v8::Object> exports) {
    NODE_SET_METHOD(exports, "hello", hello);
}

NODE_MODULE(hello, init)//所有node模块必须提供一个初始化方法


```


### 2. binding.gyp

```
{
    "targets":[
        {
            "target_name":"hello",
            "sources":["src/hello.cpp"]
        }
    ]
}
```

