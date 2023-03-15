#define _CRT_SECURE_NO_WARNINGS
#include <stdio.h>
#include <stdlib.h>
#include <string.h>

int print_bytes = 0;
int print_lines = 0;
int print_words = 0;

void print_help(const char* program_name) {
    printf("Usage: %s [-C] [-L] [-W] [file1 file2 ...]\n", program_name);
    printf("Options:\n");
    printf("  -C   Print byte count\n");
    printf("  -L   Print line count\n");
    printf("  -W   Print word count\n");
    printf("  -?   Show this help message\n");
}
void print_out(num_bytes, num_lines, num_words, name) {
    printf("%s:\t", name);
    if (print_bytes) {
        printf("\tbyte count-%d\t", num_bytes);
    }
    if (print_lines) {
        printf("\tline count-%d\t", num_lines);
    }
    if (print_words) {
        printf("\tword count-%d\t", num_words);
    }
    printf("\n");
}

int main(int argc, char* argv[]) {
    int num_files = 0;
    FILE* file;
    char line[1024];
    int sum_bytes = 0;
    int sum_lines = 0;
    int sum_words = 0;

    if (argc == 1) {
        print_help(argv[0]);
        return 0;
    }
    for (int i = 1; i < argc; i++) {
        if (strcmp(argv[i], "-C") == 0) {
            print_bytes = 1;
        }
        else if (strcmp(argv[i], "-L") == 0) {
            print_lines = 1;
        }
        else if (strcmp(argv[i], "-W") == 0) {
            print_words = 1;
        }
        else if (strcmp(argv[i], "-?") == 0) {
            print_help(argv[0]);
            return 0;
        }
        else {
            num_files++;
        }
    }
    for (int j = print_lines + print_words + print_bytes+1; j < argc; j++) {
        int num_bytes = 0;
        int num_lines = 0;
        int num_words = 0;
        if (strcmp(argv[1], "-") == 0 || num_files == 0) {
            file = stdin;
        }
        else {
            file = fopen(argv[j], "r");
        }
        while (fgets(line, sizeof(line), file)) {
            num_bytes += strlen(line);
            num_lines++;
            for (int i = 0; line[i] != '\0'; i++) {
                if (line[i] == ' ' || line[i] == '\t' || line[i] == '\n') {
                    num_words++;
                }
            }
        }
        sum_bytes += num_bytes;
        sum_lines += num_lines;
        sum_words += num_words;
        print_out(num_bytes, num_lines, num_words, argv[j]);
    }
    print_out(sum_bytes, sum_lines, sum_words, "Total count");

    return 0;
}