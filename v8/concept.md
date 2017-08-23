## Handle

handle提供堆内存中js对象的引用。  
当一个对象在 JavaScript 中无法被访问到, 并且也没有任何 Handle 引用它, 则这个对象将被当作 "垃圾" 对待. 

handle有很多种类型，最常用的有

* Local
* Persistent

#### Local handle

Local Handles 保存在一个栈结构（Handle 栈并不是 C++ 调用栈的一部分，这里的栈值得是handle scope）中， 当栈的析构函数(destructor)被调用时将同时被销毁.