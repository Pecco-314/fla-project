# FLA-Project
本项目是南京大学《形式语言与自动机》课程的课程项目，实现了多带图灵机的解析和运行。

## 依赖
为了构建本项目，需要满足以下依赖条件：
1. CMake：版本3.15.0或更高
2. 编译器：支持C++17标准的C++编译器，例如GCC（版本7.0或更高）

## 构建
本项目使用CMake进行构建。进入项目文件夹下，然后执行命令：

```bash
cmake -B build && cmake --build build
```

如果成功，会在项目文件夹下创建一个`build`文件夹和一个`bin`文件夹，你可以在`bin`文件夹中找到一个名为`turing`的可执行文件。你可以执行以下命令来检验构建是否成功：

```bash
./bin/turing --help
```

如果打印出帮助信息，说明构建成功。

默认情况下构建的是Release版本，如果想构建Debug版本，可以执行以下命令：

```bash
cmake -B build_debug -DCMAKE_BUILD_TYPE=Debug && cmake --build build_debug
```

如果成功，你可以在`bin`文件夹中找到一个名为`turing_debug`的可执行文件。

## 运行
基本用法为：

```bash
turing <tm_path> <input>
```

其中，`tm_path`表示图灵机描述文件的路径，`input`表示输入，这里的输入是直接写在命令行里的。

通常情况下，程序只会打印图灵机运行的结果，但可以使用`--verbose`或`-v`参数来获取更加详细的输出。

可以使用`--help`或`-h`参数获取帮助信息。
