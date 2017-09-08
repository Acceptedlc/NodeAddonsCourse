### package.json

```
{
	"gypfile":true, //说明这个一个addon，需要node-gyp编译
	"scripts": {
    	//执行的脚本
	},
}
```


### scripts 默认值

```
"start": "node server.js"，
"install": "node-gyp rebuild"
```

一般来说，npm 脚本由用户提供。但是，npm 对两个脚本提供了默认值。也就是说，这两个脚本不用定义，就可以直接使用。

上面代码中:

* npm run start的默认值是node server.js，前提是项目根目录下有server.js这个脚本；
* npm run install的默认值是node-gyp rebuild，前提是项目根目录下有binding.gyp文件。

### 钩子

npm 脚本有pre和post两个钩子。举例来说，build脚本命令的钩子就是prebuild和postbuild。

用户执行npm run build的时候，会自动按照下面的顺序执行。

```
npm run prebuild && npm run build && npm run postbuild
```

npm 默认提供下面这些钩子。

```
prepublish，postpublish
preinstall，postinstall
preuninstall，postuninstall
preversion，postversion
pretest，posttest
prestop，poststop
prestart，poststart
prerestart，postrestart
```

