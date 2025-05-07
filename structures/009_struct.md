# 结构体（struct）

## 什么是结构体？

结构体（`struct`）是 C 语言中用于组织多个不同类型变量的自定义复合数据类型。

它可以打包一组数据，比如描述一个人（名字 + 年龄 + 分数），适用于描述实体对象。

## 结构体的基本语法

### 定义结构体类型

```c
struct Student {
    char name[20];
    int age;
    float score;
};
```

### 声明结构体变量

```c
struct Student s1;
```

### 结构体变量赋值与访问

```c
strcpy(s1.name, "Alice");  // 不能直接用 = 赋字符串
s1.age = 20;
s1.score = 95.5;

printf("姓名：%s，年龄：%d，成绩：%.2f\n", s1.name, s1.age, s1.score);
```

## 结构体数组

```c
struct Student class[3];
class[0].age = 18;
```

## 结构体与函数配合

### 结构体作为参数传递

```c
void print_student(struct Student s) {
    printf("学生：%s，%d岁，%.2f分\n", s.name, s.age, s.score);
}
```

> 结构体作为参数是按值传递（会复制），较大的结构体建议使用指针传递。

## typedef 简化定义

```c
typedef struct {
    char name[20];
    int age;
} Person;

Person p1;
```

---

## 结构体嵌套结构体

```c
struct Date {
    int year, month, day;
};

struct Book {
    char title[50];
    float price;
    struct Date pub_date;  // 嵌套 Date 结构体
};
```

## 结构体模拟继承（进阶）

虽然 C 不支持面向对象，但可以用结构体嵌套 + 函数指针模拟继承和多态。

```c
struct Animal {
    char name[20];
    void (*speak)(void);
};

struct Dog {
    struct Animal base;
    int bark_level;
};

void dog_speak() {
    printf("Woof!\n");
}

struct Dog d;
strcpy(d.base.name, "Buddy");
d.base.speak = dog_speak;
d.base.speak();  // 输出 Woof!
```

## 结构体与返回值（用于多个值返回）

```c
struct Result {
    int max;
    int min;
};

struct Result get_min_max(int a, int b) {
    struct Result r;
    r.max = (a > b) ? a : b;
    r.min = (a < b) ? a : b;
    return r;
}
```

## 总结

| 功能           | 示例                      |
| -------------- | ------------------------- |
| 定义结构体类型 | `struct Student { ... };` |
| 声明变量       | `struct Student s1;`      |
| 访问成员       | `s1.name`, `s1.age`       |
| 使用 `typedef` | `typedef struct {...} T;` |
| 结构体数组     | `struct Book books[10];`  |
| 函数传参       | `void show(struct S s)`   |
| 函数返回结构体 | `struct S get_info();`    |
| 模拟继承       | 结构体嵌套 + 函数指针     |

## 练习 (来自 ChatGPT)
### 题目：学生成绩管理系统

请你编写一个 C 程序，使用结构体记录 **最多 5 个学生的基本信息和成绩**，并实现以下功能。

### 功能要求：

1. 定义一个 `Student` 结构体，包含以下字段：
   - 姓名（`char name[20]`）
   - 学号（`int id`）
   - 成绩（`float score`）

2. 允许用户输入不超过 5 个学生的信息（实际人数由用户决定）；

3. 输出所有学生的信息；

4. 找出**最高分**的学生并输出其姓名、学号和分数；

5. 按照分数从高到低对学生排序，并输出排序结果。


### 示例运行：

```
请输入学生人数（最多5）：3

请输入第1个学生信息：
姓名：Alice
学号：1001
成绩：88.5

请输入第2个学生信息：
姓名：Bob
学号：1002
成绩：91.0

请输入第3个学生信息：
姓名：Charlie
学号：1003
成绩：79.0

--- 所有学生信息 ---
Alice    1001    88.50
Bob      1002    91.00
Charlie  1003    79.00

--- 成绩最高的学生 ---
Bob      1002    91.00

--- 成绩排序（降序） ---
Bob      1002    91.00
Alice    1001    88.50
Charlie  1003    79.00
```

### 知识点要求：

| 模块           | 要求                                       |
|----------------|--------------------------------------------|
| 结构体         | 使用 `struct Student` 定义学生信息结构      |
| 结构体数组     | 使用 `Student students[5];` 存储多个学生信息 |
| 基本输入输出   | 使用 `scanf` 和 `printf` 输入输出数据        |
| 查找最大值     | 找出成绩最高的学生                          |
| 排序算法       | 使用冒泡排序或其他简单排序算法               |

### 建议实现顺序：

1. 先实现结构体定义和一个学生的输入输出；
2. 改成循环输入多个学生；
3. 加入查找最高分逻辑；
4. 实现降序排序；
5. 整理程序结构，加入注释。