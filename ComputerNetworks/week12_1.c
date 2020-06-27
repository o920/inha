#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <pthread.h>
void* thread_main(void* arg);

int main(int argc, char* argv[]) {
	pthread_t t_id;		//스레드 아이디 
	int thread_param = 5;
	void* thr_ret;		//join 리턴값을 받을 변수

	// 스레드 생성
	// 첫번째 인자는 스레드 id 주소 값
	// 두번째 인자는 NULL 기본적 속성의 스레드 호출
	// 세번째 인자는 함수의 주소가 전달됨 스레드의 실행 흐름은 이 함수부터 시작, 세번째 인자는 그 함수를 가리키는 포인터
	// 네번째 인자는 thread_main에 가져갈 인자
	if (pthread_create(&t_id, NULL, thread_main, (void*)&thread_param) != 0) {
		puts("pthread_create() error ");
		return -1;
	};
	
	// 이 스레드가 종료될때까지 대기 상태에 있겠다는 말 (?)
	if (pthread_join(t_id, &thr_ret) != 0) {
		puts("pthread_join() error");
		return -1;
	};
	//스레드가 잘 종료되면 리턴값을 출력하고
	printf("Thread return message : %s \n", (char*)thr_ret);
	free(thr_ret);	//스레드 리턴값을 동적할당 한 것을 해지해줌
	return 0;
}
void* thread_main(void* arg) {
	// 스레드의 실행 흐름이 되는 함수
	// 5를 받아서 1초씩 대기하게 됨 
	// 실행 중이라는 간단한 함수임
	// 스레드를 사용한 함수를 컴파일 할 때 -lpthread를 선언해서 별도로 지시를 해줘야함
	int i;
	int cnt = *((int*)arg);		// 5, 형변환 어떻게 되는 건지 확인하기!!
	char* msg = (char*)malloc(sizeof(char) * 50); //메세지 동적할당해줌 
	strcpy(msg, "Hello I am thread");
	for (i = 0; i < cnt; i++) { // 5번동안 반복함
		sleep(1);
		puts("running thread");
	}
	return (void*)msg;		//동적할당된 값을 반환해서 thr_ret 에 저장하려는 거임
}