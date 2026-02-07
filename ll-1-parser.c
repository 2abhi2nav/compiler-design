#include <stdio.h>
#include <string.h>
#include <stdlib.h>

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
            return i;
        }
    }
    return -1;
}

void print_arr(char *arr, int n) {
    printf("\n");
    for (int i = 0; i < n; i++) {
        printf("%c ", arr[i]);
    }
    printf("\n");
}

int main() {
    char waste_string[50];
    char waste_char;
    char *waste_ptr;
    char line[100];

    int nt;
    printf("Enter number of non-terminals: ");
    scanf("%d", &nt);

    fgets(waste_string, sizeof(waste_string), stdin);

    printf("\nEnter non-terminals: ");
    fgets(line, sizeof(line), stdin);

    char non_terminals[nt];

    waste_ptr = strtok(line, " ");
    non_terminals[0] = waste_ptr[0];

    for (int i = 1; i < nt; i++) {
        waste_ptr = strtok(NULL, " ");
        non_terminals[i] = waste_ptr[0];
    }

    int t;
    printf("Enter number of terminals: ");
    scanf("%d", &t);

    fgets(waste_string, sizeof(waste_string), stdin);

    printf("\nEnter terminals: ");
    fgets(line, sizeof(line), stdin);

    char terminals[t];

    waste_ptr = strtok(line, " ");
    terminals[0] = waste_ptr[0];

    for (int i = 1; i < t; i++) {
        waste_ptr = strtok(NULL, " ");
        terminals[i] = waste_ptr[0];
    }

    Derivation derivations[nt * t];
    int derivation_index = 0;

    printf("Enter LL(1) Parsing Table:\n");

    for (int i = 0; i < nt * t; i++) {
        char nonterm, term;
        char der[30];

        int count = scanf(" M[%c,%c] = %s", &nonterm, &term, der);

        if (count == 3) {
            derivations[derivation_index].nonterm = nonterm;
            derivations[derivation_index].term = term;
            strcpy(derivations[derivation_index].output, der);

            derivation_index++;
        }
        else {
            break;
        }
    }

    while (getchar() != '\n');

    char input[100];
    printf("\nEnter input string: ");
    fgets(input, sizeof(input), stdin);
    input[strcspn(input, "\n")] = 0;

    printf("\n%-10s %-10s %-10s\n", "Stack", "Input", "Action");
    printf("------------------------------\n");

    push('$');
    push(derivations[0].nonterm);

    int ip = 0;

    while (1) {
        stack[top + 1] = '\0';

        char X = stack[top];
        char a = input[ip];

        printf("%-10s %-10s ", stack, &input[ip]);

        if (X == a) {
            if (X == '$') {
                printf("String accepted\n");
                break;
            }
            printf("Match %c\n", a);
            pop();
            ip++;
        }
        else if (X != a && (X < 'A' || X > 'Z') && X != '$') {
            printf("\nTerminal mismatch\n");
            break;
        }
        else {
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
            }
            else {
                printf("No rule for M[%c,%c]\n", X, a);
                break;
            }
        }
    }

    return 0;
}
