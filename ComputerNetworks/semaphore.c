#include <stdio.h>
#include <pthread.h>
#include <semaphore.h>

void* read(void* arg);
void* accu(void* arg);
static sem_t sem_one;
static sem_t sem_two;
static int num;

int main(int argc, char* argv[]) {
	pthread_t id_t1, id_t2;
	sem_init(&sem_one, 0, 0);	// 세마포어 초기값 0
	sem_init(&sem_two, 0, 1);	// 세마포어 초기값 1
	pthread_create(&id_t1, NULL, read, NULL);
	pthread_create(&id_t2, NULL, accu, NULL);
	pthread_join(id_t1, NULL);
	pthread_join(id_t2, NULL);
	sem_destroy(&sem_one);
	sem_destroy(&sem_two);
	return 0;
}

void* read(void* arg) {
	int i;
	for (i = 0; i < 5; i++) {
		fputs("input num : ", stdout);
		sem_wait(&sem_two);		//공유 메모리 num 접근하기 전에 sem_wait 해서 두번째 세마포어값 -1
								//세마 포어 값이 0이 됨
		scanf("%d", &num);
		sem_post(&sem_one);		// 첫번째 세마포어 값을 1 증가시킴, 생성 시  초기 값 0 이라서 1이 됨
	}
	return NULL;
}
void* accu(void* arg) {
	int sum = 0, i;
	for (i = 0; i < 5; i++) {
		sem_wait(&sem_one);		// 첫번째 세마포어 값을 1 감소시킴
								// 초기 값은 0 이었음, 처음 도착한 스레드는 lock 걸림, post해서 1이 되면 임계영역 접근
		sum += num;
		sem_post(&sem_two);		// 두번째 세마포어 값을 1로 증가시킴
								// 먼저 0으로 바꿨었음, 이걸 1로 바꿈
	}
	printf("Result : %d \n", sum);
	return NULL;
}