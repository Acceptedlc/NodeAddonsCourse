### 初始化

```
void init(Handle <Object> exports, Handle<Object> module) {
  NODE_SET_METHOD(module, "exports", CreateObject);
}


NODE_MODULE(addon, init)  

```

### 方法实现

```
void CreateObject(const FunctionCallbackInfo<Value>& args) {
  Isolate* isolate = args.GetIsolate();

  Local<Object> obj = Object::New(isolate);
  obj->Set(String::NewFromUtf8(isolate, "msg"), args[0]->ToString());

  args.GetReturnValue().Set(obj);
}
```