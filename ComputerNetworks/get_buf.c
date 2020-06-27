#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
#include <sys/socket.h>
void error_handling(char *message);

int main(int argc, char *argv[])
{
	int sock;
	int snd_buf, rcv_buf, state; // 버퍼 크기 저장할 변수 
	socklen_t len;

	sock = socket(PF_INET, SOCK_STREAM, 0); // 소켓생성
	len = sizeof(snd_buf); // 출력 버퍼 크기 참조
	state = getsockopt(sock, SOL_SOCKET, SO_TYPE, (void*)&snd_buf, &len);
	//출력버퍼에 접근해서 크기를 snd_buf에 저장
	
	if (state) error_handling("getsockopt() error!");
	len = sizeof(rcv_buf);
	state = getsockopt(sock, SOL_SOCKET, SO_RCVBUF, (void*)&rcv_buf, &len);
	if (state) error_handling("getsockopt() error!");

	printf("Input buffer size : %d \n", rcv_buf);
	printf("Output buffer size : %d \n", snd_buf);
	return 0;
}

void error_handling(char *message) 
{
    fputs(message, stderr);
    fputc('\n', stderr);
    exit(1);
}