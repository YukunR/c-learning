/*
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
*/

#include <stdio.h>
#include <string.h>

typedef struct {
    char name[20];
    int id;
    float score;
} Student;


int is_less(Student a, Student b);
void bubble_sort(Student arr[], int len);
Student input(int index);

int main() {
    int student_counts;
    printf("请输入学生人数 (最多 5): ");
    scanf("%d", &student_counts);
    // 判断是否大于 5
    if (student_counts > 5) {
        return 1;
    }

    Student students[student_counts];
    for (int i = 0; i < student_counts; i++) {
        students[i] = input(i + 1);
    }

    // 打印所有学生信息
    printf("\n--- 所有学生信息 ---\n");
    for (int i = 0; i < student_counts; i++) {
        printf("%s\t%d\t%.2f\n", students[i].name, students[i].id, students[i].score);
    }
    printf("\n--- 成绩最高的学生 ---\n");
    bubble_sort(students, student_counts);
    printf(
        "%s\t%d\t%.2f\n", 
        students[student_counts].name, 
        students[student_counts].id, 
        students[student_counts].score
    );
    printf("\n--- 成绩排序 (降序) ---\n");
    for (int i = 0; i < student_counts; i++) {
        printf("%s\t%d\t%.2f\n", students[i].name, students[i].id, students[i].score);
    }

    return 0;
}

int is_less(Student a, Student b) {
    return a.score < b.score;
}

void bubble_sort(Student arr[], int len) {
    int i, j;
    Student temp;
    for (i = 0; i < len - 1; i++) {
        for (j = 0; j < len - i - 1; j++) {
            if (is_less(arr[j], arr[j + 1])) {
                temp = arr[j + 1];
                arr[j + 1] = arr[j];
                arr[j] = temp;
            }
        }
    }
}

Student input(int index) {
    printf("请输入第 %d 个学生信息: \n", index);
    printf("姓名: ");
    char name[20];
    scanf("%s", name);
    printf("学号: ");
    int id;
    scanf("%d", &id);
    printf("成绩: ");
    float score;
    scanf("%f", &score);
    printf("\n");
    Student s;
    strcpy(s.name, name);
    s.id = id;
    s.score = score;
    return s;
}

