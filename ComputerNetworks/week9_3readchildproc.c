#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
#include <signal.h>
#include <sys/wait.h>

//복습
// 자식 프로세스 종료 발생 시 부모가 시그차일드를 받고 호출하는 함수로
// 내부 waitpid가 블로킹이 발생하지 않게 사용되는 그런 함수
void read_childproc(int sig) {
	int status;									// 
	pid_t id = waitpid(-1, &status, WNOHANG);	// 임의의 자식의 종료 확인
	if (WIFEXITED(status)) {	
		//wifexited 매크로  함수 : 인자 status 잘 종료됐는지 종료됐다면 반환한 값은 뭔지 가져오는 함수
		// 자식 프로세스가 좀비가 되지 않게 함
		printf("Removed porc id : %d \n", id);
		printf("Child send : %d \n", WEXITSTATUS(status));
	}
}

int main(int argc, char* argv[]) {
	pid_t pid;
	struct sigaction act;	//시그널 등록시 ㅇ ㅓ떤 함수나 어떤 시그널을 등록할지 저장함
	act.sa_handler = read_childproc;		//자식 프로세스가 좀비가 되지 않게하는 함수 등록
	sigemptyset(&act, sa_mask);				// 0으로 다 초기하기 위해 사용한 함수
	act.sa_flags = 0;
	sigaction(SIGCHILD, &act, 0);				// 시그차일드에 대한 시그함수를 등록
	// 자식프로세스 종료되면 read_childproc이 호출될 거임

	pid = fork();								// 자식 프로세스 생성
	if (pid == 0) {
		puts("Hi! I am a child process");		// 자식 프로세스가 출력하고 10초 대기 후 12반환하고 종료
		sleep(10);
		return 12;
	}
	else {
		printf("Chile proc id : %d\n", pid);	// 첫번째 자식 프로세스 종료하고 두번째 자식 프로세스 생성
		pid = fork();	
		if (pid == 0) {
			puts("Hi! I am a chilf process");	// 문자열 출력하고 10초 대기 후 24를 반환하고 종료
			sleep(10);
			exit(24);
		}
		else {
			printf("Child proc id : %d\n", pid);// 두번째 자식 pid 출력하고 5초 대기를 5번 반복
			for (int i = 0; i < 5; i++) {		// 첫번째 두번째 자식이 종료하면서 반환한 값을 대기함
				puts("wait...");				// 각각의 문자열 출력, 부모가 5초 대기하는 동안 자식이 종료될 거임
				sleep(5);						// 시그널 발생하면 시그널 함수 호출하고 자식을 잘 종료시키고 있는 걸 볼 수 있음	
			}									
		}
	}
	return 0;
}
// 우분투에서 실행하기