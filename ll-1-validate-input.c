#include <stdio.h>
#include <string.h>
#include <ctype.h>

typedef struct {
    char nonterm;
    char term;
    char output[30];
} Derivation;

char stack[100];
int top = -1;

void push(char c) {
    stack[++top] = c;
}

char pop() {
    if (top == -1) return '\0';
    return stack[top--];
}

int get_rule_index(Derivation ders[], int der_count, char non_term, char term) {
    for (int i = 0; i < der_count; i++) {
        if (ders[i].nonterm == non_term && ders[i].term == term) {
            
            if (strcmp(ders[i].output, "-") == 0) return -1;
            return i;
        }
    }
    return -1;
}

int main() {
    char line[100];
    int nt, t;

    printf("Enter number of non-terminals: ");
    scanf("%d", &nt);
    getchar(); 

    printf("Enter non-terminals: ");
    fgets(line, sizeof(line), stdin);
    
    printf("Enter number of terminals: ");
    scanf("%d", &t);
    getchar();

    printf("Enter terminals: ");
    fgets(line, sizeof(line), stdin);

    Derivation derivations[nt * t];
    int derivation_index = 0;

    printf("Enter LL(1) Parsing Table:\n");
    for (int i = 0; i < nt * t; i++) {
        char nt_val, t_val, der[30];
        
        if (scanf(" M[%c,%c] = %s", &nt_val, &t_val, der) == 3) {
            derivations[derivation_index].nonterm = nt_val;
            derivations[derivation_index].term = t_val;
            strcpy(derivations[derivation_index].output, der);
            derivation_index++;
        }
    }
    getchar(); 

    char input[100];
    printf("\nEnter input string: ");
    fgets(input, sizeof(input), stdin);
    input[strcspn(input, "\n")] = 0; 

    printf("\n%-20s %-20s %-20s\n", "Stack", "Input", "Action");
    printf("------------------------------------------------------------\n");

    push('$');
    
    push(derivations[0].nonterm);

    int ip = 0;
    while (1) {
        stack[top + 1] = '\0';
        char X = stack[top];
        char a = input[ip];

        printf("%-20s %-20s ", stack, &input[ip]);

        if (X == a) {
            if (X == '$') {
                printf("Match $\n");
                printf("\nString Accepted\n");
                break;
            }
            printf("Match %c\n", a);
            pop();
            ip++;
        } 
        else if (isupper(X)) { 
            int r = get_rule_index(derivations, derivation_index, X, a);
            if (r != -1) {
                printf("Apply %c -> %s\n", X, derivations[r].output);
                pop();
                char *prod = derivations[r].output;
                if (strcmp(prod, "e") != 0) { 
                    for (int k = strlen(prod) - 1; k >= 0; k--) {
                        push(prod[k]);
                    }
                }
            } else {
                printf("Error: No rule for M[%c,%c]\n", X, a);
                break;
            }
        } 
        else {
            printf("Terminal mismatch\n");
            break;
        }
    }

    return 0;
}