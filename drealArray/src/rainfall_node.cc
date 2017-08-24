#include <node.h>
#include <v8.h>
#include "rainfall.h" 
#include <iostream>

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


void CalculateResults(const v8::FunctionCallbackInfo<v8::Value>&args) {
  Isolate* isolate = args.GetIsolate();
  std::vector<location> locations;  // we'll get this from Node.js
  std::vector<rain_result> results; // we'll build this in C++
  
  // we'll populate this with the results
  Local<Array> input = Local<Array>::Cast(args[0]);
  unsigned int num_locations = input->Length();
  for (unsigned int i = 0; i < num_locations; i++) {
    locations.push_back(
           unpack_location(isolate, Local<Object>::Cast(input->Get(i))));
  }

  results.resize(locations.size());
  for(unsigned int i = 0;i < num_locations; i++){
    results.push_back(locations[i].calc_rain_stats());
  }
  
  Local<Array> result_list = Array::New(isolate);
  for (unsigned int i = 0; i < results.size(); i++ ) {
    Local<Object> result = Object::New(isolate);
    pack_rain_result(isolate, result, results[i]);
    result_list->Set(i, result);
  }


  args.GetReturnValue().Set(result_list);
}


void init(Handle <Object> exports, Handle<Object> module) {

    NODE_SET_METHOD(exports, "calculate_results", CalculateResults);
}


NODE_MODULE(rainfall, init)  