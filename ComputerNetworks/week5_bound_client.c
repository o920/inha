#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <unistd.h>
#include <arpa/inet.h>
#include <sys/socket.h>
#define BUF_SIZE 30
void error_handling(char *message);

int main(int argc, char *argv[])
{
    int sock;
	char msg1[] = "HI!";
	char msg2[] = "I'm another UDP host!";
	char msg3[] = "Nice to meet you";
    int str_len; // 보낼 버퍼의 크기
	socklen_t your_adr_sz;//목적지 주소정보가 있는 구조체의 크기
	struct sockaddr_in your_adr; //수신시 어디서 왔는지 확인하는 구조체
	//자신의 구조체 선언 안됨 - sendto 호출시 알아서 할당 됨

    if(argc!=3){
        printf("Usage : %s <IP> <port>\n", argv[0]);
        exit(1);
    }

	sock = socket(PF_INET, SOCK_DGRAM, 0); //서버코드와 마찬가지로 udp 소켓 생성 
    if(sock == -1)
        error_handling("socket() error");

    memset(&your_adr, 0, sizeof(your_addr)); // 목적지 정보 들어갈 구조체 초기화
    your_adr.sin_family=AF_INET;
    your_adr.sin_addr.s_addr=inet_addr(argv[1]); // ip주소 저장할 때 실행할때 넣은 주소를 넣음
    your_adr.sin_port=htons(atoi(argv[2]));
	
	//tcp에서 write 처럼 udp 에서 sendto 사용
	// 주소정보 할당 안해서 sendto 처음 호출 될때 이 udp 소켓에 ip 할당 되고, 이 소켓이 닫힐 때까지 이 주소 사용
	sendto(sock, msg1, sizeof(msg1), 0, (struct sockaddr*)&your_adr, sizeof(your_adr));
	sendto(sock, msg2, sizeof(msg2), 0, (struct sockaddr*)&your_adr, sizeof(your_adr));
	sendto(sock, msg3, sizeof(msg3), 0, (struct sockaddr*)&your_adr, sizeof(your_adr));
	// tcp에서는 write 세번 호출해서 보내면 데이터 경계없어서 모두 다 잘 전달 됐지만
	// udp 에서는 데이터 경계가 있어서 연달에서 sendto를 세번 호출하면
	// 빠르게 세번 보내도 세번의 recvfrom을 호출해야함
	// 5초 대기해서 recvfrom 을 받았지만 여기서는 sleep 안해서 그냥 쭉 보내는 거임
	// 대신 서버코드에서 5초 기다리면서 3번 recvfrom해서 천천히 받음
    close(sock); // 서버보다 먼저 빠르게 보내고 빨리 먼저 종료 될거임
	// 서버 코드에서 msg1,2,3가 도착하지만 첫 for 문에서 1만 받음 
	// 두번째 for문에서 msg2를 받고 하는거임
    return 0;
}

void error_handling(char *message)
{
    fputs(message, stderr);
    fputc('\n', stderr);
    exit(1);
}
