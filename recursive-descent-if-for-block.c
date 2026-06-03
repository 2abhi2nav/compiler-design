
#include <ctype.h>
#include <stdio.h>
#include <string.h>

char *cursor;

void E();
void T();
void F();

void match(char c, int *f) {
	if (*cursor == c)
		cursor++;
	else
		*f = 0;
}

void Ec(int *f) {
	if (*f && (*cursor == '+' || *cursor == '-')) {
		cursor++;
		T(f);
		Ec(f);
	}
}

void E(int *f) {
	if (*f) {
		T(f);
		Ec(f);
	}
}

void Tc(int *f) {
	if (*f && (*cursor == '*' || *cursor == '/')) {
		cursor++;
		F(f);
		Tc(f);
	}
}

void T(int *f) {
	if (*f) {
		F(f);
		Tc(f);
	}
}

void F(int *f) {
	if (!*f)
		return;
	if (*cursor == '(') {
		cursor++;
		E(f);
		match(')', f);
	} else if (isalnum(*cursor)) {
		cursor++;
	} else {
		*f = 0;
	}
}

int main() {
	char input[100];
	fgets(input, sizeof(input), stdin);
	input[strcspn(input, "\n")] = 0;
	cursor = input;
	int flag = 1;

	if (strncmp(cursor, "for(", 4) == 0) {
		cursor += 4;
		E(&flag);
		match(';', &flag);
		E(&flag);
		match(';', &flag);
		E(&flag);
		match(')', &flag);
		if (strncmp(cursor, "{}", 2) == 0)
			cursor += 2;
		else
			flag = 0;
	} else {
		flag = 0;
	}

	if (flag && *cursor == '\0')
		printf("Valid for-loop syntax\n");
	else
		printf("Invalid for-loop syntax\n");

	return 0;
}
