#define _CRT_SECURE_NO_WARNINGS
#include <stdio.h>
#include <math.h>
int main() {
	FILE *fp = fopen("oil2.txt", "wt");
	FILE *oil = fopen("oil.txt", "rt");
	int year;
	float production;

	if (fp) {
		while (EOF != fscanf(oil, "%d %f\n", &year, &production)) {
			production = log(production);
			fprintf(fp, "%d %f\n", year, production);
		}
		fclose(oil);
		fclose(fp);
	}
}