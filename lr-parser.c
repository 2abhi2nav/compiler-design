

#include <stdio.h>
#include <string.h>

// Grammar Rule: LHS -> RHS
typedef struct {
    char lhs;
    char rhs[10]; // e.g., "E+T" or "id"
} Rule;

// LR(0) Item: rule_index + dot_position
typedef struct {
    int rule_index;
    int dot;
} Item;

// State: A collection of Items
typedef struct {
    int id;
    int item_count;
    Item items[100];
} State;

// --- Global Variables ---

Rule grammar[10];
int rule_count = 0;

State states[50];
int state_count = 0;

// Action Table: [State][Terminal] -> "s5", "r2", "acc"
char action_table[50][20][10];
// Goto Table: [State][NonTerminal] -> State Index
int goto_table[50][20];

// Terminals and Non-Terminals
char terminals[20][5] = {"id", "+", "*", "(", ")", "$"};
char non_terminals[20] = {'E', 'T', 'F'};
int term_count = 6;
int non_term_count = 3;

// --- Helper Functions ---

// Check if an item exists in a state
int item_exists(State *s, int rule_idx, int dot) {
    for (int i = 0; i < s->item_count; i++) {
        if (s->items[i].rule_index == rule_idx && s->items[i].dot == dot)
            return 1;
    }
    return 0;
}

// Add an item to a state
void add_item(State *s, int rule_idx, int dot) {
    if (!item_exists(s, rule_idx, dot)) {
        s->items[s->item_count].rule_index = rule_idx;
        s->items[s->item_count].dot = dot;
        s->item_count++;
    }
}

// Helper to get symbol at dot position
// Returns single char or special 'i' for "id"
char get_symbol_at_dot(int rule_idx, int dot) {
    char *rhs = grammar[rule_idx].rhs;
    if (dot >= strlen(rhs)) return '\0';
    
    // Special handling for 'id' which is 2 chars but treated as 1 unit
    if (rhs[dot] == 'i' && rhs[dot+1] == 'd') return 'i'; 
    return rhs[dot];
}

// The core CLOSURE operation
void compute_closure(State *s) {
    int changed = 1;
    while (changed) {
        changed = 0;
        int current_count = s->item_count;
        for (int i = 0; i < current_count; i++) {
            char symbol = get_symbol_at_dot(s->items[i].rule_index, s->items[i].dot);
            
            // If symbol is a non-terminal, add its productions
            // (Check if symbol is E, T, or F)
            if (symbol >= 'A' && symbol <= 'Z') {
                for (int r = 1; r < rule_count; r++) { // Start at 1 to skip Augmented E'
                    if (grammar[r].lhs == symbol) {
                        if (!item_exists(s, r, 0)) {
                            add_item(s, r, 0);
                            changed = 1;
                        }
                    }
                }
            }
        }
    }
}

// The core GOTO operation
int compute_goto(int state_idx, char symbol) {
    State temp_state;
    temp_state.item_count = 0;
    State *src = &states[state_idx];

    for (int i = 0; i < src->item_count; i++) {
        int r = src->items[i].rule_index;
        int d = src->items[i].dot;
        char sym = get_symbol_at_dot(r, d);

        if (sym == symbol) {
            // Calculate increment. If 'id' (mapped to 'i'), skip 2 chars. Else 1.
            int advance = (sym == 'i') ? 2 : 1;
            add_item(&temp_state, r, d + advance);
        }
    }

    if (temp_state.item_count == 0) return -1;

    compute_closure(&temp_state);

    // Check if this state already exists
    for (int i = 0; i < state_count; i++) {
        if (states[i].item_count == temp_state.item_count) {
            int match = 1;
            for (int k = 0; k < temp_state.item_count; k++) {
                if (!item_exists(&states[i], temp_state.items[k].rule_index, temp_state.items[k].dot)) {
                    match = 0;
                    break;
                }
            }
            if (match) return i;
        }
    }

    // Add new state
    temp_state.id = state_count;
    states[state_count] = temp_state;
    state_count++;
    return state_count - 1;
}

// Hardcoded FOLLOW sets for this specific grammar
// FOLLOW(E) = {$, +, )}
// FOLLOW(T) = {$, +, ), *}
// FOLLOW(F) = {$, +, ), *}
int is_in_follow(char non_term, char *terminal) {
    if (non_term == 'E') {
        if (strcmp(terminal, "$") == 0 || strcmp(terminal, "+") == 0 || strcmp(terminal, ")") == 0) return 1;
    }
    else if (non_term == 'T' || non_term == 'F') {
        if (strcmp(terminal, "$") == 0 || strcmp(terminal, "+") == 0 || strcmp(terminal, ")") == 0 || strcmp(terminal, "*") == 0) return 1;
    }
    return 0;
}

