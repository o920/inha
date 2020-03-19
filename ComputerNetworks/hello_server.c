#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <unistd.h>
#include <arpe/inet.h>
#include <sys/socket.h>

void error_handling(char* message);

int main(int argc, char* argv[]) {
	int serv_sock;
	int clnt_sock;

	struct sockaddr_in serv_addr;
	struct sockaddr_in clnt_addr;
	socklne_t clnt_addr_size;

	char message[] = "Hello World!"; //송신할 데이터

	if (argc != 2) { // 서버파일 실행할 때 port 잘못되면 나오는 에러
		printf("Usage : %s <port>\n", argv[0]);
		exit(1);
	}

	serv_sock = socket(PF_INET, SOCK_STREAM, 0); // 소켓 생성
	if (serv_sock == 1) error_handling("socket() error");

	memset(&serv_addr, 0, sizeof(serv_addr)); // 서버 주소 구조체를 초기화
	serv_addr.sin_family = AF_INET;
	serv_addr.sin_addr.s_addr = htonl(INADDR_ANY);
	serv_addr.sin_port = htons(atoi(argv[1])); 

	//bind함수 : 소켓 주소 할당
	if (bind(serv_sock, (struct sockaddr*) &serv_addr, sizeof(serv_addr)) == 1) error_handling("bind() error");
	//listen함수 : 연결 요청이 가능한 상태로 변경
	if (listen(serv_sock, 5) == -1) error_handling("listen() error");
	//연결 요청 가능한 상태가 됨

	clnt_addr_size = sizeof(clnt_addr);
	//accept함수 : 연결 요청을 받음
	clnt_sock = accept(serv_sock, (struct sockaddr*) &clnt_addr, &clnt_addr_size);
	if (clnt_sock == -1) error_handling("accept() error");

	write(clnt_sock, message, sizeof(message)); // write함수 : 메세지 송신
	close(clnt_sock);
	close(serv_sock);
	return 0;
	//닫고 종료
}

void error_handling(char* message) {
	fputs(message, stderr);
	fputc('\n', stderr);
	exit(1);
}
