
#include <stdio.h>
#include <string.h>

typedef struct {
	char res[10], arg1[10], op[5], arg2[10];
	int is_arithmetic;
} tac;

typedef struct {
	char var[10];
	int reg;
} reg_map;

tac instructions[100];
reg_map map[100];
int tac_count = 0, map_count = 0, reg_counter = 1;

int get_var_reg(char *var) {
	for (int i = 0; i < map_count; i++) {
		if (strcmp(map[i].var, var) == 0)
			return map[i].reg;
	}
	return -1;
}

void bind_reg(char *var, int r) {
	for (int i = 0; i < map_count; i++) {
		if (strcmp(map[i].var, var) == 0) {
			map[i].reg = r;
			return;
		}
	}

	strcpy(map[map_count].var, var);
	map[map_count].reg = r;
	map_count++;
}

void print_operation(char *op) {
	if (strcmp(op, "+") == 0)
		printf("ADD ");
	else if (strcmp(op, "-") == 0)
		printf("SUB ");
	else if (strcmp(op, "*") == 0)
		printf("MUL ");
	else if (strcmp(op, "/") == 0)
		printf("DIV ");
}

int main() {
	char line[100];
	printf("Enter TAC and type END\n");

	while (fgets(line, sizeof(line), stdin)) {
		line[strcspn(line, "\n")] = 0;
		if (strcmp(line, "END") == 0)
			break;

		int token_count =
			sscanf(line, "%s = %s %s %s", instructions[tac_count].res, instructions[tac_count].arg1,
				   instructions[tac_count].op, instructions[tac_count].arg2);

		instructions[tac_count].is_arithmetic = (token_count == 4) ? 1 : 0;
		tac_count++;
	}

	printf("\n\n");
	for (int i = 0; i < tac_count; i++) {
		tac *t = &instructions[i];

		if (t->is_arithmetic) {
			int r1 = get_var_reg(t->arg1);
			if (r1 == -1) {
				r1 = reg_counter++;
				printf("LOAD R%d, %s\n", r1, t->arg1);
				bind_reg(t->arg1, r1);
			}

			int r2 = get_var_reg(t->arg2);
			if (r2 == -1) {
				r2 = reg_counter++;
				printf("LOAD R%d, %s\n", r2, t->arg2);
				bind_reg(t->arg2, r2);
			}

			int r_result = reg_counter++;
			print_operation(t->op);
			printf("R%d, R%d, R%d\n", r_result, r1, r2);
			bind_reg(t->res, r_result);
		}

		else {
			int r_rhs = get_var_reg(t->arg1);
			if (r_rhs != -1) {
				printf("STORE %s, R%d\n", t->res, r_rhs);
			} else {
				int r_new = reg_counter++;
				printf("LOAD R%d, %s\n", r_new, t->arg1);
				printf("STORE %s, R%d\n", t->res, r_new);
			}
		}
	}

	return 0;
}
