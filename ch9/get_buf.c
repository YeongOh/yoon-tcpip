#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <unistd.h>
#include <sys/socket.h>

void error_handling(char* message);

int main(int argc, char* argv[])
{
	int sock = socket(PF_INET, SOCK_STREAM, 0);

	int snd_buf;
	int rcv_buf;
	socklen_t len = sizeof(snd_buf);
	int state = getsockopt(sock, SOL_SOCKET, SO_SNDBUF, (void*)&snd_buf, &len);
	if (state)
		error_handling("getsockopt() error!");

	state = getsockopt(sock, SOL_SOCKET, SO_RCVBUF, (void*)&rcv_buf, &len);
	if (state)
		error_handling("getsockopt() error!");

	printf("Input buffer size: %d \n", snd_buf);
	printf("Output buffer size: %d \n", rcv_buf);
	return 0;
}

void error_handling(char* message)
{
	fputs(message, stderr);
	fputc('\n', stderr);
	exit(1);
}