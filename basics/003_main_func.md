# 运行 C 程序的快捷键与问题解决

## 快捷键说明

在 VS Code 中，运行当前 C 文件的快捷键为（使用 `Code Runner` 插件）：

```
Ctrl + Alt + N
```

## 🛠️ 初次运行时遇到的问题

运行后出现如下输出:

```bash
[Running] cd "d:\c_learn\basics\" && gcc 003_main_func.c -o 003_main_func && "d:\c_learn\basics\"003_main_func
'gcc' �����ڲ����ⲿ���Ҳ���ǿ����еĳ���
���������ļ���

[Done] exited with code=1 in 0.054 seconds
```

分析：打开 cmd 输入相同命令无报错，因此确定是 vscode 没有识别新的环境变量。

## 解决方法

- **彻底重启 VS Code**，关闭所有 VSCode 窗口，重新打开，让其重新加载最新的系统环境变量。
- 再次运行，结果如下：

```bash
[Running] cd "d:\c_learn\basics\" && gcc 003_main_func.c -o 003_main_func && "d:\c_learn\basics\"003_main_func

[Done] exited with code=0 in 0.246 seconds
```
