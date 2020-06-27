#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <unistd.h>
#include <arpa/inet.h>
#include <sys/socket.h>
#include <fcntl.h>
#define BUF_SIZE 1024

void error_handling(char *message); //에러처리함수

int main(int argc, char *argv[])
{
    int serv_sock, clnt_sock; // 서버 클라이언트 소켓
	char message[BUF_SIZE]; // 전송할 데이터
	int str_len, i; //read에서 몇글짜를 입력받았는지 세는게 str_len

    struct sockaddr_in serv_addr; // 서버소켓 주소 구조체
    struct sockaddr_in clnt_addr; // accept함수 인자로 들어갈 구조체, 클라이언트 주소정보
    socklen_t clnt_addr_size; // clnt_addr의 크기를 담아줌


    if(argc!=2){ //실행 방법 오류 처리하는 부분
        printf("Usage : %s <port>\n", argv[0]);
        exit(1);
    }

    serv_sock=socket(PF_INET, SOCK_STREAM, 0); // ipv4 기반 tcp 소켓 생성, 파일디스크립터 저장
	// 두번째 인자 같은 건 시험에 내기 쉬우니까 기억해두기
	if(serv_sock ==-1)
        error_handling("socket() error");

    memset(&serv_addr, 0, sizeof(serv_addr)); // 서버어드레스 구조체 초기화
    serv_addr.sin_family=AF_INET; 
    serv_addr.sin_addr.s_addr=htonl(INADDR_ANY); // 자신의 ip를 넣어주기
    serv_addr.sin_port=htons(atoi(argv[1]));  // 포트번호

	// 서버 주소정보를 가진 구조체를 넘겨서 소켓 할당
    if(bind(serv_sock, (struct sockaddr*) &serv_addr, sizeof(serv_addr))==-1) 
        error_handling("bind() error");
        
	// 서버 소켓이 인자로 들어가고, 연결 요청 대기 큐의 크기가 5
	// serv_sock 파일 디스크립터를 가리키는 소켓을 서버 소켓으로 바꿔서 연결요청 가능한 소켓으로 만들어 줌
    if(listen(serv_sock, 5)==-1)
        error_handling("listen() error");
	// 클라이언트가 연결요청 할 수 있는 상태가 됨
    clnt_addr_size=sizeof(clnt_addr); // clnt_addr 크기를 저장
    
	for (i = 0; i < 5; i++) {
		// accept 함수 : 연결 요청할 서버소켓과 클라이언트 소켓 주소 - 이 요청을 수락한다.
		clnt_sock = accept(serv_sock, (struct sockaddr*)& clnt_adr, &clnt_adr_sz);
		if (clnt_sock == -1) error_handling("accept() error");
		else printf("Connected client %d \n", i + 1); // 몇번째 클라이언트가 연결이 됐는지 출력
		// read함수로 실제 데이터 송수신하는 소켓 -> 클라이언트에게 전송하면 거기의 데이터를 받아서 read함
		while ((str_len = read(clnt_sock, message, BUF_SIZE)) != 0) // 클라이언트가 보낸게 없으면 빠져나옴
			write(clnt_sock, message, str_len); // 클라이언트에서 보낸걸 다시 그대로 보내주는 것 - 메아리처럼 그대로 보내줘서 echo 서버
		close(clnt_sock); // 클라이언트에서 보낸 데이터가 없으면, 클라이언트만 닫음
		// 대기 큐의 새로운 클라이언트 연결 요청을 수락함
	}
	// 다섯개 연결요청 모두 처리 후 서버 소켓 닫음
    close(serv_sock);
    return 0;
}

void error_handling(char *message)
{
    fputs(message, stderr);
    fputc('\n', stderr);
    exit(1);
}