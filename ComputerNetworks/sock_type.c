#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
#include <sys/socket.h>
void error_handling(char *message);

int main(int argc, char *argv[])
{
	int tcp_sock, udp_sock;
	int sock_type; // 옵션 정보 읽어와서 저장할 변수임
	socklen_t option;
	int state;

	optlen = sizeof(sock_type);
	//소켓 생성해서 각각 파일디스크립터 저장 중
	tcp_sock = socket(PF_INET, SOCK_STREAM, 0); 
	udp_sock = socket(PF_INET, SOCK_DGRAM, 0);
	//각각 어떤 변수 갖는지 출력해줌
	printf("SOCK_STREAM : %d \n", SOCK_STREAM);
	printf("SOCK_DGRAM : %d \n", SOCK_DGRAM);

	//tcp 소켓 넣고 인자 어쩌구저쩌구 넘겨주고 주소 전달하고 자시고 해서 
	state = getsockopt(tcp_sock, SOL_SOCKET, SO_TYPE, (void*)&sock_type, &optlen);
	if (state) error_handling("getsockopt() error!");
	printf("Socket type one : %d \n", sock_type); // 어떤 값 들어갔는지 출력해줌
	
	// 이 변수가 공유되고 있어서 출력해준담에 그거 해준거임 먼말알지
	state = getsockopt(udp_sock, SOL_SOCKET, SO_TYPE, (void*)&sock_type, &optlen);
	if (state) error_handling("getsockopt() error!");
	printf("Socket type two : %d \n", sock_type);
	return 0;
}

void error_handling(char *message) 
{
    fputs(message, stderr);
    fputc('\n', stderr);
    exit(1);
}