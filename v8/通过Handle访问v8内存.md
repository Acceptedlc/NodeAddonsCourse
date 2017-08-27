handle可以理解为v8管理的内存的指针，与bv8引擎交互的时候通过Handle来获取，修改v8虚拟机中的内存

handle有很多种，最常用的有两类Local，和Persistent

### Local

Local 可以理解为js中的栈（handle scope）。一旦handle scope结束，那么所有的Local都会消失。  

Node.js 在调用addones方法的时候，是不需要显示生命Handle scope的，因为v8会在调用前创建好


#### Persistent

如果要利用libuv去执行异步操作的时候，显然需要一个变量保存回调函数的，这个时候Persistent就排场用场了

但是，有一个问题需要注意：**node addones中，如果你试图在其他线程访问Persistent，会收到一个错误反馈**

这是因为Nodejs对v8虚拟机加锁了

代码在node.cc StartNodeInstance 这个方法中

```
    Locker locker(isolate);
    Isolate::Scope isolate_scope(isolate);
```

只有当主线程放弃这个Locker的时候，其他线程才能获得锁，接近v8虚拟机

