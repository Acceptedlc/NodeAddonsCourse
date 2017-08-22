## 场景说明
计算某个地区的平均降雨量（经纬度作为地区的唯一标识）


## js调用

```
var rainfall = require("./build/Release/rainfall.node");
var location = {
    latitude : 40.71, longitude : -74.01,
       samples : [
          { date : "2014-06-07", rainfall : 2 },
          { date : "2014-08-12", rainfall : 0.5}
       ] };

console.log("Average rain fall = " + rainfall.avg_rainfall(location) + "cm");
```

## 定义类型

__location:__ 

```
class location {
public:
  double longitude;
  double latitude;
  vector<sample> samples; //观测到的样本数据
  double avg_rainfall();
};
```



__sample__

```
class sample {
public:
  sample ();
  sample (string d, double r);

  string date;
  double rainfall;
};
```

## 实现类

```
double location::avg_rainfall() {
    double total = 0;
    for (const auto &sample : this->samples) {
      total += sample.rainfall;
    }
    return total / this->samples.size();
}

sample::sample() {
  date = ""; rainfall = 0;
}

sample::sample (string d, double r)  {
  date = d;
  rainfall = r;
}
```

## 与node进行绑定

__绑定模块初始化函数__

```
NODE_MODULE(rainfall, init)  
```

__初始化函数实现，给exports上增加一个avg_rainfall方法__

```
void init(Handle <Object> exports, Handle<Object> module) {
    NODE_SET_METHOD(exports, "avg_rainfall", AvgRainfall);
}
```


__AvgRainfall实现__

```
void AvgRainfall(const FunctionCallbackInfo<Value> & args) {
    Isolate* isolate = args.GetIsolate(); //获取v8 实例
    
    location loc = unpack_location(isolate, args); //将js数据转化为，刚刚生命的c++类
    double avg = loc.avg_rainfall(); //计算平均降雨量
  
    Local<Number> retval = Number::New(isolate, avg); //将c语言返回值，变为js的number
    args.GetReturnValue().Set(retval); //设置返回值
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




