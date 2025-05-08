# 文件操作

C 语言通过标准库函数实现对文件的读写操作，常用于保存数据、读取配置或生成日志等。

## 一、文件操作的基本流程

```text
打开文件 → 读/写数据 → 关闭文件
```

## 二、常用函数概览

| 步骤     | 函数                                                  | 用法示例                           |
| -------- | ----------------------------------------------------- | ---------------------------------- |
| 打开文件 | `FILE *fopen(const char *filename, const char *mode)` | `fopen("file.txt", "r")`           |
| 写入文本 | `fprintf(FILE *fp, const char *format, ...)`          | `fprintf(fp, "hello %s", name);`   |
| 读取文本 | `fscanf(FILE *fp, const char *format, ...)`           | `fscanf(fp, "%d", &x);`            |
| 读一行   | `fgets(char *str, int size, FILE *fp)`                | `fgets(buf, 100, fp);`             |
| 关闭文件 | `int fclose(FILE *fp)`                                | `fclose(fp);`                      |
| 判断结尾 | `int feof(FILE *fp)`                                  | `while (!feof(fp)) { ... }`        |
| 二进制读 | `size_t fread(void *, size_t, size_t, FILE *)`        | `fread(arr, sizeof(int), n, fp);`  |
| 二进制写 | `size_t fwrite(const void *, size_t, size_t, FILE *)` | `fwrite(arr, sizeof(int), n, fp);` |

---

## 三、文件打开模式（mode）

| 模式   | 含义                                   |
| ------ | -------------------------------------- |
| `"r"`  | 只读模式，文件必须存在                 |
| `"w"`  | 只写模式，文件不存在则创建，存在则清空 |
| `"a"`  | 追加写模式，文件不存在则创建           |
| `"r+"` | 读写模式，文件必须存在                 |
| `"w+"` | 读写模式，文件存在则清空               |
| `"a+"` | 读写模式，数据追加到末尾               |

---

## 四、写入文本文件示例

```c
#include <stdio.h>

int main() {
    FILE *fp = fopen("output.txt", "w");
    if (fp == NULL) {
        printf("文件打开失败！\n");
        return 1;
    }

    fprintf(fp, "Hello, world!\n");
    fprintf(fp, "写入一行文本数据。\n");

    fclose(fp);
    return 0;
}
```

## 读取文本文件示例

```c
#include <stdio.h>

int main() {
    FILE *fp = fopen("output.txt", "r");
    if (fp == NULL) {
        printf("文件打开失败！\n");
        return 1;
    }

    char buffer[100];
    while (fgets(buffer, sizeof(buffer), fp)) {
        printf("读取内容：%s", buffer);
    }

    fclose(fp);
    return 0;
}
```

## 文件操作注意事项

- `fopen()` 打开失败时返回 `NULL`，应始终检查；
- 使用 `fclose()` 关闭文件，避免资源泄漏；
- 文本文件换行在 Windows 是 `\r\n`，在 Linux 是 `\n`；
- 对于二进制数据（结构体等），用 `fread()` 和 `fwrite()` 更合适；
- 多次写入时，可使用 `"a"` 或 `"a+"` 以追加方式打开文件。

## 常用头文件

```c
#include <stdio.h>   // 文件读写主函数库
#include <stdlib.h>  // exit()
```

## 练习 (来自 ChatGPT)

### 读取并统计学生成绩

请编写一个 C 程序，读取一个包含学生姓名和成绩的文本文件 `scores.txt`，按行处理并统计数据。

### 功能要求

1. 打开 `scores.txt` 文件；
2. 使用 `fgets()` 一行一行读取文件；
3. 每行格式为：`<姓名> <成绩>`，例如 `Alice 88.5`；
4. 将每位学生的姓名与成绩打印出来；
5. 最后输出：
   - 总共的学生人数；
   - 所有学生的平均成绩。

---

### 示例输出

```
读取学生信息：
Alice 的成绩是 88.50
Bob 的成绩是 92.00
Charlie 的成绩是 79.00
David 的成绩是 85.50

共有 4 位学生，平均成绩为 86.75
```

### 示例文件内容（scores.txt）

```
Alice 88.5
Bob 92.0
Charlie 79.0
David 85.5
```

> 请将该文件保存在程序运行目录下。

---

### 编程提示

- 使用 `FILE *fp = fopen("scores.txt", "r");` 打开文件；
- 使用 `fgets(buffer, sizeof(buffer), fp)` 读取每一行；
- 使用 `sscanf(buffer, "%s %f", name, &score)` 或 `strtok` + `atof` 分割并转换；
- 使用计数变量统计人数，累加求平均；
- 最后使用 `fclose(fp);` 关闭文件。

### 限制条件

- 不使用结构体或指针运算；
- 所有字符串使用 `char name[20];`；
- 不需要从命令行输入文件名；
- 每行最大不超过 100 个字符。

### 拓展建议（可选）

- 将学生信息写入另一个文件（如 `report.txt`）；