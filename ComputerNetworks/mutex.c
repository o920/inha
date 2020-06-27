#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <pthread.h>
#define NUM_THREAD 100
void* thread_inc(void* arg);
void* thread_des(void* arg);

long long num = 0;
pthread_mutex_t mutex;		// OS가 생성한 뮤텍스 참조에 사용하는 변수 선언

int main(int argc, char* argv[]) {
	pthread_t thread_id[NUM_THREAD];
	int i;
	pthread_mutex_init(&mutex, NULL);	//뮤텍스 생성 init 함수
	for (i = 0; i < NUM_THREAD; i++) {
		if (i % 2) pthread_create(&(thread_id[i]), NULL, thread_inc, NULL);
		else pthread_create(&(thread_id[i]), NULL, thread_des, NULL);
	}
	for (i = 0; i < NUM_THREAD; i++) pthread_join(thread_id[i], NULL);
	printf("result : %lld \n", num);
	pthread_mutex_destroy(&mutex);
	return 0;
}

void* thread_inc(void* arg) {
	int i;
	pthread_mutex_lock(&mutex);		// 들어가기 전에 lock 함수 호출
	//한 스레드가 이 뮤텍스로 잠그면 다른 스레드는 임계영역에 접근하지 못하고 대기 상태에 놓임
	for (i = 0; i < 50000000; i++) num += 1;
	pthread_mutex_unlock(&mutex);	// 뮤텍스 좌물쇠를 염
	// 대기 중인 다른 스레드가 임계영역에 접근 가능
	// unlock 안해주면 다른 스레드가 lock 함수에 걸려서 임계영역을 못 들어가게 됨
	return NULL;
}
void* thread_des(void* arg) {
	int i;
	// 반복문에 뮤텍스가 들어가 있음
	for (i = 0; i < 50000000; i++) {
		pthread_mutex_lock(&mutex);
		num -= 1;
		pthread_mutex_unlock(&mutex);
	}
	//결과는 차이 없음
	//inc는 임계영역을 넓게 잡고, des는 num 변환하는 부분만 좁게 잡음
	return NULL;
}
// 코드를 inc하고 compile하는게 좋을 거임 왜냐면 des처럼 하면 개오래걸림 나 아직도 기다리는 중
// lock과 unlock 호출을 최소화 하는게 성능에 유리함