int get_term_index(char *t) {
    for(int i=0; i<term_count; i++) if(strcmp(terminals[i], t)==0) return i;
    return -1;
}
int get_nonterm_index(char t) {
    for(int i=0; i<non_term_count; i++) if(non_terminals[i] == t) return i;
    return -1;
}

int main() {
    // 1. Augment Grammar
    // Rule 0 is E' -> E
    grammar[0].lhs = 'Z'; strcpy(grammar[0].rhs, "E"); 
    grammar[1].lhs = 'E'; strcpy(grammar[1].rhs, "E+T");
    grammar[2].lhs = 'E'; strcpy(grammar[2].rhs, "T");
    grammar[3].lhs = 'T'; strcpy(grammar[3].rhs, "T*F");
    grammar[4].lhs = 'T'; strcpy(grammar[4].rhs, "F");
    grammar[5].lhs = 'F'; strcpy(grammar[5].rhs, "(E)");
    grammar[6].lhs = 'F'; strcpy(grammar[6].rhs, "id");
    rule_count = 7;

    // Initialize tables
    for(int i=0; i<50; i++) {
        for(int j=0; j<20; j++) {
            action_table[i][j][0] = '\0';
            goto_table[i][j] = -1;
        }
    }

    // 2. Construct LR(0) Collection
    // Initial State I0
    states[0].id = 0;
    states[0].item_count = 0;
    add_item(&states[0], 0, 0); // Add E' -> .E
    compute_closure(&states[0]);
    state_count = 1;

    // Generate other states (BFS)
    for (int i = 0; i < state_count; i++) {
        // Try shifting over every possible symbol
        // Terminals
        for (int j = 0; j < term_count; j++) {
            char sym = (strcmp(terminals[j], "id") == 0) ? 'i' : terminals[j][0];
            if (sym == '$') continue;
            
            int next_state = compute_goto(i, sym);
            if (next_state != -1) {
                sprintf(action_table[i][j], "s%d", next_state);
            }
        }
        // Non-Terminals
        for (int j = 0; j < non_term_count; j++) {
            int next_state = compute_goto(i, non_terminals[j]);
            if (next_state != -1) {
                goto_table[i][j] = next_state;
            }
        }
    }

    // 3. Add REDUCE actions based on FOLLOW sets
    for (int i = 0; i < state_count; i++) {
        for (int k = 0; k < states[i].item_count; k++) {
            int r = states[i].items[k].rule_index;
            int d = states[i].items[k].dot;
            int len = strlen(grammar[r].rhs);

            if (d == len) { // Dot at end (A -> alpha.)
                if (r == 0) {
                    // Accept state: Z -> E.
                    sprintf(action_table[i][get_term_index("$")], "acc");
                } else {
                    // Reduce: For all 'a' in FOLLOW(A)
                    char lhs = grammar[r].lhs;
                    for (int t = 0; t < term_count; t++) {
                        if (is_in_follow(lhs, terminals[t])) {
                            // If conflict exists (Shift-Reduce), prefer Shift (simple precedence rule) 
                            // or just overwrite for this simple grammar
                            if (action_table[i][t][0] == '\0') {
                                sprintf(action_table[i][t], "r%d", r);
                            }
                        }
                    }
                }
            }
        }
    }

    // 4. Display Table
    printf("\nSLR Parsing Table:\n");
    printf("------------------------------------------------------------------\n");
    printf("| State |  id  |  +   |  * |  (   |  )   |  $   |  E   |  T   |  F   |\n");
    printf("------------------------------------------------------------------\n");

    for (int i = 0; i < state_count; i++) {
        printf("|  %2d   |", i);
        // Action Part
        for (int j = 0; j < term_count; j++) {
            printf(" %-4s |", action_table[i][j]);
        }
        // Goto Part
        for (int j = 0; j < non_term_count; j++) {
            if (goto_table[i][j] != -1)
                printf("  %2d  |", goto_table[i][j]);
            else
                printf("      |");
        }
        printf("\n");
    }
    printf("------------------------------------------------------------------\n");

    return 0;
}
