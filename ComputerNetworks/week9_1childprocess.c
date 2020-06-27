#include <stdio.h>
#include <unistd.h>

int main(int argc, char* argv[]) {
	pid_t pid = fork();
	if (pid == 0) puts("Hi I am a chile process");	// 자식프로세스 아이드가 0 인거는 자식프로세스는 0을 반환 받았다는 거임
	else { 
		printf("Child Process ID : %d \n", pid);	// pid는 자식프로세스의 프로세스 아이디
		sleep(30);	//30초의 대기시간
	}
	if (pid == 0) puts("End child process"); // 종료한다고 출력
	else puts("End parent process"); // 부모는 아직 30초간 대기 중, 이후 출력
} 
// 리눅스 계열에서 확인해야함
// 우분투에서는 defunct를 확인할 수 있음 
// 좀비프로세스가 되어있다는 것을 확인할 수 있음
