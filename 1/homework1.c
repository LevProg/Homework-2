#define _CRT_SECURE_NO_WARNINGS
#include <stdio.h>
#include <stdlib.h>
#include <string.h>

short param_c = 0;
short param_l = 0;
short param_w = 0;

void print_help(const char* program_name) {
    printf("Usage: %s [-C] [-L] [-W] [file1 file2 ...]\n", program_name);
    printf("Options:\n");
    printf("  -C   Print byte count\n");
    printf("  -L   Print line count\n");
    printf("  -W   Print word count\n");
    printf("  -?   Show this help message\n");
    return 0;
}
void print_out(num_bytes, num_lines, num_words, name) {
    printf("%s:\t", name);
    if (param_c) printf("\tbyte count-%d\t", num_bytes);
    if (param_l) printf("\tline count-%d\t", num_lines);
    if (param_w) printf("\tword count-%d\t", num_words);
    printf("\n");
}
int main(int argc, char* argv[]) {
    int num_files = 0;
    FILE* file;
    char line[1024];
    int sum_bytes = 0;
    int sum_lines = 0;
    int sum_words = 0;
    for (int i = 1; i < argc; i++) {
        if (strcmp(argv[i], "-C") == 0) param_c = 1;
        else if (strcmp(argv[i], "-L") == 0) param_l = 1;
        else if (strcmp(argv[i], "-W") == 0) param_w = 1;
        else if (strcmp(argv[i], "-?") == 0) print_help(argv[0]);
        else num_files++;
    }
    if (param_c+param_l+param_w==0) print_help(argv[0]);
    for (int j= param_c + param_l + param_w +1; j <= argc; j++) {
        int num_bytes = 0;
        int num_lines = 0;
        int num_words = 0;
        if (num_files != 0 && j == argc) break;
        if (num_files == 0 || strcmp(argv[j], "-") == 0) file = stdin;
        else file = fopen(argv[j], "r");
        while (fgets(line, sizeof(line), file)) {
            num_bytes += strlen(line);
            num_lines++;
            if (strlen(line) == 1) {
                num_lines--;
                break;
            }
            for (int i = 0; line[i] != '\0'; i++) {
                if (line[i] == ' ' || line[i] == '\t' || line[i] == '\n') {
                    num_words++;
                }
            }
        }
        sum_bytes += num_bytes;
        sum_lines += num_lines;
        sum_words += num_words;
        if (file == stdin) print_out(num_bytes, num_lines, num_words, "stdin");
        else print_out(num_bytes, num_lines, num_words, argv[j]);
    }
    if (num_files > 1) print_out(sum_bytes, sum_lines, sum_words, "Total count");
    return 0;
}