#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
#include <sys/wait.h>
#include <string.h>
#include <netinet/in.h>
#include <arpa/inet.h>
#include <sys/socket.h>
#include <pthread.h>

#define BUF_SIZE 100
#define MAX_CLNT 256
void* handle_clnt(void* arg);		//스레드 메인함수
void send_msg(char* msg, int ln);	//메세지 보내는 함수
void error_handling(char* message);

int clnt_cnt = 0;		//클라이언트 개수
int clnt_socks[MAX_CLNT];
pthread_mutex_t mutx;	//뮤텍스 참조 변수

int main(int argc, char* argv[]) {
	int serv_sock, clnt_sock;
	struct sockaddr_in serv_adr, clnt_adr;
	int clnt_adr_sz;
	pthread_t t_id;
	if (argc != 2) {
		printf("Usage : %s \n", argv[0]);
		exit(1);
	}
	pthread_mutex_init(&mutx, NULL);					//뮤텍스 생성 초기화
	serv_sock = socket(PF_INET, SOCK_STREAM, 0);		//tcp 소켓 생성
	memset(&serv_adr, 0, sizeof(serv_adr));				//초기화 과정
	serv_adr.sin_family = AF_INET;
	serv_adr.sin_addr.s_addr = htonl(INADDR_ANY);
	serv_adr.sin_port = htons(atoi(argv[1]));

	//서버 소켓에 주소를 할당
	if (bind(serv_sock, (struct sockaddr*) & serv_adr, sizeof(serv_adr)) == -1)
		error_handling("bind error");
	//서버 소켓을 연결 요청 가능한 상태로 만듦
	if (listen(serv_sock, 5) == -1)
		error_handling("listen error");

	while (1) {
		clnt_adr_sz = sizeof(clnt_adr);
		clnt_sock = accept(serv_sock, (struct sockaddr*) & clnt_adr, &clnt_adr_sz);

		pthread_mutex_lock(&mutx);
		clnt_socks[clnt_cnt++] = clnt_sock;	//클라이언트 소켓 관리 배열을 임계영역으로 함
		pthread_mutex_unlock(&mutx);
		// 클라이언트 소켓 갯수도 임계영역임
		// 다른 스레드에서 이 배열, 변수에 접근하기 때문에 임계영역으로 관리함

		pthread_create(&t_id, NULL, handle_clnt, (void*)&clnt_sock);
		pthread_detach(t_id);	// 해당 스레드가 종료되면 메모리가 자연스럽게 소멸되게 함
		printf("connected client IP : %s \n", inet_ntoa(clnt_adr.sin_addr));
	}
	close(serv_sock);
	return 0;
}
void* handle_clnt(void* arg) {		//새롭게 생성된 클라이언트와 연결된 소켓 파일디스크립터를 인자로 받음
	int clnt_sock = *((int*)arg);
	int str_len = 0, i;
	char msg[BUF_SIZE];
	while ((str_len = read(clnt_sock, msg, sizeof(msg))) != 0) send_msg(msg, str_len);
	// 받은 메세지를 그대로 함수에 넣어서 호출해줌
	// 클라이언트 연결 종료 시 while 문을 빠져나옴
	pthread_mutex_lock(&mutx);
	for (i = 0; i < clnt_cnt; i++) {
		// 연결되어 있던 클라이언트 소켓을 정리함
		if (clnt_sock == clnt_socks[i]) { //해당 소켓이 어디에 있는지 찾음
			while (i < clnt_cnt - 1) {
				clnt_socks[i] = clnt_socks[i + 1];
				i++;
			}
			break;
		}	
	}
	clnt_cnt--;
	// 클라이언트 소켓을 관리하는 배열과, 클라이언트 소켓 변수가 변화되기 때문에
	// 다른 스레드가 접근하지 못하게 임계영역으로 설정함
	pthread_mutex_unlock(&mutx);
	close(clnt_sock);
	return NULL;
}
void send_msg(char* msg, int len) {
	int i;
	pthread_mutex_lock(&mutx);
	for (i = 0; i < clnt_cnt; i++) write(clnt_socks[i], msg, len);
	// 클라이언트 소켓 배열에 접근하기 때문에 임계영역으로 설정
	pthread_mutex_unlock(& mutx);
}
void error_handling(char* buf) {
	fputs(buf, stderr);
	fputc('\n', stderr);
	exit(1);
}