# WJSON #
&emsp;&emsp;个人练手，启发于<https://github.com/miloyip/json-tutorial>，[json-tutorial](https://github.com/miloyip/json-tutorial)是基于C的，想写一个基于C++的。

## 正确性与性能 ##
&emsp;&emsp;通过[nativejson-benchmark](https://github.com/miloyip/nativejson-benchmark)所有测试。
由于系统和g++版本原因，该测试中部分库不能通过编译，因此我fork了一个，剔除了无法在ubuntu下编译的库。链接为<https://github.com/Weissle/nativejson-benchmark>。
## 项目中使用wjson ##
### 连接libwjson库 ###
```sh
#在项目根目录下执行以下命令
mkdir build && cd build
cmake ../
make
```
编译后在`build`目录下可以看到`libwjson.a`或`libwjson.lib`，把它链接到自己项目中即可
### 使用.h/.cpp文件 ###
直接把`include`和`src`下所有文件放到自己项目中即可；如果希望只有一个.h和.cpp可以执行下面的命令，会把所有.h和.cpp整合到json.h和json.cpp中。

```sh
git submodule update --init
sh tool/amalgamate_to_one.sh
```

## 用法 ##
```c++
//一共六种类型：Null,Bool,Number,String,Array,Object，左边是Json值类型，右边是其C++类型
Null => std::nullptr_t
Bool => bool
// Number 为自定义的类，具体可查看include/value/number.h
String => std::string
Array => std::deque<wjson::Json>
Object => std::map<std::string,wjson::Json>
```
```json
{
  "info": "假设我们有这么一个json字符串",
  "array": [false, "abcd", null, 123456]
}
```
### 准备工作 ###
```c++
//测试的输出设置，这不是必要的，只是展示时使用。
cout << boolalpha << setiosflags(ios::fixed) << setprecision(6);
```

### 解析json ###
```c++
wjson::Json json;
// 解析json，解析失败会抛出异常
json.parse("{  \"info\": \"假设我们有这么一个json字符串\",\
               \"array\": [false, \"abcd\", null, 123456]}");
```

### 检查类型 ###
```c++
// json.is<T>() T 可以是六种类型之一
cout << json.is<Object>() << endl; // true
cout << json["array"].is<Array>() << endl; // true
cout << json["info"].is<Null>() << endl; // false
```

### 获取值 ###
```c++
// json.get<T>() T 可以是六种类型之一
//如果类型不对会抛出异常
cout << json["array"][0].get<Bool>() << endl; //false
cout << json["array"][1].get<String>() << endl; // abcd
cout << json["array"][2].get<Null>() << endl; // nullptr ; 在dump和pretty函数中是null;
cout << json["array"][3].get<Number>() <<endl; // 123456

{
  try{
    auto &tmp = json["array"][2].get<Array>();
  }catch(exception e){
    cout << e.what() << endl;
  }
}
```

### 改变值的类型
```c++
// 默认类型为Null, 使用to<T>()改变类型; T 可以是六种类型之一
cout << json["unknow"].is<String>() << endl; // false
cout << json["unknow"].is<Null>() << endl; // true
json["unknow"].to<String>();
cout << json["unknow"].is<String>() << endl; // true
```

### 改变值的内容
```c++
// 改变值; 如果类型不对，会自动改变类型
json["unknow"] = 123456.789;
cout << json["unknow"].is<Number>() << endl; // true
cout << json["unknow"].get<Number>() << endl;// 123456
```

### Dump 和 Pretty
```c++
cout << json.dump() << endl;
// {"array":[false,"abcd",null,123456],"info":"假设我们有这么一个json字符串","unknow":123456.789000}
```
```c++
cout << json.pretty(2,' ') << endl;
/*
{
  "array":[
    false,
    "abcd",
    null,
    123456
  ],
  "info":"假设我们有这么一个json字符串",
  "unknow":123456.789000
}
*/
```

## 使用GTEST进行测试
请先安装google test, 如果没有google test，可以使用下面命令直接安装(ubuntu测试成功)
```sh
sh tool/install_googletest.sh
```
cmake的时候额外添加这个`-DBUILD_TEST=TRUE`，`make`结束后执行`make test`即可进行测试。

## TODO ##
* 为Array的修改添加更多的功能
* 添加更多的测试
* 修改Number的解析和类，使其更高效
