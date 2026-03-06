#include <stdio.h>
#include <string.h>
#include <stdlib.h>

#define MAX 50

struct Symbol {
    char name[20];
    char type[10];
} table[MAX];

int symbolCount = 0;

char* lookup(char *var) {
    for(int i = 0; i < symbolCount; i++) {
        if(strcmp(table[i].name, var) == 0)
            return table[i].type;
    }
    return NULL;
}

char* arithmeticResult(char *t1, char *t2) {

    if(strcmp(t1,"float")==0 || strcmp(t2,"float")==0)
        return "float";

    if(strcmp(t1,"int")==0 && strcmp(t2,"int")==0)
        return "int";

    if(strcmp(t1,"char")==0 && strcmp(t2,"char")==0)
        return "int";

    if((strcmp(t1,"char")==0 && strcmp(t2,"int")==0) ||
       (strcmp(t1,"int")==0 && strcmp(t2,"char")==0))
        return "int";

    return NULL;
}

int main() {

    int n;
    char type[10], var[20];
    char expr[50];


    scanf("%d",&n);

    for(int i=0;i<n;i++) {
        scanf("%s %s",type,var);
        strcpy(table[symbolCount].name,var);
        strcpy(table[symbolCount].type,type);
        symbolCount++;
    }

    scanf(" %[^\n]",expr);

    char lhs[20], op1[20], op2[20], operator;

    if(sscanf(expr,"%s = %s %c %s",lhs,op1,&operator,op2) != 4) {
        printf("Invalid expression format\n");
        return 0;
    }

    char *typeLHS = lookup(lhs);
    char *type1 = lookup(op1);
    char *type2 = lookup(op2);

    if(typeLHS == NULL || type1 == NULL || type2 == NULL) {
        printf("Undeclared Variable Error\n");
        return 0;
    }

    char *resultType = arithmeticResult(type1,type2);

    if(resultType == NULL) {
        printf("Type Mismatch Error in arithmetic operation\n");
        return 0;
    }

    if(strcmp(typeLHS,resultType)==0) {
        printf("Type Valid. Result Type: %s\n",resultType);
    }
    else if(strcmp(typeLHS,"float")==0 && strcmp(resultType,"int")==0) {
        printf("Type Valid. Result Type: float\n");
    }
    else if(strcmp(typeLHS,"int")==0 && strcmp(resultType,"float")==0) {
        printf("Type Mismatch Error: Cannot assign float to int\n");
    }
    else {
        printf("Type Mismatch Error\n");
    }

    return 0;
}
