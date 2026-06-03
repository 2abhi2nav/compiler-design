
#include <stdio.h>
#include <string.h>

char expr[100];
int pos = 0, t_count = 1;

void newtemp(char *);
void expression(char *);
void term(char *);
void factor(char *);

void newtemp(char *t) { sprintf(t, "t%d", t_count++); }

void expression(char *place) {
	char p1[10], p2[10], t[10];
	term(p1);
	while (expr[pos] == '+' || expr[pos] == '-') {
		char op = expr[pos++];
		term(p2);
		newtemp(t);
		printf("%s = %s %c %s\n", t, p1, op, p2);
		strcpy(p1, t);
	}
	strcpy(place, p1);
}

void term(char *place) {
	char p1[10], p2[10], t[10];
	factor(p1);
	while (expr[pos] == '*' || expr[pos] == '/') {
		char op = expr[pos++];
		factor(p2);
		newtemp(t);
		printf("%s = %s %c %s\n", t, p1, op, p2);
		strcpy(p1, t);
	}
	strcpy(place, p1);
}

void factor(char *place) {
	if (expr[pos] == '(') {
		pos++;
		void expression(char *);
		expression(place);
		pos++;
	} else {
		place[0] = expr[pos++];
		place[1] = '\0';
	}
}

int main() {
	char result[10];
	scanf("%s", expr);
	expression(result);
	return 0;
}
