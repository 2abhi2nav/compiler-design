
#include <stdio.h>
#include <stdlib.h>
#include <ctype.h>
#include <string.h>

char r[100][20], a[100][20], o[100], b[100][20];
int n = 0;

int is_num(char *s) { return isdigit(s[0]) || (s[0] == '-' && isdigit(s[1])); }

int main() {
	char buf[100];

	while (fgets(buf, 100, stdin) && strncmp(buf, "END", 3)) {
		buf[strcspn(buf, "\r\n")] = 0;
		o[n] = b[n][0] = 0;
		if (sscanf(buf, "%s = %s %c %s", r[n], a[n], &o[n], b[n]) >= 2)
			n++;
	}

	int changed = 1;
	while (changed) {
		changed = 0;
		for (int i = 0; i < n; i++) {
			if (!r[i][0])
				continue;

			if (o[i] == '+' && !strcmp(b[i], "0")) {
				o[i] = 0;
				changed = 1;
			}
			if (o[i] == '*' && !strcmp(b[i], "1")) {
				o[i] = 0;
				changed = 1;
			}

			if (o[i] && is_num(a[i]) && is_num(b[i])) {
				int x = atoi(a[i]), y = atoi(b[i]), v = 0;
				if (o[i] == '+')
					v = x + y;
				if (o[i] == '-')
					v = x - y;
				if (o[i] == '*')
					v = x * y;
				if (o[i] == '/') {
					if (!y)
						continue;
					v = x / y;
				}
				sprintf(a[i], "%d", v);
				o[i] = 0;
				changed = 1;
			}

			if (o[i]) {
				for (int j = 0; j < i; j++) {

					if (r[j][0] && o[j] == o[i] && !strcmp(a[i], a[j]) && !strcmp(b[i], b[j])) {
						strcpy(a[i], r[j]);
						o[i] = 0;
						changed = 1;
						break;
					}
				}
			}

			if (!o[i]) {
				for (int j = i + 1; j < n; j++) {
					if (!r[j][0])
						continue;
					if (!strcmp(r[i], a[j])) {
						strcpy(a[j], a[i]);
						changed = 1;
					}
					if (o[j] && !strcmp(r[i], b[j])) {
						strcpy(b[j], a[i]);
						changed = 1;
					}
				}

				if (!is_num(a[i])) {
					r[i][0] = '\0';
				}
			}
		}
	}

	for (int i = 0; i < n; i++) {

		if (r[i][0] != '\0' && o[i] == '\0' && !isdigit(a[i][0])) {
			r[i][0] = '\0';
		}
	}

	for (int i = 0; i < n; i++) {
		if (r[i][0]) {
			if (o[i])
				printf("%s = %s %c %s\n", r[i], a[i], o[i], b[i]);
			else
				printf("%s = %s\n", r[i], a[i]);
		}
	}
	return 0;
}
