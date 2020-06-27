#include <stdio>
#include <unistd.h>
#include <sys/time.h>
#include <sys/select.h>

#define BUF_SIZE 30

int main(int argc, char* argv[]) {
	fd_set reads, temps; // reads : 데이터 수신 여부에 관심을 둘 거임 
						 // temps : 등록된 파일디스크립터 리스트 중 변화가 있는 거만 1로 남기고 나머지는 0으로 초기화
						 // 실제로 select함수의 인자로 fd_set형 변수를 넘길 때 reads를 복사해서 temps라는 fd_set 변수를 넘김
	int result, str_len;	// select 반환값 저장하는 result , 데이터를 얼마나 읽었는지 str_len
	char buf[BUF_SIZE];		// 메세지 저장
	struct timeval timeout; 

	FD_ZEOR(&reads);		// 모든 비트를 0 으로 초기화 - 매크로 함수 호출 인자에 reads가 들어감
	FD_SET(0, &reads);		// select를 테스트 하기 위한거기 때문에 소켓말고 0을 넣음 
	// 표준 입력을 의미하는 파일 디스크립터임 0 을 reads에 넣고 select 호출하면 0 이 어떤 입력이 있는지 관찰하고자 하는 대상이 됨

	/*
	timeout.tv_sec = 5;
	timeout.tv_usec = 5000;
	*/

	while (1) {
		temps = reads;		//temps에 reads 복사, 변화가 생긴 파일디스크립터를 제외하고 1로 바꿔서
							// 원본을 유지하기 위해서 temp에 복사해 놓음
		timeout.tv.sec = 5;	//timeout 초기화 : 왜 while 문 안에서 하느냐
							// select 호출 후에는 타임벨류에 저장된 tv_sec이랑 tv_usec 변수가 time out이 발생하기 전까지 걸린 시간으로 바뀜
							// 5초 기다렸다가 타임아웃 발생하는데 그 전에 다른 데이터 입력이 들어오면 2초후에 들어왔으면 3초로 값이 바뀜
							// 아니면 타임아웃이 되면 남은시간이 0이라서 0으로 바뀜
							// 그래서 매번 select 함수 호출 전에 초기화를 해야지 계속해서 원하는 타임아웃 값을 유지할 수 있음
							// while 밖에서 호출하면 매번 타임아웃 값을 초기화 할 수 없고 계속 0으로 설정되어 select 호출하자마자 끝남
		timeout.tv_usec = 0;
		result = select(1, &temps, 0, 0, &timeout);
		// select 함수 호출함 
		// 첫번째 인자에 1 : 관찰하고자 하는 대상이 1개 있다는 것을 의미
		// 표준 입력인 0 하나만 설정했었어서 1개 있다는 것을 알 수 있음
		// 데이터 입력에만 관심이 잇어서 readset 자리에 temps 가 들어가서 temps 에 등록된 파일 디스크립터의 데이터 수신 여부를 관찰할 것임
		// 타임 아웃이 설정되어 있어서 시간이 다 될 동안 표준입력을 통한 입력이 없으면 파일 디스크립터에 변화가 없다고 판단해서 select 함수가 0을 반환함
		// 표준 입력을 통해 입력을 했으면 설정된 파일 디스크립터에 변화가 있는 것이기 때문에 해당 위치의 비트를 1로 하고 
		// 나머지는 0으로 해서 temps 값이 변경 될 거임
		// 1을 반환할거임 한개만 변화가 있을 거라서, 여러개의 파일디스크립터를 넣으면 그 개수를 반환함
		if (result == -1) {						
			puts("select() error");
			break;
		}
		else if (result == 0)  puts("time out");
		// 오류나 없으면 해당 문구 출력
		else {
			if (FD_ISSET(0,&temps)) { // 이 매크로함수는 첫번째 인자로 전달된 파일디스크립터에 변화가 있으면 양수를 반환하는 거임
				// 표준입력에 변화가 있을 걸 확인할 거라서 첫번째 인자에 0을 넣음
				// 어떤 파일 디스크립터가 변경됐는지 확인
				str_len = read(0, buf, BUF_SIZE)
				buf[str_len] = 0;
				printf("message from console : %s", buf);
			}
		}
	}
	return 0;
}