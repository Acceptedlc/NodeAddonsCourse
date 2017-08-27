### 场景

本实例演示如果通过c++ addone产生一个function，返回给js

### CreateFunction

```
void CreateFunction(const FunctionCallbackInfo<Value>& args) {
  Isolate* isolate = args.GetIsolate();
	
//创建一个function 模板，与c++的函数MyFunction绑定
  Local<FunctionTemplate> tpl = FunctionTemplate::New(isolate, MyFunction);
//通过function模板，创建一个function的实例  
  Local<Function> fn = tpl->GetFunction();

  // 如果不给fn设置方法名称，则是一个匿名函数
  fn->SetName(String::NewFromUtf8(isolate, "theFunction"));

  args.GetReturnValue().Set(fn);
}

//js代码通过调用CreateFunction 获得了下面函数的实例
void MyFunction(const FunctionCallbackInfo<Value>& args) {
  Isolate* isolate = args.GetIsolate();
  args.GetReturnValue().Set(String::NewFromUtf8(isolate, "hello world"));
}

```