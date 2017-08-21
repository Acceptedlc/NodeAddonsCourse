## 编译命令

__linux:__

```
cd helloworld

gyp build.gyp --depth=. -f make --generator-output=./build

make -C ./build/
```



__mac:__

```
cd helloworld

gyp build.gyp --depth=. -f xcode --generator-output=./build

xcodebuild -project ./build/xcodefiles/build.xcodeproj
```


#### gyp 命令行参数解释

* --depth：以当前目录作为根路径
* -f:输出的格式
* --generator-output：makefile 等中间文件输出到哪里


#### make 命令行参数解释

* -C
	* 在读取makefile文件前，先cd到"dir"目录下，即把dir当作为当前目录。
	* 如果存在多个-C选项，make的最终当前目录是第一个目录的相对路径
		* 如“make -C /home/root -C src”,等价于"make -C /home/root/src" 