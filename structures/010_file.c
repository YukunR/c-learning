/*
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

- 所有字符串使用 `char name[20];`；
- 不需要从命令行输入文件名；
- 每行最大不超过 100 个字符。

### 拓展建议（可选）

- 将学生信息写入另一个文件（如 `report.txt`）；
*/

#include <stdio.h>
#include <string.h>

typedef struct
{
    char name[100];
    float score;
} Student;

void read_file(FILE *fp, Student arr[], int *students_count);
float calculate_average_score(Student arr[], int count);

int main()
{
    char FILEPATH[] = "./010_score.txt";
    FILE *fp = fopen(FILEPATH, "r");

    Student students[100];
    int count;

    read_file(fp, students, &count);
    float average_score = calculate_average_score(students, count);

    // 输出结果
    printf("\n共有 %d 位学生, 平均成绩位 %.2f", count, average_score);
    return 0;
}

void read_file(FILE *fp, Student arr[], int *students_count)
{
    void read_file(FILE * fp, Student arr[], int *students_count);
    if (fp == NULL)
    {
        printf("读取失败\n");
    }
    else
    {
        int count = 0;
        char buffer[100];
        printf("读取学生信息：\n");
        while (fgets(buffer, sizeof(buffer), fp))
        {

            Student s;
            char name[20];
            float score;
            sscanf(buffer, "%s %f", name, &score);
            printf("%s 的成绩是 %.2f\n", name, score);

            strcpy(s.name, name);
            s.score = score;

            arr[count] = s;
            count += 1;
        }

        *students_count = count;
    }
}

float calculate_average_score(Student arr[], int count)
{
    float sum;
    for (int i = 0; i < count; i++)
    {
        sum += arr[i].score;
    }

    return sum / count;
}