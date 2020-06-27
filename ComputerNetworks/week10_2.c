// 멀티 프로세스 기반 서버 코드에 클라이언트가 보낸 문자열을 전달되는 순서대로 파일에 저장하는 코드 추가
// 새로운 프로세스 만들고 클라이언트에서 서비스를 제공하는 프로세스로부터 문자열 정보를 수신하도록 할거임
// 서버에서 두 프로세스간 통신을 다루기 때문에 파이프를 통해서 그 기능을 구현할 거임
// 확장된 멀티프로세스 기반 서버 코드는 다른 에코클라이언트랑 실행해도 됨
// 10주차 입출력 분할한 에코 클라이언트랑 실행해보기
#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
#include <sys/wait.h>
#include <string.h>
#include <signal.h>
#include <arpa/inet.h>
#include <sys/socket.h>

#define BUF_SIZE 100
void error_handling(char* message);
void read_childproc(int sig);

int main(int argc, char* argv[]) {
	int serv_sock, clnt_sock;
	strunct sockaddr_in serv_adr, clnt_adr;
	int fds[2];	// 파이프 파일디스크립터 저장될 배열

	pid_t pid;
	struct sigaction act; // 시그널 등록을 위해 어떤 함수가 시그널 이후에 호출될지 정보를 저장할 sigaction 구조체
	// 지난시간에 수업했으니까 복습 하기 
	socklen_t adr_sz;
	int str_len, state;
	char buf[BUF_SIZE];
	if (argc != 2) {
		printf("Usage : %s \n", argv[0]);
		exit(1);
	}

	act.sa_handler = read_childproc; // 시그널 등록 , 
	sigemptyset(% act, sa_mask);
	act.sa_flags = 0;	// 지금 사용안할 거라서 0으로 초기화
	state = sigaction(SIGCHLD, &act, 0); 
	// 자식이 종료하게 되면 발생하는 시그널이 발생하면 act에 담긴 함수 호출하도록 시그널 등록

	serv_socket(PF_INET, SOCK_STREAM, 0); // tcp 소켓 생성
	memset(&serv_adr, 0, sizeof(serv_adr)); // 초기화 과정
	serv_adr.sin_family = AF_INET;
	serv_adr.sin_addr.s_addr = htonl(INADDR_ANY);
	serv_adr.sin_port = htons(atoi(argv[1]);

	// 서버 소켓에 주소를 할당하는 중
	if (bind(serv_sock, (struct sockaddr*) & serv_adr, sizeof(serv_adr)) == -1)
		error_handling("bind error");
	// 연결 요청 가능한 상태로 서버 소켓을 만들어 줌
	if (listen(serv_sock, 5) == -1)
		error_handlling("listen error");

	// 파이프 함수로 부모 프로세스가 파이프를 생성하고 있음
	pipe(fds);	// 아직 자식은 없음
	pid = fork(); // 자식을 생성하면 부모랑 자식이 통신을 할 수 있게 됨 파이프로
	if (pid == 0) {	
		FILE* fp = fopen("echomsg.txt", "\t"); // 쓰기용으로 파일 열어줌
		char msgbuf[BUF_SIZE];	
		int len;
		for (int i = 0; i < 10; i++) {
			len = read(fds[0], msgbuf, BUF_SIZE); // 파이프 출구를 통해 read함수로 읽어오고 msgbuf에 저장
			fwrite((void*)msgbuf, 1, len, fp); // 저장된 데이터를 fwrite로 열어 놓은 파일에 저장
		} // 이 과정 열번 반복
		fclose(fp); // 닫고 종료
		return 0;
	}
	// 파일 저장하는 자식 프로세스 끝
	// 부모 프로세스 부분 시작
	// if 문이랑 겹치지 않는다는 거 확인하기
	while (1) {
		adr_sz = sizeof(clnt_adr);
		// 데이터 송수신용 소켓 생성
		clnt_sock = accept(serv_sock, (struct sockaddr*) & clnt_adr, &adr_sz);
		if (clnt_sock == -1) continue;
		else puts("new clinet connected");
		pid = fork();	// 새로운 자식 프로세스 생성
		// 이 자식 프로세스는 데이터 송수신을 담당하는 프로세스
		// 두번째 자식 프로세스, 이전에 파일 저장하는 프로세스는 첫번째 자식 프로세스
		if (pid == 0) { 
			close(serv_sock); // 생성되고 나서 연결요청을 받는 서버 소켓은 자기에게 필요없어서 close
			while ((str_len = read(clnt_sock, buf, BUF_SIZE)) != 0) {
				// 소켓으로부터 데이터를 읽어와서 버퍼에 저장
				write(clnt_sock, buf, str_len); // 다시 클라이언트 소켓을 통해서 에코되고
				write(fds[1], buf, str_len); // fds 1 파이프의 입구를 통해 다른 프로세스와 공유될 수 있는 공간에 그 데이터를 저장
			} // 이 데이터가 첫번째 자식프로세스가 읽게되는 데이터임
			close(clnt_sock);// 데이터 전송이 끝나면 클라이언트와 연결된 소켓 파일 디스크립터 닫고 종료
			puts("client disconnected");
			return 0;
		}
		else close(clnt_sock);
		return 0;
	}
}

// 이전시간 내용 복습
// 자식 프로세스가 종료되면 좀비가 되지 않도록 시그널 등록할 때 썼던 함수
// SIGCHLD 발생하면 이 함수가 호출되도록 시그널 등록해줬음
void read_childproc(int sig) {
	pid_t pid;
	int status;
	pid = waitpid(-1, &status, WNOHANG);
	// 자식 프로세스가 잘 종료되도록 도와주는 함
	//  첫번째 인자가 -1이면 임의의 자식이 종료될 경우
	// status가 가리키는 곳에 종료되는 상황 정보를 저장함
	// WNOHANG 옵션 : waitpid는 blocking 상태에 놓이지 않음
	printf("removed proc id : %d \n", pid);
	// 자식의 pid를 받아서 출력하고 끝남
}

void error_handling(char* message) {
	fputs(message, stderr);
	fputc('\n', stderr);
	exit(1);
}
// 실행시 client를 두개 이상 키기
// 다중 접속 서버가 잘 되는 지 확인하기
// 새롭게 프로세스가 생성되는지도 확인하기
// 두개면 데이터 송수신하는 프로세스가 두개가 생성되어야 하는데 실제 종료는 세개가 됨
// 밑에 두개가 디스커넥트하고 나온 pid임