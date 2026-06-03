#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <ctype.h>

int table[12][9];

int r_lhs[] = {0, 6, 6, 7, 7, 8, 8}; 
int r_len[] = {0, 3, 1, 3, 1, 3, 1}; 

int get_token(char **s, int *val) {
    while (isspace(**s)) (*s)++;
    if (!**s) return 5; 
    
    if (isdigit(**s)) {
        *val = 0;
        while (isdigit(**s)) *val = *val * 10 + (*((*s)++) - '0');
        return 0; 
    }
    
    char c = *((*s)++);
    return (c == '+') ? 1 : (c == '*') ? 2 : (c == '(') ? 3 : (c == ')') ? 4 : 999;
}

int main() {
    char cell[10];
    printf("Enter parsing table, using _ for empty cells):\n");
    
    
    for (int i = 0; i < 12; i++) {
        for (int j = 0; j < 9; j++) {
            scanf("%s", cell);
            if (strcmp(cell, "acc") == 0) table[i][j] = 100;
            else if (cell[0] == 's') table[i][j] = atoi(&cell[1]);
            else if (cell[0] == 'r') table[i][j] = -atoi(&cell[1]);
            else if (isdigit(cell[0])) table[i][j] = atoi(cell);
            else table[i][j] = 999; 
        }
    }

    while (getchar() != '\n'); 

    char buf[256];
    printf("\nEnter input string: ");
    if (!fgets(buf, sizeof(buf), stdin)) return 0;
    buf[strcspn(buf, "\n")] = 0; 

    
    char *cursor = buf;
    int states[100], vals[100];
    int top = 0;
    states[0] = 0;

    int lex_val;
    int tok = get_token(&cursor, &lex_val);

    
    while (1) {
        int action = table[states[top]][tok];

        if (action == 100) { 
            printf("Result: %d\n", vals[top]);
            break;
        } else if (action == 999) { 
            printf("Syntax Error\n");
            break;
        } else if (action >= 0) { 
            states[++top] = action;
            vals[top] = lex_val;
            tok = get_token(&cursor, &lex_val);
        } else { 
            int r = -action;
            int new_val = 0;
            
            if (r == 1) new_val = vals[top-2] + vals[top];      
            if (r == 2 || r == 4 || r == 6) new_val = vals[top];
            if (r == 3) new_val = vals[top-2] * vals[top];      
            if (r == 5) new_val = vals[top-1];                  

            top -= r_len[r]; 
            states[top+1] = table[states[top]][r_lhs[r]]; 
            vals[++top] = new_val; 
        }
    }
    return 0;
}