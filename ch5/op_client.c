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
	
	int operands_length;
	fputs("Operand count: ", stdout);
	scanf("%d", &operands_length);

	char message[BUF_SIZE];
	message[0] = (char)operands_length;

	for (int i = 0; i < operands_length; i++) {
		printf("Operands %d: ", i + 1);
		scanf("%d", (int*)&message[i * sizeof(int) + 1]);
	}
	// \n »èÁ¦
	fgetc(stdin);
	fputs("Operator: ", stdout);
	scanf("%c", &message[operands_length * sizeof(int) + 1]);
	write(sock, message, operands_length * sizeof(int) + 2);

	int result = 0;
	read(sock, &result, sizeof(int));
	printf("Operation result: %d \n", result);
	
	close(sock);
	return 0;
}

void error_handling(char* message)
{
	fputs(message, stderr);
	fputc('\n', stderr);
	exit(1);
}