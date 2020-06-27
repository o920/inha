#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <unistd.h>
#include <arpa/inet.h>
#include <sys/socket.h>
#include <fcntl.h>
#define BUF_SIZE 50
void error_handling(char *message);

int main(int argc, char *argv[])
{
	int serv_sd, clnt_sd; //소켓변수
	FILE * fp;// 파일입출력에 사용될 포인터변수
	char buf[BUF_SIZE]; //내용 담길 버퍼
	int read_cnt; //읽은 데이터 크기

	struct sockaddr_in serv_adr, clnt_adr; //서버와 클라이언트 주소 담길 구조체
	socklen_t clnt_adr_sz;

    if(argc!=2){ 
        printf("Usage : %s <port>\n", argv[0]);
        exit(1);
    }

	fp = fopen("week6_file_server.c", "rb"); //rb = binary형식으로 읽기모드로 연다는거
	serv_sd = socket(PF_INET, SOCK_STREAM, 0); // tcp 소켓 생성

    memset(&serv_adr, 0, sizeof(serv_adr));  //서버주소 담긴 구조체 초기화
    serv_adr.sin_family=AF_INET; 
    serv_adr.sin_addr.s_addr=htonl(INADDR_ANY); 
    serv_adr.sin_port=htons(atoi(argv[1])); 

	bind(serv_sd, (struct sockaddr*) &serv_adr, sizeof(serv_adr)); // 주소 할당
	listen(serv_sd, 5); // listen 상태

	clnt_adr_sz = sizeof(clnt_adr);
	clnt_sd = accept(serv_sd, (struct sockaddr*)&clnt_adr, &clnt_adr_sz); // 연결요청받고
	// 송수신 담당 소켓 할당되는 거 udp 에는 없는거 한번더 기억하기
        
	while (1) {
		read_cnt = fread((void*)buf, 1, BUF_SIZE, fp); // fp가 가리키는 파일 스트림에서 퍼버에다가 데이터를 읽어옴
		//fread의 반환값은 읽어온 데이터의 개수임
		//fp 파일에서 1바이트씩 buf_size가 가리키는 개수만큼 buf에 저장
		if (read_cnt < BUF_SIZE) { // 마지막 사이즈까지 읽어오지 못한 경우 읽어온 부분까지만 보내기
			write(clnt_sd, buf, read_cnt); // 다시 전송
			break; // 읽어온데 까지만 읽고 while문 탈출
		}
		write(clnt_sd, buf, BUF_SIZE); // 읽어온 데이터만큼 다시 전송
	}
	shutdown(clnt_sd, SHUT_WR); // 클라이언트 소켓과 출력스트림만 닫아줌 half_close 방법
	//출력 스트림이 닫히면 eof가 클라이언트에게 감
	read(clnt_sd, buf, BUF_SIZE);// 클라이언트로부터 수신되는 thank you 수신
	printf("Message from client : %s \n", buf);

	fclose(fp);
	close(clnt_sd);
	close(serv_sd);
	return 0;
}

void error_handling(char *message) 
{
    fputs(message, stderr);
    fputc('\n', stderr);
    exit(1);
}