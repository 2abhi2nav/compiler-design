#include <ctype.h>
#include <stdio.h>
#include <string.h>

void E(int *);
void Ec(int *);
void T(int *);
void Tc(int *);
void F(int *);

char *cursor;

void E(int *flag_p) {
	T(flag_p);
	Ec(flag_p);
}

void Ec(int *flag_p) {
	if (*cursor == '+' || *cursor == '-') {
		cursor++;
	} else {
		return;
	}

	T(flag_p);
	Ec(flag_p);
}

void T(int *flag_p) {
	F(flag_p);
	Tc(flag_p);
}

void Tc(int *flag_p) {
	if (*cursor == '*' || *cursor == '/') {
		cursor++;
	} else {
		return;
	}

	F(flag_p);
	Tc(flag_p);
}

void F(int *flag_p) {
	if (*cursor == '(') {
		cursor++;
		E(flag_p);

		if (*cursor == ')') {
			cursor++;
		} else {
			*flag_p = 0;
			return;
		}
	} else if (isalpha(*cursor) || isdigit(*cursor)) {
		cursor++;
	} else {
		*flag_p = 0;
		return;
	}
}

int main() {
	char input[120];
	fgets(input, sizeof(input), stdin);

	input[strcspn(input, "\n")] = 0;
	cursor = input;

	int flag = 1;
	E(&flag);

	if (*cursor == '\0' && flag == 1) {
		printf("Parsing successful\n");
	} else {
		printf("Parsing failed\n");
	}

	return 0;
}
