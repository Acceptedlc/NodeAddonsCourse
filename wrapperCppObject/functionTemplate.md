## FunctionTemplate

FunctionTemplate 是v8的一种数据类型

通过FunctionTemplate只能生成一个function，并且生成的function的生命周期与content一致

一旦，生成方法后任何对于FunctionTemplate的修改会造成错误

### 1.给function对象添加属性

**c++**

```
v8::Local<v8::FunctionTemplate> t = v8::FunctionTemplate::New();
t->Set("func_property", v8::Number::New(1));
```

**js**

```
func_property in function == true;
function.func_property == 1;
```


### 2. 原型链模板

相当于函数的原型链

**c++**

```
v8::Local<v8::FunctionTemplate> t = v8::FunctionTemplate::New();

v8::Local<v8::Template> proto_t = t->PrototypeTemplate();
proto_t->Set("proto_method", v8::FunctionTemplate::New(InvokeCallback));
proto_t->Set("proto_const", v8::Number::New(2));
```

**js**

```
function.prototype.proto_method() invokes 'c++的InvokeCallback'
function.prototype.proto_const == 2;
```

### 3. 实例模板

指的是，将function当做构造函数，创建出来的对象的模板

**c++**

```
v8::Local<v8::ObjectTemplate> instance_t = t->InstanceTemplate();
instance_t->SetAccessor("instance_accessor", InstanceAccessorCallback);
instance_t->SetNamedPropertyHandler(PropertyHandlerCallback, ...);
instance_t->Set("instance_property", Number::New(3));
```

**js**

```
instance instanceof function == true;
instance.instance_accessor calls 'InstanceAccessorCallback'
instance.instance_property == 3;
```

### 4. 使用

```
//通过模板构造出一个方法
v8::Local<v8::Function> function = t->GetFunction();
//通过上面的方法，构造一个对象
v8::Local<v8::Object> instance = function->NewInstance();
```

### 5. 内部属性（internal field）

SetInternalFieldCount方法告诉v8，当使用模板创建对象的时候，分配一些内部存储空间，这些存储空间只有c++可以访问，js无法访问

通过FunctionTemplate构造出一个构造函数，可以为InstanceTemplate设置internal field，在这些存储单元中，保存js的对象与c++对象的关联