#include <stdio.h>
#include <stdlib.h>
#include <limits.h>

int main(void) {
    FILE *fin = fopen("in.txt", "rb");
    if (!fin) {
        perror("in.txt");
        return 1;
    }

    long cat_upper = 0, cat_lower = 0, cat_digit = 0, cat_other = 0;
    int upper_cnt[26] = {0};
    int lower_cnt[26] = {0};

    long lines = 0;
    int line_len = 0;
    int max_line = 0;
    int min_line = INT_MAX;
    int saw_any_line = 0;

    int c;
    while ((c = fgetc(fin)) != EOF) {
        unsigned char uc = (unsigned char)c;

        if (uc == '\n') {
            lines++;
            if (!saw_any_line) {
                min_line = line_len;
                saw_any_line = 1;
            } else if (line_len < min_line) {
                min_line = line_len;
            }
            if (line_len > max_line) {
                max_line = line_len;
            }
            line_len = 0;
            cat_other++;
            continue;
        }

        line_len++;

        if (uc >= 'A' && uc <= 'Z') {
            cat_upper++;
            upper_cnt[uc - 'A']++;
        } else if (uc >= 'a' && uc <= 'z') {
            cat_lower++;
            lower_cnt[uc - 'a']++;
        } else if (uc >= '0' && uc <= '9') {
            cat_digit++;
        } else {
            cat_other++;
        }
    }

    if (!saw_any_line && lines == 0) {
        min_line = 0;
    }

    fclose(fin);

    FILE *fout = fopen("out.txt", "wb");
    if (!fout) {
        perror("out.txt");
        return 1;
    }

    fprintf(fout, "Task1:\n");
    fprintf(fout, "CAPITAL:\n");
    for (int i = 0; i < 26; i++) {
        fprintf(fout, "%c:%d\n", 'A' + i, upper_cnt[i]);
    }
    fprintf(fout, "LOWERCASE:\n");
    for (int i = 0; i < 26; i++) {
        fprintf(fout, "%c:%d\n", 'a' + i, lower_cnt[i]);
    }

    (void)cat_upper;
    (void)cat_lower;
    (void)cat_digit;
    (void)cat_other;
    (void)lines;
    (void)max_line;
    (void)min_line;

    fclose(fout);
    return 0;
}
