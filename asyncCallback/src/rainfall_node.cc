#include <node.h>
#include <v8.h>
#include "rainfall.h" 
#include <iostream>
#include <uv.h>

using namespace v8;

sample unpack_sample(Isolate * isolate, const Handle<Object> sample_obj) {
  sample s;
  Handle<Value> date_Value = sample_obj->Get(String::NewFromUtf8(isolate, "date"));
  Handle<Value> rainfall_Value = sample_obj->Get(String::NewFromUtf8(isolate, "rainfall"));

  v8::String::Utf8Value utfValue(date_Value);
  s.date = std::string(*utfValue);

  // Unpack the numeric rainfall amount directly from V8 value
  s.rainfall = rainfall_Value->NumberValue();
  return s;
}

void pack_rain_result(v8::Isolate* isolate, v8::Local<v8::Object> & target, rain_result & result){
  target->Set(String::NewFromUtf8(isolate, "mean"), Number::New(isolate, result.mean));
  target->Set(String::NewFromUtf8(isolate, "median"), Number::New(isolate, result.median));
  target->Set(String::NewFromUtf8(isolate, "standard_deviation"), Number::New(isolate, result.standard_deviation));
  target->Set(String::NewFromUtf8(isolate, "n"), Integer::New(isolate, result.n));
}

location unpack_location(Isolate * isolate, const Local<Object>& location_obj) {
    location loc;
    Handle<Value> lat_Value = 
                  location_obj->Get(String::NewFromUtf8(isolate,"latitude"));
    Handle<Value> lon_Value = 
                  location_obj->Get(String::NewFromUtf8(isolate,"longitude"));
    loc.latitude = lat_Value->NumberValue();
    loc.longitude = lon_Value->NumberValue();
  
    Handle<Array> array =  Handle<Array>::Cast(
                          location_obj->Get(
                                String::NewFromUtf8(isolate,"samples")));
  
    int sample_count = array->Length();
    for ( int i = 0; i < sample_count; i++ ) {
      sample s = unpack_sample(isolate, Handle<Object>::Cast(array->Get(i)));
      loc.samples.push_back(s);
    }
    return loc;
}

struct Work {
  uv_work_t  request;
  Persistent<Function> callback;

  std::vector<location> locations;
  std::vector<rain_result> results;
};

static void WorkAsync(uv_work_t *req){
  // 从libuv 的数据载体中获取数据
  Work *work = static_cast<Work *>(req->data);

  work->results.resize(work->locations.size());
  
  for(unsigned int i = 0;i < work->locations.size(); i++){
    work->results.push_back(work->locations[i].calc_rain_stats());
  }
}

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



void init(Handle <Object> exports, Handle<Object> module) {
  NODE_SET_METHOD(exports, "calculate_results_async", CalculateResultsAsync); 
}


NODE_MODULE(rainfall, init)  