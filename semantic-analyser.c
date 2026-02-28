
#include <stdio.h>
#include <stdlib.h>
#include <ctype.h>
#include <string.h>

char *cursor;
int has_error = 0;

int parse_E();
int parse_T();
int parse_F();

void match(char expected) {
    if (*cursor == expected) {
        cursor++;
    } else {
        has_error = 1;
    }
}

int parse_E() {
    int val = parse_T();
    while (*cursor == '+') {
        match('+');
        if (has_error) return 0;
        val += parse_T();
    }
    return val;
}

int parse_T() {
    int val = parse_F();
    while (*cursor == '*') {
        match('*');
        if (has_error) return 0;
        val *= parse_F();
    }
    return val;
}

int parse_F() {
    int val = 0;
    if (*cursor == '(') {
        match('(');
        if (has_error) return 0;
        val = parse_E();
        match(')');
    } else if (isdigit(*cursor)) {
        while (isdigit(*cursor)) {
            val = val * 10 + (*cursor - '0');
            cursor++;
        }
    } else {
        has_error = 1;
    }
    return val;
}

int main() {
    char table_line[256];
    printf("Enter the Parsing Table (6 rules, one per line):\n");
    for (int i = 0; i < 6; i++) {
        if (!fgets(table_line, sizeof(table_line), stdin)) break;
    }

    char input[256];
    printf("\nEnter Valid Test Case String: ");
    if (fgets(input, sizeof(input), stdin)) {
        input[strcspn(input, "\n")] = 0;
        
        char clean_input[256];
        int j = 0;
        for (int i = 0; input[i] != '\0'; i++) {
            if (!isspace(input[i])) {
                clean_input[j++] = input[i];
            }
        }
        clean_input[j] = '\0';
        
        if (strlen(clean_input) == 0) return 0;

        cursor = clean_input;
        has_error = 0;
        
        int result = parse_E();
        
        if (has_error || *cursor != '\0') {
            printf("Syntax Error\n");
        } else {
            printf("Result: %d\n", result);
        }
    }
    return 0;
}
