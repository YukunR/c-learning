# 控制语句

- [控制语句](#控制语句)
  - [条件判断](#条件判断)
    - [if 语句](#if-语句)
    - [if-else 语句](#if-else-语句)
    - [switch-case 语句](#switch-case-语句)
  - [循环语句](#循环语句)
    - [while 循环](#while-循环)
    - [for 循环](#for-循环)
    - [do-while 循环](#do-while-循环)
  - [控制跳转语句](#控制跳转语句)
    - [break, continue](#break-continue)
  - [练习 (来自 ChatGPT)](#练习-来自-chatgpt)
    - [题目](#题目)
    - [遇到的问题](#遇到的问题)

## 条件判断

### if 语句

```C
#include <stdio.h>

int main() {
    int score = 85;

    if (score >= 60) {
        printf("passed\n");
    }

    return 0;
}
```

### if-else 语句

```C
#include <stdio.h>

int main() {
    int score = 85;

    if (score >= 90) {
        printf("great\n");
    } else if (score >= 60) {
        printf("passed\n");
    } else {
        printf("failed\n");
    }
}
```

### switch-case 语句

```C
#include <stdio.h>

int main() {
    int option = 2;

    switch (option) {
        case 1:
            printf("option 1\n");
            break;
        case 2:
            printf("option 2\n");
            break;
        default:
            printf("invalid option\n");
            break;
    }
}
```

## 循环语句

### while 循环

```C
#include <stdio.h>

int main() {
    int i = 0;

    while (i <= 5) {
        printf("i = %d\n", i);
        i++;
    }

    return 0;
}
```

### for 循环

```C
#include <stdio.h>

int main() {
    for (int i = 1; i <= 5; i++) {
        printf("i = %d\n", i);
    }

    return 0;
}
```

### do-while 循环

```C
#include <stdio.h>

int main() {
    int i = 0;

    do {
        printf("i = %d\n", i);
        i++;
    } while (i <= 5);

    return 0;
}
```

## 控制跳转语句

### break, continue

```C
#include <stdio.h>

int main() {
    for (int i = 0; i <= 10; i++) {
        if (i == 3) continue;
        if (i > 5) break;
        printf("i == %d\n", i);
    }

    return 0;
}
```

## 练习 (来自 ChatGPT)

### 题目

编写一个简易 ATM 模拟程序，需要完成的功能如下：

1. 启动程序后，提示用户输入密码（假设密码是 1234）；
2. 用户有 最多 3 次机会 输入正确密码，否则程序退出；
3. 登录成功后，进入一个菜单循环，显示以下选项：

```markdown
    欢迎使用简易ATM系统
    1. 查询余额
    2. 存款
    3. 取款
    4. 修改密码
    5. 退出
```

4. 用户可以重复选择功能，直到选择“退出”；
5. 每个功能的具体行为如下：
   - 查询余额：输出当前余额（初始为 1000 元）；
   - 存款：提示用户输入存款金额，金额需为正；
   - 取款：提示用户输入取款金额，金额需为正且不得超过余额；
   - 修改密码：输入旧密码验证后可设置新密码；
   - 退出：结束程序。

### 遇到的问题

1. 编译后运行：coderunner 无法运行，改为编译 (快捷键 Ctrl + Shift + B) 后 在 vscode 的 terminal 中运行；
2. 中文显示问题：中文显示会乱码，解决方法：[powershell 打开中文乱码问题的解决办法](https://blog.csdn.net/plasma007/article/details/128259243)。修改后运行 `005_chinese_output_test.bat` 看到正常中文输出即可。
