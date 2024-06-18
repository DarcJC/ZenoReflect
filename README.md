# Zeno Reflect

[English](docs/README-en.md) | [中文](docs/README-zh.md)

## Quickstart

1. 把本项目作为子模块添加到你的项目中
2. 引入本项目作
```cmake
add_subdirectory(/path/to/ZenoReflect)
```
3. 然后在你需要反射的模块中使用（参考`examples`文件夹下内容）
```cmake
make_absolute_paths(REFLECTION_HEADERS 
    include/data.h
    include/test.h
) 
zeno_declare_reflection_support(ReflectExample "${REFLECTION_HEADERS}")
```

## FQA

> Q: 有什么限制
> A: 不能有同名类型同时拥有反射标记，且头文件名不应重名

