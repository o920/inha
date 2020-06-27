#include <stdio.h>
#include <pthread.h>
void* thread_summation(void* arg);	// 스레드 메인함수 역할
int sum = 0;						// 공유된 메모리로 사용될 거임
int main(int argc, char* argv[]) {
	pthread_t id_t1, id_t2;
	int range1[] = { 1,5 };
	int range2[] = { 6,10 };

	pthread_create(&id_t1, NULL, thread_summation, (void*)range1);
	pthread_create(&id_t2, NULL, thread_summation, (void*)range2);

	// 각 스레드가 종료될때까지 대기 상태
	// 스레드1이 종료상태까지 기다렸다가 리턴하고, 그때 join 호출
	// 스레드 1이랑 2는 거의 비슷하게 생성되고 1,2가 같이 작업하고 있음
	pthread_join(id_t1, NULL);
	pthread_join(id_t2, NULL);
	printf("result : %d\n", sum);
	return 0;
}

void* thread_summation(void* arg) {
	int start = ((int*)arg)[0];	//이거도 형변환 봐야할 듯
	int end = ((int*)arg)[1];

	while (start <= end) {
		sum += start;
		start++;
	}
	return NULL;
}