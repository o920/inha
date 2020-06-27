// 여러 스레드가 프로세스로부터 받은 작업하고 출력함
// 앞서 말한 실습에서 두 스레드가 모두 1~5, 6~10 더함
// 이번에는 100개 스레드가 50개로 나뉘고 num을 오천만번씩 1 증 또는 감소 시키는 확장된 실험임
// 이 실행 결과는 처음에 num이 0부터시작해서 1씩 증가 감소해서 0이 나와야 함
#include <stdio.h>
#include <unistd.h>
#include <stdlib.h>
#include <pthread.h>
#define NUM_THREAD 100		//스레드의 갯수 100개

void* thread_inc(void* arg);	
void* thread_des(void* arg);
long long num = 0;			

int main(int argc, char* argv[]) {
	pthread_t thread_id[NUM_THREAD];	//스레드 아이디를 관리할 저장 장소
	int i;

	printf("size of long long : %d\n", sizeof(long long));			//8byte 64bit의 정수형 자료형
	for (i = 0; i < NUM_THREAD; i++) {	//100번 반복하면서 스레드가 100개 생성됨
		if(i%2) pthread_create(&(thread_id[i]), NULL, thread_inc, NULL);	
		else pthread_create(&thread_id[i], NULL, thread_des, NULL);
	}

	for (i = 0; i < NUM_THREAD; i++)
		pthread_join(thread_id[i], NULL);
	
	printf("result : %d\n", num);
	return 0;
}

void* thread_inc(void* arg) {
	int i;
	for (i = 0; i < 50000000; i++) num += 1; //임계영역
	return NULL;
}

void* thread_des(void* arg) {
	int i;
	for (i = 0; i < 50000000; i++) num -= 1; //임계영역
	return NULL;
}
// 오래 걸림 많은 연산을 해서
// 예상과 다른 이유
// 100개의 스레드가 각 작업 수행하면서 공유 변수 num에 동시접근함
// 접근한다는 거는 값 증가 또는 감소를 위한 것
// 문맥전환하면 실제로는 스레드가 cpu 시간을 나눠서 사용하는거임
// 이거 오에스때 존나 배움 ㅎㅋㅎㅋㅎㅋㅎㅋㅎㅋㅎㅋㅎㅋㅎㅋㅋ
