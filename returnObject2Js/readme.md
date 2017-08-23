## 场景说明

延续上一个例子，这里增加一个新的函数返**data_rainfall**，返回的是一个js的object


## js调用

```
//rainfall.js

var data = rainfall.data_rainfall(location);

console.log("Mean = " + data.mean)
console.log("Median = " + data.median);
console.log("Standard Deviation = " + data.standard_deviation);
console.log("N = " + data.n);
```

## 定义类型

__rain_result:__ 在头文件中声明返回object的c++类型

```
class rain_result {
public:
    float median;
    float mean;
    float standard_deviation;
    int n;
};

```




## 实现类

**给localtion，增加实现的方法**

```
rain_result location::calc_rain_stats() {
	rain_result result;
	double ss = 0;
	double total = 0;
	
	result.n = this->samples.size();

	for (const auto &sample : this->samples) {
   	 total += sample.rainfall;
  	}
  	result.mean = total / this->samples.size();
  	
  	for (const auto &sample : this->samples) {
   	 ss += pow(sample.rainfall - result.mean, 2);
  	}
  	result.standard_deviation = sqrt(ss/(result.n-1));

  	std::sort(this->samples.begin(), this->samples.end());
	if (result.n %2 == 0) {
		result.median = (this->samples[result.n / 2 - 1].rainfall + this->samples[result.n / 2].rainfall) / 2;
	}
	else {
		result.median = this->samples[result.n / 2].rainfall;
	}
	return result;
}

```

## 与node进行绑定


__初始化函数实现，给exports上增加一个data_rainfall方法__

```
void init(Handle <Object> exports, Handle<Object> module) {
	NODE_SET_METHOD(exports, "data_rainfall", RainfallData);
}
```


__RainfallData实现__

```
void RainfallData(const v8::FunctionCallbackInfo<v8::Value>& args) {
  //获取虚拟机
  Isolate* isolate = args.GetIsolate();
  
  // 将js传递进来的参数，转化为c++对应的类型
  location loc = unpack_location(isolate, args);   
  
  // 调用刚刚实现的方法，并且获得一个rain_result类型的返回值
  rain_result result = loc.calc_rain_stats(); 
  
  // 在v8的堆上创建一个对象
  Local<Object> obj = Object::New(isolate);

  // 设置对象的key 和value
  obj->Set(String::NewFromUtf8(isolate, "mean"), 
    Number::New(isolate, result.mean));
  obj->Set(String::NewFromUtf8(isolate, "median"), 
    Number::New(isolate, result.median));
  obj->Set(String::NewFromUtf8(isolate, "standard_deviation"), 
    Number::New(isolate, result.standard_deviation));
  obj->Set(String::NewFromUtf8(isolate, "n"), 
    Integer::New(isolate, result.n));

  // 设置返回值
  args.GetReturnValue().Set(obj);

}
```

**unpack_location**

```
location unpack_location(Isolate * isolate, const FunctionCallbackInfo<Value>& args) {
    location loc;
    Handle<Object> location_obj = Handle<Object>::Cast(args[0]);//获取传入的js object
    Handle<Value> lat_Value = 
                  location_obj->Get(String::NewFromUtf8(isolate,"latitude")); //获得latitude这个key的value
    Handle<Value> lon_Value = 
                  location_obj->Get(String::NewFromUtf8(isolate,"longitude"));//获得longitude这个key的value
    loc.latitude = lat_Value->NumberValue();//提取latitude
    loc.longitude = lon_Value->NumberValue();//提取longitude
  
    Handle<Array> array =  Handle<Array>::Cast(
                          location_obj->Get(
                                String::NewFromUtf8(isolate,"samples")));//获得samples这个key的value，并转化为js中的Array
  
    int sample_count = array->Length();
    for ( int i = 0; i < sample_count; i++ ) {
      sample s = unpack_sample(isolate, Handle<Object>::Cast(array->Get(i)));//将js中表示sample的object 转化为刚刚生命的c++ class
      loc.samples.push_back(s);
    }
    return loc;
  }
```

**unpack_sample**

```
sample unpack_sample(Isolate * isolate, const Handle<Object> sample_obj) {
  sample s;
  Handle<Value> date_Value = sample_obj->Get(String::NewFromUtf8(isolate, "date"));
  Handle<Value> rainfall_Value = sample_obj->Get(String::NewFromUtf8(isolate, "rainfall"));

  v8::String::Utf8Value utfValue(date_Value);
  s.date = std::string(*utfValue);//将v8中的string 转化为std的string

  // Unpack the numeric rainfall amount directly from V8 value
  s.rainfall = rainfall_Value->NumberValue();
  return s;
}
```

## 编译文件

```
{
  "targets": [
    {
      "target_name": "rainfall",
      "sources": [ "src/rainfall.cc" , "src/rainfall_node.cc" ],
      "cflags": ["-Wall", "-std=c++11"],//-Wall 提供所有警告信息 -std=c++11 使用标准c++11
      "include_dirs":['include'], //头文件引入路径
      "xcode_settings": { //os x 连接器设置
        "OTHER_CFLAGS": [
          "-std=c++11"
        ]
      }
    }
  ]
}
```




