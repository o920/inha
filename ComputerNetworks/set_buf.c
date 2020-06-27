#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
#include <sys/socket.h>
void error_handling(char *message);

int main(int argc, char *argv[])
{
	int sock;
	int snd_buf=1024*3, rcv_buf=1024*3, state; //3072크기로
	socklen_t len;

	sock = socket(PF_INET, SOCK_STREAM, 0); 
	//입력버퍼 크기변경중
	state = setsockopt(sock, SOL_SOCKET, SO_RCVBUF, (void*)&rcv_buf, &len);
	if (state) error_handling("getsockopt() error!");
	//입력된값 가져와서 출력함수의 버퍼크기 변경 중
	state = setsockopt(sock, SOL_SOCKET, SO_TYPE, (void*)&snd_buf, &len);
	if (state) error_handling("getsockopt() error!");

	//get_buf 처럼 입출력 버퍼크기 읽어와서 출력하기 중임
	len = sizeof(snd_buf);
	state = getsockopt(sock, SOL_SOCKET, SO_TYPE, (void*)&snd_buf, &len);
	if (state) error_handling("getsockopt() error!");

	len = sizeof(rcv_buf);
	state = getsockopt(sock, SOL_SOCKET, SO_RCVBUF, (void*)&rcv_buf, &len);
	if (state) error_handling("getsockopt() error!");

	printf("Input buffer size : %d \n", rcv_buf);
	printf("Output buffer size : %d \n", snd_buf);
	//시그윈에서 하면 3072가 나올거임 
	return 0;
}

void error_handling(char *message) 
{
    fputs(message, stderr);
    fputc('\n', stderr);
    exit(1);
}