#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
#include <string.h>
#include <arpa/inet.h>
#include <sys/socket.h>
#include <pthread.h>

#define BUF_SIZE 100
#define NAME_SIZE 20

void error_handling(char* message);		
void *recv_msg(void* arg);		
void* send_msg(void* arg);

char name[NAME_SIZE] = "[DEFAULT]";
char msg[BUF_SIZE];

int main(int argc, char* argv[]) {
	int sock;						// 클라이언트 소켓 파일디스크립터
	struct sockaddr_in serv_adr;	// 연결 요청할 서버 주소를 담는 구조체
	pthread_t snd_thread, rcv_thread;
	void* thread_return;
	if (argc != 4) {
		printf("Usage : %s <IP> <port> <name>\n", argv[0]);
		exit(1);
	}
	sprintf(name, "[%s] \n", argv[3]);

	sock = socket(PF_INET, SOCK_STREAM, 0);			//tcp 소켓 생성
	memset(&serv_adr, 0, sizeof(serv_adr));			//초기화
	serv_adr.sin_family = AF_INET;
	serv_adr.sin_addr.s_addr = inet_addr(argv[1]);
	serv_adr.sin_port = htons(atoi(argv[2]));

	if (connect(sock, (struct sockaddr*) & serv_adr, sizeof(serv_adr)) == -1) //연결요청
		error_handling("connect error");

	pthread_create(&snd_thread, NULL, send_msg, (void*)&sock);
	pthread_create(&rcv_thread, NULL, recv_msg, (void*)&sock);
	pthread_join(snd_thread, &thread_return);
	pthread_join(rcv_thread, &thread_return);
	close(sock);
	return 0;
}

void* send_msg(void* arg) {
	int sock = *((int*)arg);
	char name_msg[NAME_SIZE + BUF_SIZE];
	while (1) {
		fgets(msg, BUF_SIZE, stdin);
		if (!strcmp(msg, "q\n") || !strcmp(msg, "Q\n")) {
			close(sock);
			exit(0);
		}
		sprintf(name_msg, "%s %s", name, msg);
		write(sock, name_msg, strlen(name_msg));
	}
	return NULL;
}

void* recv_msg(void* arg) {
	int sock = *((int*)arg);
	char name_msg[NAME_SIZE + BUF_SIZE];
	int str_len;
	while (1) {
		str_len = read(sock, name_msg, NAME_SIZE + BUF_SIZE - 1);
		if (str_len = -1) return (void*)-1;
		name_msg[str_len] = 0;
		fputs(name_msg, stdout);
	}
	return NULL;
}

void error_handling(char* message) {
	fputs(message, stderr);
	fputc('\n', stderr);
	exit(1);
}
