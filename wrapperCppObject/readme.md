### 场景

本实例演示如果通过c++ addone产生一个function，返回给js

### Init方法

* 创建一个[FunctionTemplate](./functionTemplate.md)
* 设置FunctionTemplate的InstanceTemplate的内部属性个数
* 设置方法名称
* 设置PrototypeTemplate
* 通过FunctionTemplate生成方法，并绑定到exports上
* 将Function或者FunctionTemplate使用Persistent Handle保存（这个根据具体情况）
