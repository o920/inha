#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <unistd.h>
#include <arpa/inet.h>
#include <sys/socket.h>
#define BUF_SIZE 1024

void error_handling(char *message);

int main(int argc, char *argv[])
{
    int sock;
    struct sockaddr_in serv_addr; // 목적지 정보 저장하는 구조체
    char message[BUF_SIZE];
    int str_len; // 읽어 드리는 데이터 크기

    if(argc!=3){
        printf("Usage : %s <IP> <port>\n", argv[0]);
        exit(1);
    }

	sock = socket(PF_INET, SOCK_STREAM, 0); // tcp 소켓이 생성됨

    if(sock == -1)
        error_handling("socket() error");

    memset(&serv_addr, 0, sizeof(serv_addr));
    serv_addr.sin_family=AF_INET;
    serv_addr.sin_addr.s_addr=inet_addr(argv[1]);
    serv_addr.sin_port=htons(atoi(argv[2]));

	if (connect(sock, (struct sockaddr*)& serv_addr, sizeof(serv_addr)) == -1) error_handling("conncet() error");
	else puts("Connected....................");
	while (1) {
		fputs("input message(0 to quit) : " stdout);
		fgets(message, BUF_SIZE, stdin);

		if (!strcmp(message, "q\n" || !strcamp(message, "0\n"))) break;

		write(sock, message, strlen(message));
		str_len = read(sock, message, BUF_SIZE - 1);
		message[str_len] = 0;
		printf("Message from server : %s", message);
	}
    close(sock);
    return 0;
}

void error_handling(char *message)
{
    fputs(message, stderr);
    fputc('\n', stderr);
    exit(1);
}