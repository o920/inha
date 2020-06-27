#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <unistd.h>
#include <arpa/inet.h>
#include <sys/socket.h>
#include <fcntl.h>
#define BUF_SIZE 30 
void error_handling(char *message); //에러처리함수

int main(int argc, char *argv[])
{
	int serv_sock; // udp 파일디스크립터를 저장할 변수
	char message[BUF_SIZE]; // 수신할 데이터
	int str_len;
	socklen_t clnt_adr_sz; //구조체 크기 담을 변수

    struct sockaddr_in serv_addr; // 서버소켓 주소 구조체
    struct sockaddr_in clnt_adr; // 
    


    if(argc!=2){ //실행 방법 오류 처리하는 부분
        printf("Usage : %s <port>\n", argv[0]); // 서버쪽이라서 포트번호만 들어간다함
        exit(1);
    }

    serv_sock=socket(PF_INET, SOCK_DGRAM, 0);
	// 두번째 인자 DGRAM은 udp 비연결지향 소켓이 생성되어 파일디스크립터를 저장하게 됨
	if(serv_sock ==-1)
        error_handling("UDP socket creation error");

    memset(&serv_addr, 0, sizeof(serv_addr)); // 서버어드레스 구조체 초기화
	//serv_addr은 sockaddr_in 구조체 타입임. 이 구조체는 다시 주소정보를 담는 serv_addr구조체와 크기가 같음
	//iPv4 주소체계에서 주소를 더 쉽게 담기 위해서 아래 변수들을 초기화하고 형변환해서 들어감
    serv_addr.sin_family=AF_INET; 
    serv_addr.sin_addr.s_addr=htonl(INADDR_ANY); // 자신의 ip를 넣어주기
    serv_addr.sin_port=htons(atoi(argv[1]));  // 포트번호

	// 서버 주소정보를 가진 구조체를 넘겨서 소켓 할당
	// 목적지가 되어져야하는 소켓이라서 주소를 할당받아야함
	// 주소정보 초기화한걸 넣음
    if(bind(serv_sock, (struct sockaddr*) &serv_addr, sizeof(serv_addr))==-1) 
        error_handling("bind() error");
        
	//에코작업
	while (1) {
		clnt_adr_sz = sizeof(clnt_adr); // 클라이언트 주소 정보 담는 구조체 변수 길이 저장
		// recvfrom으로 클라이언트로 부터 받은 정보 수신
		// 옵션 안써서 세번째 인자에 0 들어감
		//서버 소켓으로부터 최대 buf_size만큼 받아옴
		// 받아오는 클라이언트 정보를 저장하는 변수 주소 네번째 거기서 얼만큼 읽을지가 다섯번째
		str_len = recvfrom(serv_sock, message, BUF_SIZE, 0, (struct sockaddr*)&clnt_adr, &clnt_adr_sz);
		//수신할 데이터를 그대로 보내주기 위해서 sendto함수 호출
		//serv_sock으로 보낸다는 말 - 실제 서버 소켓과는 다르니까 구분해서 이해하기
		//받은만큼 메세지를 전송할거라고 인자 두번째 세번째에 넘겨주고 이ㅣㅆ음
		// 옵션 없어서 0
		// 클라이언트 주소 정보를 네번째 다섯번째에 넘겨줌, recvfrom에서 받아온 주소정보를 그대로 넘겨줌
		sendto(serv_sock, message, str_len, 0, (struct sockaddr*)&clnt_adr, clnt_adr_sz);
		//계속 반복돼서 while문에 있음- 무한루프임. 탈출 조건이 없음.
	}
    close(serv_sock); // 서버 소켓을 강제로 닫는 일이 없으면 while문을 벗어나지 않아서 이거 호출 안됨
    return 0;
}

void error_handling(char *message) 
{
    fputs(message, stderr);
    fputc('\n', stderr);
    exit(1);
}