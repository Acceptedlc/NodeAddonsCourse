## Handle

handle提供堆内存中js对象的引用。  
当一个对象在 JavaScript 中无法被访问到, 并且也没有任何 Handle 引用它, 则这个对象将被当作 "垃圾" 对待. 

handle有很多种类型，最常用的有

* Local
* Persistent

#### Local handle

Local Handles 保存在一个栈结构（Handle 栈并不是 C++ 调用栈的一部分，这里的栈值得是handle scope）中， 当栈的析构函数(destructor)被调用时将同时被销毁.

这里有一个小的困惑，在Node的addones中，不创建HandleScope 直接创建很多Local Handle，垃圾回首依然正常，这有可能是在调用这个function的时候Node或者v8自动创建了吧。在Stack Overflow上，有人问过，答案是如果这样做会造成内存泄漏