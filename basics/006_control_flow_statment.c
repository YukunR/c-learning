/* 
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
*/

#include <stdio.h>

int main() {
    int password = 1234;
    int login_attempt = 0;
    int balance = 1000;
    int entered_password;
    // menu
    const char* MENU = 
    "欢迎使用简易 ATM 系统\n"
    "1. 余额查询\n"
    "2. 存款\n"
    "3. 取款\n"
    "4. 修改密码\n"
    "5. 退出\n";

    // login loop
    while (1) {
        if (login_attempt >= 3) {
            printf("失败三次，自动退出！");
            return 0;
        }
        printf("请输入密码：");
        scanf("%d", &entered_password);
        if (entered_password == password) {
            login_attempt = 0;
            break;
        } else {
            printf("密码错误，请重试。\n");
            login_attempt++;
        }
    }

    while (1) {
        printf("%s", MENU);

        int option, deposit_amount, withdraw_amount, new_password;
        printf("请输入选项：");
        scanf("%d", &option);
        switch (option) {
            case 1:  // 余额查询
                printf("余额为: %d 元\n", balance);
                break;
            case 2:  // 存钱
                printf("请输入存入金额：");
                scanf("%d", &deposit_amount);
                if (deposit_amount < 0) {
                    printf("存入金额不能少于 0 元！\n");
                } else {
                    balance += deposit_amount;
                    printf("存入成功！\n");
                }
                break;
            case 3:  // 取钱
                printf("请输入取出金额：");
                scanf("%d", &withdraw_amount);
                if (withdraw_amount < 0) {
                    printf("取出金额不能少于 0 元！\n");
                } else if (withdraw_amount > balance) {
                    printf("取出金额大于余额，现有余额 %d 元", balance);
                } else {
                    balance -= withdraw_amount;
                    printf("取款成功！\n");
                }
                break;
            case 4:  // 修改密码
                printf("请输入旧密码：");
                scanf("%d", &entered_password);
                if (entered_password == password) {
                    printf("请输入新密码：");
                    scanf("%d", &new_password);
                    password = new_password;
                    printf("密码修改成功！\n");
                } else {
                    printf("密码错误！\n");
                }
                break;
            case 5:
                printf("退出系统\n");
                return 0;
            default:
                printf("非法选项，请重新输入\n");
                break;
        }
    }
    
    return 0;
}