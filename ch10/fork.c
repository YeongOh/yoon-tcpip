#include <stdio.h>
#include <unistd.h>

int gval = 10;

int main(int argc, char* argv[])
{
	int lval = 20;

	pid_t pid = fork();
	if (pid == 0) {
		gval += 2;
		lval += 2;
	} else {
		gval -= 2;
		lval -= 2;
	}

	if (pid == 0) {
		printf("Child proc: [%d, %d]\n", gval, lval);
	} else {
		printf("Parent proc: [%d, %d]\n", gval, lval);
	}

	return 0;
}
