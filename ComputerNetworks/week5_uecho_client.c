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
    int sock; // 소켓 파일 디스크립터 변수 선언
    char message[BUF_SIZE]; // 송수신에 사용될 버퍼
    int str_len; // 보낼 버퍼의 크기
	socklen_t adr_sz;//주소정보가 있는 구조체의 크기

	struct sockaddr_in serv_adr, from_adr; // 발신지 수신지 주소정보를 담을 구조체 변수
	//하나의 서버에서만 데이터를 받기때문에 둘은 서로 같은 곳을 가리킬 거임

    if(argc!=3){
        printf("Usage : %s <IP> <port>\n", argv[0]);
        exit(1);
    }

	sock = socket(PF_INET, SOCK_DGRAM, 0); //서버코드와 마찬가지로 udp 소켓 생성 
    if(sock == -1)
        error_handling("socket() error");

    memset(&serv_adr, 0, sizeof(serv_addr)); // serv_adr 초기화
    serv_adr.sin_family=AF_INET;
    serv_adr.sin_addr.s_addr=inet_addr(argv[1]);
    serv_adr.sin_port=htons(atoi(argv[2]));
	
	//connect 함수 없이 바로 데이터
	// bind로 주소를 할당하지 않음 - sendto함수가 호출될때 주소를 소켓에 할당하게 되는데 이때 저절로 만들어지고 그대로 종료까지 유지
	while (1) {
		fputs("Insert message(q to quit) : ", stdout); 
		fgets(message, sizeof(message), stdin);// 키보드로 직접 입력하는 변수
		if (!strcmp(message, "q\n") || !strcmp(message, "Q\n")) break; // 어플리케이션 프로토콜로 약속한 거 4주차에 배움
		//Q를 입력하면 와일문 끝내고 소켓 닫음

		// 여기부터 데이터 송수신 시작												   
		// sendto함수 호출시 sock 변수에 ip주소 알아서 할당 됨
		sendto(sock, message, strlen(message), 0, (struct sockaddr*)*serv_adr, sizeof(serv_adr));
		
		adr_sz = sizeof(from_adr);
		// 서버에서 응답한 내용을 받기 위한 거
		str_len = recvfrom(sock, message, BUF_SIZE, 0, (struct sockaddr*)&from_adr, &adr_sz);
		message[str_len] = 0;
		printf("Message from server : %s", message);
		// while 문 안에서 sendto, recvfrom 쓸때 주소 구조체 변수를 담았다는 것을 유의해서 보기
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
//udp는 클라이언트가 먼저 실행돼도 상관 없음
// 대신 송수신때는 둘다 켜져있어야함