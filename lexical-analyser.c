#include <stdio.h>
#include <string.h>
#include <ctype.h>

int main() {
    
    char key_arr[6][30] = {"main", "for", "while", "int", "float", "if"};
    char op_arr[6][4] = {"+", "-", "*", "/", "||", "&&"};
    
    char str_arr[5][30];
    for (int i = 0; i < 5; i++) {
        scanf("%s", str_arr[i]);
    }
    
    for (int i = 0; i < 5; i++) {
        char string[30];
        strcpy(string, str_arr[i]);
        
        // keyword checking
        int key_flag = 0;
        for (int j = 0; j < 6; j++) {
            if (strcmp(string, key_arr[j]) == 0) {
                key_flag = 1;
            }
        }
        
        // number
        int num_flag = 0;
        for (int j = 0; string[j] != '\0'; j++) {
            char c = string[j];

            if (!isdigit(c)) {
                num_flag = 0;
                break;
            }
            
            num_flag = 1;
        }
        
        //operator
        int op_flag = 0;
        for (int j = 0; j < 6; j++) {
            if (strcmp(string, op_arr[j]) == 0) {
                op_flag = 1;
            }
        }
        
        // attribute
        int attr_flag = 0;
        for (int j = 0; string[j] != '\0'; j++) {
            char c = string[j];
            
            if (j == 0 && isdigit(c)) {
                break;
            }
            if (!isalpha(c) && !isdigit(c) && c != '_') {
                break;
            }
            
            attr_flag = 1;
        }

        // special
        int spec_flag = 0;
        char c = string[0];
        if (c == '{' || c == '}' || c == ';' || c == '('
            || c == ')' || c == '[' || c == ']') {
            spec_flag = 1;
        }

        // print
        if (key_flag == 1) {
            printf("<%s, keyword> ", string);
        } else if (num_flag == 1) {
            printf("<%s, digit> ", string);
        } else if (op_flag == 1) {
            printf("<%s, operator> ", string);
        } else if (attr_flag == 1) {
            printf("<%s, identifier> ", string);
        } else if (spec_flag == 1) {
            printf("<%s, special> ", string);
        } else {
            printf("<%s, invalid> ", string);
        }
        
    }
    
    return 0;
}
