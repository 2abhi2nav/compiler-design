
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <ctype.h>

char *cursor;
int flag = 1;

void E();

void F() {
	if (*cursor == '(') {
		cursor++;
		E();
		if (*cursor != ')') {
			flag = 0;
			return;
		} else {
			cursor++;
		}
	} else if (isalpha(*cursor) || isdigit(*cursor)) {
		cursor++;
	} else {
		flag = 0;
		return;
	}
}

int main() {
	FILE *fptr = fopen("file.c", "r");
	char line[100];

	while (fgets(line, sizeof(line), fptr)) {
		line[strcspn(line, "\n")] = 0;
		
		char *test = "a";
		int testi = atoi(test);
	}

	fclose(fptr);

	return 0;
}
