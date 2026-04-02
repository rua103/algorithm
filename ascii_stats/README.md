# 实验 12_1：初识 ASCII 码文件（北邮样例）

从当前目录读取 `in.txt`，写入 `out.txt`。

```bash
gcc -std=c11 -Wall -Wextra -O2 -o ascii_stats ascii_stats.c
./ascii_stats
```

## 输出格式（与题目「呼出」样例一致）

```
Task1:
CAPITAL:
A:…
…
Z:…
LOWERCASE:
a:…
…
z:…
```

## 题目要求 1、2（四类字符数、行数与行长）

程序**已在读文件时完成统计**（变量 `cat_*`、`lines`、`max_line`、`min_line`）。官方给出的「呼出」样例**未包含**这几项的文本行；若任课教师或 OJ 要求一并写入 `out.txt`，在 `fclose(fout);` 之前自行添加对应的 `fprintf` 即可（并删去仅用于消除警告的 `(void)cat_upper;` 等语句）。

## 行结束符

Windows 下 `in.txt` 可能为 `\r\n`：`\r`（ASCII 13）计入第四类，且算入行长度；换行符 `\n`（ASCII 10）不计入行长度，但计入第四类。评测机一般为 Unix 换行，仅含 `\n`。
