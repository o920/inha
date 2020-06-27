#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <unistd.h>
#include <arpa/inet.h>
#include <sys/socket.h>

#define BUF_SIZE 30
void error_handling(char* message);		
void read_routine(int sock, char* buf);		
void write_routine(int sock, char* buf);

int main(int argc, char* argv[]) {
	int sock;		// 클라이언트의 소켓 파일디스크립터
	pid_t pid;
	char buf[BUF_SIZE]; // 메시지 담는 버퍼
	struct sockaddr_in serv_adr; // 연결 요청할 서버 주소 담는 구조체
	if (argc != 3) {
		printf("Usage : %s <IP> <port>\n", argv[0]);
		exit(1);
	}

	sock = socket(PF_INET, SOCK_STREAM, 0);	//TCP 소켓, 이때 반환되는 파일 디스크립터가 fork 생성 후 자식 프로세스에게 복사되어 사용됨
	memset(&serv_adr, 0, sizeof(serv_adr)); // 초기화 과정
	serv_adr.sin_family = AF_INET;
	serv_adr.sin_addr.s_addr = inet_addr(argv[1]);
	serv_adr.sin_port = htons(atoi(argv[2]));
	// 이거 초기화 다시 복습하기

	if (connect(sock, (struct sockaddr*) & serv_adr, sizeof(serv_adr)) == -1) // 연결요청
		error_handling("connect error");

	pid = fork(); // 자식프로세스 생성
	if (pid == 0) write_routine(sock, buf); // 자식 프로세스는 반환값이 0 임 write
	else read_routine(sock, buf);			// 부모 프로세스는 자식의 pid 반환해서 read
	close(sock);
	return 0;
}

void read_routine(int sock, char* buf) { // 소켓 파일 디스크립터 넘겨 받아서 해당 데이터를 읽기
	while (1) {
		int str_len = read(sock, buf, BUF_SIZE); // 소켓으로부터 수신한 데이터를 버퍼에 담는 read함수 호출
		if (str_len == 0) return; // 입력받은 게 없으면 return
		buf[str_len] = 0;
		printf("Message from server : %s ", buf); // 입력받은 내용 프린트
	}
}

void write_routine(int sock, char* buf) { // 해당 파일디스크립터 사용해서 write하는 함수
	while (1) {
		fgets(buf, BUF_SIZE, stdin);	// 해당 데이터를 서버로 전송한다는 내용
		if (!strcmp(buf, "q\n") || !strcmp(buf, "Q\n")) { // q를 입력받지 않으면 계속 입력 함
			shutdown(sock, SHUT_WR); 
			return;
			// q가 입력되면 소켓의 출력 스트림을 닫기, tcp half close 기법
			// 출력 스트림만 닫고 입력 스트림은 살아있어서 서버에서 클라이언트로 데이터를 계속 보내도
			// 부모 프로세스가 계속 데이터를 read할 수 있음
		}
		write(sock, buf, strlen(buf)); // 이땓 입력받은 걸 계속 씀
		// 송신만 담당함
	}
}

void error_handling(char* message) {
	fputs(message, stderr);
	fputc('\n', stderr);
	exit(1);
}
// 입력받는 값이 없음
// 이전에는 데이터를 기다리는 내용
// 이번에는 데이터 수신을 기다리지 않아서 입력란이 없음
// 출력의 간결함을 위해서 빠진거임
// 9주차 멀티프로세스 기반 서버와 함께 실행
