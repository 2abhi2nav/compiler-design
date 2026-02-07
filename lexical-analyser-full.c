#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <ctype.h>

typedef struct {
    int id;
    char type[30];
    char name[30]; 
    int value;
} Record;


const char *keywords[] = {"int", "float", "main", "if", "for", "while", "return"};
int key_count = 7;

int is_keyword(char *token) {
    for (int i = 0; i < key_count; i++) {
        if (strcmp(token, keywords[i]) == 0) return 1;
    }
    return 0;
}

int is_arithmetic(char *token) {
    char op_table[4][2] = {"+", "-", "*", "/"};
    for (int j = 0; j < 4; j++) {
        if (strcmp(token, op_table[j]) == 0) return 1;
    }
    return 0;
}


int is_relational(char *token) {
    char rel_table[6][3] = {">", "<", ">=", "<=", "==", "!="};
    for (int i = 0; i < 6; i++) {
        if (strcmp(token, rel_table[i]) == 0) return 1;
    }
    return 0;
}

int is_number(char *token) {
    if (*token == '\0') return 0;
    for (int i = 0; token[i] != '\0'; i++) {
        if (!isdigit(token[i])) return 0;
    }
    return 1;
}


int is_identifier(char *token) {
    if (is_keyword(token)) return 0;
    if (!isalpha(token[0]) && token[0] != '_') return 0;
    for (int i = 1; token[i] != '\0'; i++) {
        if (!isalnum(token[i]) && token[i] != '_') return 0;
    }
    return 1;
}

void print_counts(int *counts) {
    printf("\n\nToken counts:\n");
    printf("Arithmetic operators: %d\n", counts[0]);
    printf("Relational operators: %d\n", counts[1]);
    printf("Constants: %d\n", counts[2]);
    printf("Identifiers: %d\n", counts[3]);
}

void print_table(Record *table, int n) {
    printf("\nSymbol table:\n");
    printf("ID, NAME, TYPE, VALUE\n");
    for (int i = 0; i < n; i++) {
        printf("%d, %s, %s, %d\n", table[i].id, table[i].name, table[i].type, table[i].value);
    }
}

int main() {
    int op_counts[4] = {0, 0, 0, 0};
    Record symbol_table[50];
    int token_index = 0;

    FILE *fptr = fopen("code.c", "r"); 
    char line[100];

    if (fptr == NULL) {
        printf("Error: Could not open code.c\n");
        return 1;
    }

    printf("Operator tokens:\n");

    while (fgets(line, sizeof(line), fptr)) {
        
        line[strcspn(line, "\n")] = 0;
        
        char *token = strtok(line, " ;=(){},"); 

        while (token != NULL) {
            Record temp;
            temp.id = token_index + 1;
            temp.value = 0;
            strcpy(temp.name, token);

            if (is_arithmetic(token)) {
                printf("<%s, arithmetic_op> ", token); 
                op_counts[0]++;
                strcpy(temp.type, "arithmetic");
                symbol_table[token_index++] = temp;
            } 
            else if (is_relational(token)) {
                printf("<%s, relational_op> ", token); 
                op_counts[1]++;
                strcpy(temp.type, "relational");
                symbol_table[token_index++] = temp;
            } 
            else if (is_number(token)) {
                op_counts[2]++;
                strcpy(temp.type, "constant");
                temp.value = atoi(token);
                symbol_table[token_index++] = temp;
            } 
            else if (is_identifier(token)) {
                op_counts[3]++;
                strcpy(temp.type, "identifier");
                symbol_table[token_index++] = temp;
            }

            token = strtok(NULL, " ;=(){},");
        }
    }

    fclose(fptr);
    print_counts(op_counts);
    print_table(symbol_table, token_index);

    return 0;
}