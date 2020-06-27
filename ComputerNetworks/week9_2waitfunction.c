#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
#include <sys/wait.h>

int main(int argc, char* argv[]) {
	int status;				//wait 함수에 인자로 들어갈 자식 종료 상태나 반환 값을 담는 변수
	pid_t pid = fork();		// 첫번째 자식 생성, 부모 프로세스와 다른 프로세스가 되어서 코드 진행
	if (pid == 0) return 3; // 첫번째 자식은 3을 반환하고 종료
	else {
		printf("Child PID : %d \n", pid);		//첫번째 자식 아이디 출력
		pid = fork();							//두번째 자식 생성
		if (pid == 0) exit(7);					// 오에스로 7을 전달하고 종료
		else {
			printf("child pid : %d \n", pid);	// 두번째 자식 아이디 출력
			wait(&status);						// 종료된 자식이 있는지 확인 두개 자식 중 하나의 값이 전달 될 거임, 
			if (WIFEXITED(status))				// 자식이 정상적으로 종료됐는지 묻는 함수
				printf("Child send one : %d \n", WEXITSTATUS(status));
			wait(&status);						// 앞서 확인한 값 말고 다른 자식 프로세스가 종료하면서 전달한 값 저장
			if (WIFEXITED(status))				// 정상적으로 종료가 됐으면 전달받은 값을 확인하여 출력
				printf("Child send two : %d \n", WEXITSTATUS(status));
			sleep(30);
		}
	}
	return 0;
}
// 시그윈에서 돌아감
// 시그윈에서 여러번하면 첫번째 자식이 전달하는게 3이고 
// 두번째 자식은 7을 전달함 
// 프로세스 실행 순서는 운영체제가 정해서
// 3이 먼저 나올 수도 있고 7이 먼저 나올 수도 있음