#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <unistd.h>
#include <arpa/inet.h>
#include <sys/socket.h>

#define BUF_SIZE 1024

void error_handling(char* message);
int calculate(int operands_len, int operands[], char op);

int main(int argc, char *argv[])
{
	if (argc != 2)
	{
		printf("Usage: %s <port>\n", argv[0]);
		exit(1);
	}

	int serv_sock = socket(PF_INET, SOCK_STREAM, 0);
	if (serv_sock == -1)
		error_handling("socket() error");

	struct sockaddr_in serv_addr;
	memset(&serv_addr, 0, sizeof(serv_addr));
	serv_addr.sin_family = AF_INET;
	serv_addr.sin_addr.s_addr = htonl(INADDR_ANY);
	serv_addr.sin_port = htons(atoi(argv[1]));

	if (bind(serv_sock, (struct sockaddr*)&serv_addr, sizeof(serv_addr)) == -1)
		error_handling("bind() error");

	if (listen(serv_sock, 5) == -1)
		error_handling("listen() error");

	struct sockaddr_in clnt_addr;
	socklen_t clnt_addr_size = sizeof(clnt_addr);

	int recv_cnt;
	for (int i = 0; i < 5; i++) {
		int clnt_sock = accept(serv_sock, (struct sockaddr*)&clnt_addr, &clnt_addr_size);
		if (clnt_sock == -1)
			error_handling("accept() error");
		else
			printf("Connected client %d \n", i + 1);

		int operands_len = 0;
		read(clnt_sock, &operands_len, 1);

		int recv_len = 0;
		char opinfo[BUF_SIZE];
		while (recv_len < (operands_len * sizeof(int) + 1)) {
			recv_cnt = read(clnt_sock, &opinfo[recv_len], BUF_SIZE - 1);
			recv_len += recv_cnt;
		}

		char op = opinfo[recv_len - 1];
		int result = calculate(operands_len, (int*)opinfo, op);
		write(clnt_sock, (char*)&result, sizeof(int));
		close(clnt_sock);
	}
	close(serv_sock);

	return 0;
}

int calculate(int operands_len, int operands[], char op) {
	int result = operands[0];
	for (int i = 1; i < operands_len; i++) {
		if (op == '+') {
			result += operands[i];
		}
		else if (op == '-') {
			result -= operands[i];
		}
		else if (op == '*') {
			result *= operands[i];
		}
	}
	return result;
}

void error_handling(char* message)
{
	fputs(message, stderr);
	fputc('\n', stderr);
	exit(1);
}