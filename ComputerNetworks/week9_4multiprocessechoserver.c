#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <unistd.h>
#include <signal.h>
#include <sys/wait.h>
#include <arpa/inet.h>
#include <sys/socket.h>

//챕터 4의 에코클라이언트랑 같이 실행하면 됨
#define BUF_SIZE 30
void error_handling(char* message);
void read_childproc(int sig);		//좀비  프로세스가 되지 않도록 자식 종료시 호출

int main(int argc, char* argv[]) {
	int serv_sock, clnt_sock;		// 소켓 파일 디스크립터 변수
	struct sockaddr_in serv_adr, clnt_adr;	// 주소 담는 구조체
	pid_t pid;						// 프로세스 주소
	struct sigaction act;			// 시그널 함수 담는 구조체
	socklen_t adr_sz;			
	char buf[BUF_SIZE];
	if (argc != 2) {
		printf("Usage : %s <port>\n", argv[0]);
		exit(1);
	}

	act.sa_handler = read_childproc;				// 시그널 등록을 위해서 함수 저장 중
	sigemptyset(&act.sa_mask);
	act.sa_flags = 0;
	state = sigaction(SIGCHLD, &act, 0);			// 실제 시그널 등록
	serv_sock = socket(PF_INET, SOCK_STREAM, 0);
	memset(&serv_adr, 0, sizeof(serv_adr));			// 구조체 초기화 과정
	serv_adr.sin_family = AF_INET;
	serv_adr.sin_addr.s_addr = htonl(INADDR_ANY);
	serv_adr.sin_port = htons(atoi(argv[1]));

	// 바인드로 서버 주소 할당
	if (bind(serv_sock, (struct sockaddr*) & serv_adr, sizeof(serv_adr)) == -1)
		error_handling("bind() error");
	// 실제 서버 소켓으로 만듦
	if (listen(serv_sock, 5) == -1)
		error_handling("listen() error");


	// 멀티 프로세스 
	while (1) {
		adr_sz = sizeof(clnt_adr);
		// accept를 대기하다가 새로운 자식프로세스를 계속해서 만드는 과정 반복
		clnt_sock = accept(serv_sock, (struct sockaddr*) & clnt_adr, &adr_sz);
		// 서버 소켓으로 연결 요청이 들어오면 수락하는 걸 부모 프로세스가 진행
		// 데이터 송수신용 소켓이 새로 만들어짐
		// 그 때 파일 디스크립터가 clnt_sock에 파일 디스크립터 저장 
		// 그 이후 fork 로 자식 프로세스 생성
		if (clnt_sock == -1) continue;
		else
			puts("new client connected..");
		pid = fork();
		if(pid ==-1){
			close(clnt_sock);
			continue;
		}
		if (pid == 0) {
			close(serv_sock);		// 필요없는 서버소켓 닫기
			while ((str_len = read(clnt_sock, buf, BUF_SIZE)) != 0) // 데이터 송수신
				write(clnt_sock, buf, str_len);
			
			close(clnt_sock);		// 연결 종료
			puts("client disconnected..");
			return 0;
		}	// 부모 프로세스는 살아있음, 종료 시 시그널 발생해서 부모가 시그널을 받음
			// 시그널을 핸들링하기 위해 함수 호출하여 좀비 프로세스가 생기지 않게 함
		else
			close(clnt_sock);	// 자식 프로세스의 파일 디스크립터를 닫음
	}
	close(serv_sock);			
	return 0;
}

void read_childproc(int sig) {
	pid_t pid;
	int status;
	pid = waitpid(-1, &status, WNOHANG);
	printf("removed proc id : %d \n", pid);
}

void error_handling(char* message) {
	fput(message, stderr);
	fputc('\n', stderr);
	exit(1);
}
//서버 하나 열고 시그윈 다른 거 여러개 열어서 클라이언트 여러개 접속시켜서 
// 정상적으로 서비스 다 하는지 확인학
// 다른 클라이언트 작동안하면 오타 확인하기
// 실행 결과화면은 ppt에 있음
// 컨트롤 씨 누르면 시그널에 의해서 함수 호출되고 아이디 출력하고 클라이언트 담당 프로세스 정상 종료
// 이 후에도 서버는 계속 살아서 요청을 계속 받음