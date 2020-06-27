// chap4에 에코클라이언트랑 실행하기
#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
#include <sys/wait.h>
#include <string.h>
#include <signal.h>
#include <arpa/inet.h>
#include <sys/socket.h>

#define BUF_SIZE 100
void error_handling(char* message);

int main(int argc, char* argv[]) {
	int serv_sock, clnt_sock;
	struct sockaddr_in serv_adr, clnt_adr;
	struct timeval timeout;
	fd_set reads, cpy_reads;	//cpy_reads가 reads를 복사해서 select의 인자로 들어갈거임

	socklen_t adr_sz;
	int fd_max, str_len, fd_num, i;
	// fd_max+1을해서 넘기면 파일디스크립터는 0 부터 시작해서 몇개의 파일디스크립터가 있는지 바로 확인할 수 있음
	// fd_num은 select 함수 반환 값을 저장할 변수임
	// i가 이후의 파일디스크립터 처럼 사용될 거임
	char buf[BUF_SIZE];
	if (argc != 2) {
		printf("Usage : %s \n", argv[0]);
		exit(1);
	}

	serv_sock = socket(PF_INET, SOCK_STREAM, 0); // tcp 소켓 생성
	memset(&serv_adr, 0, sizeof(serv_adr)); // 초기화 과정
	serv_adr.sin_family = AF_INET;
	serv_adr.sin_addr.s_addr = htonl(INADDR_ANY);
	serv_adr.sin_port = htons(atoi(argv[1]));

	// 서버 소켓에 주소를 할당하는 중
	if (bind(serv_sock, (struct sockaddr*) & serv_adr, sizeof(serv_adr)) == -1)
		error_handling("bind error");
	// 연결 요청 가능한 상태로 서버 소켓을 만들어 줌
	if (listen(serv_sock, 5) == -1)
		error_handling("listen error");

	FD_ZERO(&reads);			// 초기화
	FD_SET(serv_sock, &reads);	// 관찰 대상 서버 소켓만 넣어줌 
	// 연결요청을 받는 서버 소켓도 데이터를 수신하는 거임 데이터 수신 관련 reads에 설정
	// 표준 입출력을 사용 중이라서 0 1 2 3 값을 파일디스크립터로 사용할 거임
	fd_max = serv_sock;		// 지금 초반의 값은 3이 될 거임

	while (1) {
		cpy_reads = reads;
		timeout.tv_sec = 5;
		timeout.tv_usec = 5000;

		if ((fd_num = select(fd_max + 1, &cpy_reads, 0, 0, &timeout)) == -1) break;
		//select 함수 호출 : 첫번째 인자 파일디스크립터의 갯수 fd_max+1
		// 두번째 인자는 관심대상인 cpy_reads
		// 쓰는 거랑 어쩌구는 관심없어서 0 입력
		// 마지막으로 초기화한 타임아웃 구조체를 넘김
		// 만약 이 select 가 실패하면 break 해서 끝냄
		if (fd_num == 0) continue;		// 타임아웃이면 select는 0 반환함 
		// 다시 반복문 처음으로 돌아가서 cpy_reads가 다 0으로 초기화 됐을 거니까 다시 갱신해줌
		for (i = 0; i < fd_max + 1; i++) {
			// 파일디스크립터 개수만큼 반복하면서 
			if (FD_ISSET(i, &cpy_reads)) {	// 변화가 있었는지 확인함 , i가 파일디스크립터 의미
				if (i == serv_sock) {		// server socket의 연결요청이 들어왔다면 값이 1로 바뀌고 나머지는 0 일 거임
					//서버 소켓에서 이후에 변형된 값이 서버소켓이었다면 accept 함수로 연결 요청 수락하고 새로운 소켓을 만듦
					// 새로운 소켓은 새로운 파일디스크립터로 연결되어 있을 거고 새롭게 데이터 수신 받을 소켓 만들었으니까 그 파일디스크립터를 fd_set으로 관찰대상에 넣음
					adr_sz = sizeof(clnt_adr);
					clnt_sock = accept(serv_sock, (struct sockaddr*) & clnt_adr, &adr_sz);
					FD_SET(clnt_sock, &reads);// 여기가 새로운 파일디스크립터를 관찰대상에 넣는거임 
					if (fd_max < clnt_sock) fd_max = clnt_sock; // 지금이 첫 데이터 수신이면 4가 될거임 (지금 값이 3이라서)  그래서 fd_max 새롭게 갱신
					printf("connected client : %d \n", clnt_sock);
				}
				else {	// 어떤 파일디스크립터에 변화가 있을때 그게 서버소켓인지 물어보는 조건문에 해당하는 else
					// 변경된 파일디스크립터가 있었지만 서버소켓이 아니었다면
					// 그 소켓의 클라이언트로부터 데이터가 수신이 되었다는 것을 의미
					str_len = read(i, buf, BUF_SIZE); // i는 cpy_reads 내의 파일디스크립터 의미
					if (str_len == 0) {
						// 데이터 없으면 데이터 송수신을 끝낸거임
						// 해당 파일디스크립터를 reads 관찰대상에서 제거함
						FD_CLR(i, &reads);
						close(i); // 해당 소켓을 닫음
						printf("closed client : %d \n", i); // 연결종료 안내문 출력
					}
					else write(i, buf, str_len);	// 데이터가 있었으면 echo해줌
				}
			}
		}
	}
	close(serv_sock);
	return 0;
}

void error_handling(char* buf) {
	fputs(buf, stderr);
	fputc('\n', stderr);
	exit(1);
}