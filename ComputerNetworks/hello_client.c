#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <unistd.h>
#include <arpa/inet.h>
#include <sys/socket.h>

void error_handling(char* messsage);

int main(int argc, char*argv[]) {
	int sock;
	struct sockaddr_in serv_addr;
	char message[30];
	int str_len;

	if (argc != 3) {
		printf("Usage : %s <IP> <port>\n", argv[0]);
		exit(1);
	}
	sock = socket(PF_INET, SOCK_STREAM, 0); // 소켓 생성
	if (sock == -1)error_handling("socket() error");

	memset(&serv_addr, 0, sizeof(serv_addr)); //주소 구조체 초기화
	serv_addr.sin_family = AF_INET;
	serv_addr.sin_addr.s_addr = inet_addr(argv[1]); // argv[1] 은 ip주소
	serv_addr.sin_port = htons(atoi(argv[2])); //argv[2] 는 port 번호

	//connect함수 : 연결 요청
	if (connect(sock, (struct sockaddr*)& serv_addr, sizeof(serv_addr)) == -1)
		error_handling("connect() error!");
	str_len = read(sock, message, sizeof(message) - 1); //read는 수신하는 함수
	if (str_len == -1) error_handling("read() error!");
	printf("Message from server: %s \n", message);
	close(sock);
	return 0;
}

void error_handling(char* message) {
	fputs(message, stderr);
	fputc('\n', stderr);
	exit(1);
}
