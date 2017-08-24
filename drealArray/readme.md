## 场景说明

延续上一个例子，这里主要演示如何在js与c++之间处理数组


## js调用

```
//rainfall.js

var rainfall = require("./build/Release/rainfall");

var makeup = function(max) {
    return Math.round(max * Math.random() * 100)/100;
}

var locations = []
for (var i = 0; i < 10; i++ ) {
    var loc = {
        latitude: makeup(180),
        longitude: makeup(180),
        samples : [
            {date: "2015-07-20", rainfall: makeup(3)},
            {date: "2015-07-21", rainfall: makeup(3)},
            {date: "2015-07-22", rainfall: makeup(3)},
            {date: "2015-07-23", rainfall: makeup(3)}
        ]
    }
    locations.push(loc);
}

var results = rainfall.calculate_results(locations);
console.log(JSON.stringify(results,null,2));


```



## 与node进行绑定


__初始化函数实现，给exports上增加一个calculate_results方法__

```
NODE_SET_METHOD(exports, "calculate_results", CalculateResults);

```


__CalculateResults实现__

```
void CalculateResults(const v8::FunctionCallbackInfo<v8::Value>&args) {
  Isolate* isolate = args.GetIsolate();
  std::vector<location> locations;  // we'll get this from Node.js
  std::vector<rain_result> results; // we'll build this in C++
  
  // 将第一个参数转化为数组
  Local<Array> input = Local<Array>::Cast(args[0]);
  unsigned int num_locations = input->Length();
  //转化为location类
  for (unsigned int i = 0; i < num_locations; i++) {
    locations.push_back(
           unpack_location(isolate, Local<Object>::Cast(input->Get(i))));
  }

  //调整返回数组的大小
  results.resize(locations.size());
  for(unsigned int i = 0;i < num_locations; i++){
    results.push_back(locations[i].calc_rain_stats());
  }
  
  // 在v8中创建一个数组
  Local<Array> result_list = Array::New(isolate);
  for (unsigned int i = 0; i < results.size(); i++ ) {
  	 //在v8 中创建一个object
    Local<Object> result = Object::New(isolate);
    pack_rain_result(isolate, result, results[i]);
    //将result 翻入result_list
    result_list->Set(i, result);
  }
  // 设置返回值
  args.GetReturnValue().Set(result_list);
}

```

**pack_rain_result**

```
void pack_rain_result(v8::Isolate* isolate, v8::Local<v8::Object> & target, rain_result & result){
  target->Set(String::NewFromUtf8(isolate, "mean"), Number::New(isolate, result.mean));
  target->Set(String::NewFromUtf8(isolate, "median"), Number::New(isolate, result.median));
  target->Set(String::NewFromUtf8(isolate, "standard_deviation"), Number::New(isolate, result.standard_deviation));
  target->Set(String::NewFromUtf8(isolate, "n"), Integer::New(isolate, result.n));
}

```


