#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <unistd.h>
#include <arpa/inet.h>
#include <sys/socket.h>
#define BUF_SIZE 50
void error_handling(char *message);

int main(int argc, char *argv[])
{
	int sd;
	FILE *fp;

	char buf[BUF_SIZE];
	int read_cnt;
	struct sockaddr_in serv_adr;

    if(argc!=3){
        printf("Usage : %s <IP> <port>\n", argv[0]);
        exit(1);
    }

	fp = fopen("receive.dat", "wb"); // 이 파일을 wb 바이너리 형식인데 쓰기 형식으로 이 파일을 염
	sd = socket(PF_INET, SOCK_STREAM, 0); // tcp 소켓

    memset(&serv_adr, 0, sizeof(serv_adr)); // 목적지 주소 구조체 초기화
    serv_adr.sin_family=AF_INET;
    serv_adr.sin_addr.s_addr=inet_addr(argv[1]); 
    serv_adr.sin_port=htons(atoi(argv[2]));
	
	connect(sd, (struct sockaddr*)&serv_adr, sizeof(serv_adr)); // 연결요청
	// ip주소와 포트번호가 아직 할당 안됐는데 connect함수 호출하면서 자동으로 할당됨

	while ((read_cnt = read(sd, buf, BUF_SIZE)) != 0) // read 계속 호출해서 검사, 서버로부터 받은 데이터 크기가 반환 값
		fwrite((void*)buf, 1, read_cnt, fp); // 파일 스트림 계속 작성
		// 파일을 끝까지 수신할 때까지 계속 read호출
		// eof가 와서 더 이상 읽을 게 없으면 0을 반환해서 while문 탈출

	puts("Receive file data");
	write(sd, "Thank you", 10);
	fclose(fp);
	close(sd);
    return 0;
}

void error_handling(char *message)
{
    fputs(message, stderr);
    fputc('\n', stderr);
    exit(1);
}
