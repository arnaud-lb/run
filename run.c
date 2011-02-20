
#include <unistd.h>
#include <errno.h>
#include <sys/types.h>
#include <sys/stat.h>
#include <fcntl.h>
#include <assert.h>
#include <stdio.h>

void print_usage(FILE * out, char * argv[]) {
	fprintf(out, "Usage: %s <command> <args ...>\n", argv[0]);
}

int main(int argc, char * argv[]) {

	int out;
	pid_t pid;

	if (argc < 2) {
		print_usage(stdout, argv);
		return 1;
	}

	pid = fork();
	if (pid < 0) {
		perror("fork");
		return 1;
	} else if (pid > 0) {
		return 0;
	}

	setsid();

	out = open("/dev/null", O_RDWR);
	if (!out) {
		perror("open /dev/null");
		return 1;
	}

	dup2(out, 0);
	dup2(out, 1);
	dup2(out, 2);

	++argv;

	execvp(argv[0], argv);

	perror("execv");
	return 1;
}

