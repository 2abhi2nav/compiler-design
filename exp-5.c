
// recursive descent parser

#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <ctype.h>

// Prototypes (Keep E functions as they are)
void E(int *); void Ec(int *); void T(int *); void Tc(int *); void F(int *);
void statement(int *);
void end_block(int *);

char *cursor;

// Helper: Keyword Detection
int keyword(const char *word) {
    int len = strlen(word);
    if (strncmp(cursor, word, len) == 0) {
        cursor += len;
        return 1;
    }
    return 0;
}

// Nested Logic

void end_block(int *flag_p) {
    // Expect: ) { statements... }
    if (*cursor == ')' && *(++cursor) == '{') {
        cursor++;
        // Recursively parse statements inside the block until '}' is found
        while (*cursor != '}' && *cursor != '\0' && *flag_p == 1) {
            statement(flag_p);
        }
        if (*cursor == '}') cursor++;
        else *flag_p = 0;
    } else {
        *flag_p = 0;
    }
}

void if_statement(int *flag_p) {
    // Already consumed 'if' via keyword()
    if (*cursor == '(') {
        cursor++;
        E(flag_p);
        end_block(flag_p);
    } else {
        *flag_p = 0;
    }
}

void for_loop(int *flag_p) {
    // Already consumed 'for' via keyword()
    if (*cursor == '(') {
        cursor++;
        E(flag_p); // Init
        if (*cursor == ';') cursor++; else { *flag_p = 0; return; }
        E(flag_p); // Condition
        if (*cursor == ';') cursor++; else { *flag_p = 0; return; }
        E(flag_p); // Increment
        end_block(flag_p);
    } else {
        *flag_p = 0;
    }
}

void statement(int *flag_p) {
    if (keyword("if")) {
        if_statement(flag_p);
    } else if (keyword("for")) {
        for_loop(flag_p);
    } else {
        // Assume basic expression (e.g., a+b;)
        E(flag_p);
        if (*cursor == ';') cursor++;
    }
}

// Main Parser Entry
int main() {
    char input[100]; 
    printf("Enter code: ");
    fgets(input, sizeof(input), stdin);
    input[strcspn(input, "\n")] = 0;
    cursor = input;
    int flag = 1;

    // Process top-level statements
    while (*cursor != '\0' && flag == 1) {
        statement(&flag);
    }

    if (flag == 1 && *cursor == '\0') printf("Valid syntax\n");
    else printf("Invalid syntax\n");

    return 0;
}
