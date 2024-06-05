#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <unistd.h>
#include <arpa/inet.h>
#include <sys/socket.h>

#define BUF_SIZE 1024

void error_handling(char* message);

int main(int argc, char* argv[])
{
	if (argc != 3)
	{
		printf("Usage: %s <IP> <port>\n", argv[0]);
		exit(1);
	}

	int sock = socket(PF_INET, SOCK_STREAM, 0);
	if (sock == -1)
		error_handling("socket() error");

	struct sockaddr_in serv_addr;
	memset(&serv_addr, 0, sizeof(serv_addr));
	serv_addr.sin_family = AF_INET;
	serv_addr.sin_addr.s_addr = inet_addr(argv[1]);
	serv_addr.sin_port = htons(atoi(argv[2]));

	if (connect(sock, (struct sockaddr*)&serv_addr, sizeof(serv_addr)) == -1)
		error_handling("connect() error");
	else 
		puts("Connected...");

	// 보낼 문자열 입력 
	fputs("Write message to send: ", stdout);
	char message[BUF_SIZE];
	scanf("%s", message);

	int len = strlen(message);

	// 버퍼 입력
	char buff[BUF_SIZE];
	buff[sizeof(int) - 1] = (char)len;
	strcat(buff, message);

	for (int i = 0; i < len + 1; i++) {
		buff[sizeof(int) + i] = message[i];
	}

	write(sock, buff, sizeof(int) + len);

	/*int result = 0;
	read(sock, &result, sizeof(int));
	printf("Operation result: %d \n", result);*/
	
	close(sock);
	return 0;
}

void error_handling(char* message)
{
	fputs(message, stderr);
	fputc('\n', stderr);
	exit(1);
}