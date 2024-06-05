#include <stdio.h>
#include <stdlib.h>
#include <fcntl.h>
#include <unistd.h>
#define BUF_SIZE 100
void error_handling(char* message);

int main(void)
{
	int fd;
	char buff[] = "Let's go!\n";

	fd = open("data.txt", O_RDONLY);

	if (fd == -1)
		error_handling("open() error!");
	printf("file descriptor: %d \n", fd);

	if (read(fd, buff, sizeof(buff)) == -1)
		error_handling("read() error");
	printf("file data: %s", buff);
	close(fd);

	return 0;
}

void error_handling(char* message)
{
	fputs(message, stderr);
	fputc('\n', stderr);
	exit(1);
}