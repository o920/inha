#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <unistd.h>
#include <arpa/inet.h>
#include <sys/socket.h>
#define TRUE 1;
#define FALSE 0;
void error_handling(char *message);

//이번 실습에서 서버역할할 거임
//챕터4에서 배운 에코클라이언트랑 같이 실행될거임
int main(int argc, char *argv[])
{
	int serv_sock, clnt_sock; //서버 클라이언트 소켓 파일디스크립터 담을 변수
	char message[30];
	int option, str_len;
	socklen_t optlen, clnt_adr_sz; // 크기들을 담을 변수
	struct sockaddr_in, serv_adr, clnt_adr; // 주소를 담을 구조체

    if(argc!=2){ 
        printf("Usage : %s <port>\n", argv[0]);
        exit(1);
    }

	serv_sock = socket(PF_INET, SOCK_STREAM, 0); // tcp 소켓 생성
	if (serv_sock == -1) error_handling("socket() error");

	/* 소켓의 옵션을 변경하는 부분 
	optlen=sizeof(option);
	option=TURE;
	setsockopt(serv_sock, SOL_SOCKET, SO_REUSEADDR, (void*)&option, optlen); 
	*/

	optlen = sizeof(option); // 옵션 길이에 옵션 길이를 담아줌
    memset(&serv_adr, 0, sizeof(serv_adr));  //서버주소 담긴 구조체 초기화
    serv_adr.sin_family=AF_INET; 
    serv_adr.sin_addr.s_addr=htonl(INADDR_ANY); 
    serv_adr.sin_port=htons(atoi(argv[1])); 

	// 서버 주소를 소켓에 할당
	// bind 함수 : 서버 소켓 역할을 하려면 주소를 먼저 할당해야지 클라이언트에서 주소를 갖고 찾아옴
	// 주소 할당을 하기 위해서 bind 함수를 사용, 첫번째 인자는 서버소켓 인자 listen 할 소켓
	// 두번째는 서버소켓의 주소 = 둘이 인자가 비슷해서 헷갈릴 수 있지만 bind에 필요한 인자랑 type을 생각해보기
	if (bind(serv_sock, (struct sockaddr*) &serv_adr, sizeof(serv_adr))) error_handling("bind() error"); // 주소 할당
	// 서버소켓이 이 함수를 통해서 listen하는 함수가 됨
	// 연결 요청을 보내면 그걸 받을 수 있는 상태
	// 첫번째 인자로는 서버 소켓의 파일 디스크립터고, 두번째는 연결요청 대기 큐의 크기
	if (listen(serv_sock, 5) == -1) error_handling("listen error");// listen 상태

	clnt_adr_sz = sizeof(clnt_adr);
	clnt_sock = accept(serv_sock, (struct sockaddr*)&clnt_adr, &clnt_adr_sz); //클라이언트 연결요청 수락
    // 연결요청 수락하는 서버소켓 파일디스크립터, 연결요청한 클라이언트의 주소 구조체
	// 데이터 송수신만하는 소켓을 새로 만들고 그 파일디스크립터를 반환함

	while ((str_len = read(clnt_sock,message,sizeof(message)))!=0) {
		// 클라이언트로부터 읽어오고 그만큼 다시 써서 보내줌
		write(clnt_sock, message, str_len);
		write(1, message, str_len); // 1은 표준출력을 말함
		// 파일디스크립터 설명할때 0,1,2 가 있는데 1이 표준 출력을 뜻함
		// 터미널에서 출력한 거 확인 가능
	}
	close(clnt_sock);
	return 0;
}

void error_handling(char *message) 
{
    fputs(message, stderr);
    fputc('\n', stderr);
    exit(1);
}