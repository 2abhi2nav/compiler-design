#include <stdio.h>
#include <string.h>

char table[10][10][10]; // 2D Table: table[NonTerm][Term] = Production
char nt_list[10], t_list[10];
int nt_cnt, t_cnt;

// Helper to find index of a character in a list
int find_idx(char *list, int n, char c) {
    for (int i = 0; i < n; i++) if (list[i] == c) return i;
    return -1;
}

int main() {
    // 1. Setup Symbols
    printf("Enter Non-Terminals (e.g., ET F): ");
    scanf("%s", nt_list);
    nt_cnt = strlen(nt_list);

    printf("Enter Terminals (e.g., id+*()$): ");
    scanf("%s", t_list);
    t_cnt = strlen(t_list);

    // 2. Construct Table (Input rules)
    // Initialize with empty strings
    for(int i=0; i<10; i++) for(int j=0; j<10; j++) strcpy(table[i][j], "");

    int n_rules;
    printf("Number of table entries: ");
    scanf("%d", &n_rules);

    printf("Enter entries (format: E + E+T): \n");
    for (int i = 0; i < n_rules; i++) {
        char nt, t, prod[10];
        scanf(" %c %c %s", &nt, &t, prod);
        table[find_idx(nt_list, nt_cnt, nt)][find_idx(t_list, t_cnt, t)] = strcpy(table[find_idx(nt_list, nt_cnt, nt)][find_idx(t_list, t_cnt, t)], prod);
    }

    // 3. Display Constructed Table
    printf("\nLL(1) Parsing Table:\n    ");
    for(int i=0; i<t_cnt; i++) printf("%-8c", t_list[i]);
    printf("\n--------------------------------------------------\n");
    for(int i=0; i<nt_cnt; i++) {
        printf("%c | ", nt_list[i]);
        for(int j=0; j<t_cnt; j++) printf("%-8s", table[i][j]);
        printf("\n");
    }

    // 4. Parsing Logic
    char stack[50] = "$", input[50];
    stack[1] = nt_list[0]; 
    int top = 1, ip = 0;

    printf("\nEnter input string: ");
    scanf("%s", input);

    printf("\n%-15s %-15s %-15s\n", "Stack", "Input", "Action");
    while (top >= 0) {
        printf("%-15s %-15s ", stack, input + ip);
        
        char X = stack[top];
        char a = input[ip];

        if (X == a) {
            if (X == '$') { printf("Accept!\n"); break; }
            printf("Match %c\n", a);
            top--; ip++;
        } else {
            int r_idx = find_idx(nt_list, nt_cnt, X);
            int c_idx = find_idx(t_list, t_cnt, a);
            
            if (r_idx != -1 && c_idx != -1 && strlen(table[r_idx][c_idx]) > 0) {
                char *p = table[r_idx][c_idx];
                printf("Apply %c->%s\n", X, p);
                top--;
                if (strcmp(p, "e") != 0) { // 'e' for epsilon
                    for (int k = strlen(p) - 1; k >= 0; k--) stack[++top] = p[k];
                }
            } else {
                printf("Error!\n"); break;
            }
        }
        stack[top + 1] = '\0';
    }

    return 0;
}