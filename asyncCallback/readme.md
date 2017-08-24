## 场景说明

本实例主要演示，在nodejs addones中如何异步执行一个计算任务，并通过回调的形式回传给js


## 数据结构声明

下面的结构体用于在libuv的主线程（也就是执行event loop的线程）和工作线程之间交换数据

```
struct Work {
  //这个数据结构是libuv在不同线程中传递数据的载体。
  //先将从主线程，搬到工作线程，运算结束后，再搬回主线程
  uv_work_t  request; 
  
  // 保存js传入的回调函数
  Persistent<Function> callback; 
  
  // 工作线程的输入数据
  std::vector<location> locations; 
  // 工作线程的输出数据
  std::vector<rain_result> results;
};

```

## 绑定Addones

```
void init(Handle <Object> exports, Handle<Object> module) {
  NODE_SET_METHOD(exports, "calculate_results_async", CalculateResultsAsync); 
}


NODE_MODULE(rainfall, init)  
```

**CalculateResultsAsync**

```
void CalculateResultsAsync(const v8::FunctionCallbackInfo<v8::Value>&args) {
  Isolate* isolate = args.GetIsolate();
  //线程使用自己的函数栈，因此只能通过堆来共享数据
  Work * work = new Work(); 
  work->request.data = work;

  // 将js传入的数据转换为c++的location
  Local<Array> input = Local<Array>::Cast(args[0]);
  unsigned int num_locations = input->Length();
  for (unsigned int i = 0; i < num_locations; i++) {
    work->locations.push_back(unpack_location(isolate, Local<Object>::Cast(input->Get(i))));
  }

  // 获取第二个参数，也就是回调函数
  Local<Function> callback = Local<Function>::Cast(args[1]);
  // 将回调函数转变为Persistent Handle
  work->callback.Reset(isolate, callback);

  // 启动工作线程，WorkAsync 是工作线程执行的函数，WorkAsyncComplete 工作线程完成工作，回到主线程执行的函数
  uv_queue_work(uv_default_loop(),&work->request,WorkAsync,WorkAsyncComplete);

  args.GetReturnValue().Set(Undefined(isolate));
}

```

## 工作线程执行的任务 

```
static void WorkAsync(uv_work_t *req){
  // 从libuv 的数据载体中获取数据
  Work *work = static_cast<Work *>(req->data);

  work->results.resize(work->locations.size());
  
  for(unsigned int i = 0;i < num_locations; i++){
    work->results.push_back(locations[i].calc_rain_stats());
  }
}
```

## 任务结束主线程执行的函数


```
static void WorkAsyncComplete(uv_work_t *req,int status){
  Isolate * isolate = Isolate::GetCurrent();
  
  v8::HandleScope handleScope(isolate);

  Local<Array> result_list = Array::New(isolate);
  Work *work = static_cast<Work *>(req->data);

  for (unsigned int i = 0; i < work->results.size(); i++ ) {
    Local<Object> result = Object::New(isolate);
    pack_rain_result(isolate, result, work->results[i]);
    result_list->Set(i, result);
  }

  Handle<Value> argv[] = { Null(isolate) , result_list };
  //调用回调函数
  Local<Function>::New(isolate, work->callback)->Call(isolate->GetCurrentContext()->Global(), 2, argv);

  // 删除CalculateResultsAsync 创建的Persistent Handle
  work->callback.Reset();
  // 删除堆中的work
  delete work;
}

```

