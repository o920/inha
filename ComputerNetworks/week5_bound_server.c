#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <unistd.h>
#include <arpa/inet.h>
#include <sys/socket.h>
#include <fcntl.h>
#define BUF_SIZE 30 
void error_handling(char *message);

int main(int argc, char *argv[])
{
	int sock; 
	char message[BUF_SIZE];
	int str_len;
	socklen_t adr_sz;
	struct sockaddr_in my_adr, your_adr;

    if(argc!=2){ 
        printf("Usage : %s <port>\n", argv[0]);
        exit(1);
    }

    serv_sock=socket(PF_INET, SOCK_DGRAM, 0);
	if(serv_sock ==-1)
        error_handling("UDP socket creation error");

    memset(&my_addr, 0, sizeof(my_addr)); // 서버어드레스 구조체 초기화
    my_addr.sin_family=AF_INET; 
    my_addr.sin_addr.s_addr=htonl(INADDR_ANY); // 자신의 ip를 넣어주기
    my_addr.sin_port=htons(atoi(argv[1]));  // 포트번호

    if(bind(sock, (struct sockaddr*) &my_addr, sizeof(my_addr))==-1) 
        error_handling("bind() error");
        
	for (int i = 0; i < 3; i++) {
		sleep(5); // 5초간 실행 멈춤
		adr_sz = sizeof(your_adr); // 데이터 발신지 정보를 담을 구조체 크기
		// recvfrom을 호출 : 소켓에 들어온 데이터를 메세지 버퍼에 저장하고 your_adr 저장
		str_len = recvfrom(sock, message, BUF_SIZE, 0, (struct sockaddr*)&your_adr, &adr_sz);
		// udp 데이터 경계가 있음. 5초 대기 동안 1,2,3 메세지 모두 수신되면 그냥 맨처음 데이터만 가져오게 됨
		// 받은 데이터를 출력하고 몇번째 데이터인지 출력
		printf("Message %d : %s \n", i + 1, message);
		
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