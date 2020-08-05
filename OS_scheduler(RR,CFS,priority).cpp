#define _CRT_SEECURE_NO_WARNINGS
#include <iostream>
#include <queue>
#include <deque>
#include <stdio.h>
#define HAVE_STRUCT_TIMESPEC
#include <pthread.h>
#define MAX 20
using namespace std;

queue<int> roundrobin;	//Roundrobin�� ���� ť
queue<int> CFS;			//completely fair scheduling�� ���� ť
queue<int> SJF;			//short job first�� ���� ť

int cla[MAX] = {};
int pri[MAX] = {};		//���μ����� �켱����
int pid[MAX] = {};		//���μ����� id
int cpuburst[MAX] = {};	//cpu burst length

int c, id, p, b;
int a = 0;

void *RoundRobin(void *arg);
void *C_F_S(void* arg);
void *S_J_F(void* arg);

int main(int argc, char*argv[]) {
	freopen( "input.txt", "r", stdin);		//���Ϸ� �Է¹ޱ�
	freopen( "output.txt", "w", stdout);	//���Ϸ� ����ϱ�

	while (scanf("%d %d %d %d", &c, &id, &p, &b) != (int)EOF) {
		cla[a] = c;
		pri[a] = p;
		if (c == 0) roundrobin.push(a);
		if (c == 1) CFS.push(a);
		if (c == 2) SJF.push(a);
		pid[a] = id;
		cpuburst[a] = b;
		a++;
	}
	pthread_t t0, t1, t2;
	pthread_attr_t attr;
	pthread_attr_init(&attr);

	pthread_create(&t0, &attr, RoundRobin, NULL);
	pthread_join(t0, NULL);
	pthread_create(&t1, &attr, C_F_S, NULL);
	pthread_join(t1, NULL);
	pthread_create(&t2, &attr, S_J_F, NULL);
	pthread_join(t2, NULL);

	return 0;


}
void *RoundRobin(void* arg) {
	//roundrobin
	deque<int> round;				//timequantum ��ŭ ���� �� �� �ڿ��� ��ٸ��� ť
	int tq = 4;						//time quantum
	//round ť�� ���μ��� �ֱ�
	for (int j = 0; j < a; j++) {
		if (cla[j] != 0) continue;
		int min = 987654321;
		int min_idx = 0;
		for (int i = 0; i < a; i++) {
			if (cla[i] != 0) continue;
			if (pri[i] < min) {
				min = pri[i];
				min_idx = i;
			}
		}
		for (int i = 0; i < cpuburst[min_idx]; i++) 
			round.push_back(min_idx);
		pri[min_idx] = 987654321;
	}
	int idx = 0;
	while (round.size() > 0) {
		for (int i = 0; i < tq; i++) {						// timequantum ��ŭ �����ϰ� ť�� �ڷ� ������
			if (cpuburst[round.front()] > tq) {
				idx = round.front();
				for (int j = 0; j < tq; j++) {
					cout << pid[idx] << " ";
					round.pop_front();
				}
				cout << endl;
				cpuburst[idx] -= tq;
				for (int j = 0; j < cpuburst[idx]; j++) {
					round.push_back(idx);
					round.pop_front();
				}
				break;
			}
			else if (cpuburst[round.front()] == tq) {
				idx = round.front();
				for (int j = 0; j < tq; j++) {
					cout << pid[idx] << " ";
					round.pop_front();
				}
				cout << endl;
				break;
			}
			else if (cpuburst[round.front()] < tq) {
				idx = round.front();
				for (int j = 0; j < cpuburst[idx]; j++) {
					cout << pid[idx] << " ";
					round.pop_front();
				}
				cout << endl;
				break;
			}
		}
	}
	pthread_exit(0);
}

void *C_F_S(void* arg) {
	//CFS
	int vruntime[MAX] = { 0, };
	int weight[MAX] = { 0, };
	int delta_exec[MAX] = { 0, };
	int ready[MAX] = { 0, };
	bool ready_check[MAX] = { 0, };
	bool check[MAX] = { 0, }; // �ߺ�Ȯ��

	for (int i = 0; i < a; i++) {
		if (cla[i] != 1)continue;
		if (pri[i] == 7) weight[i] = 12;
		if (pri[i] == 8) weight[i] = 10;
		if (pri[i] == 9) weight[i] = 8;
		if (pri[i] == 10) weight[i] = 6;
		if (pri[i] == 11) weight[i] = 4;
		if (pri[i] == 12) weight[i] = 2;
		ready[i] = 0;
	}
	int prior = 0;
	while (1) {
		//�ʱ�ȭ
		for (int i = 0; i < a; i++) {
			check[i] = 0;
			ready_check[i] = 0;
		}
		//vruntime ������Ʈ
		for (int i = 0; i < a; i++) {
			if (cla[i] != 1)continue;
			vruntime[i] += (12 / weight[i]) * delta_exec[i];
		}

		//vruntime �ּҰ� ã��
		int vrunmin = 987654321;
		int vrunmin_idx = 0;
		for (int i = 0; i < a; i++) {
			if (cla[i] != 1)continue;
			if (cpuburst[i] == 0)continue;
			if (vruntime[i]<=vrunmin) {
				vrunmin_idx = i;
				vrunmin = vruntime[i];
			}
		}
		// vruntime ���� �ߺ��� ��� ã��
		int count = 0;
		for (int i = 0; i < a; i++) {
			if (cla[i] != 1)continue;
			if (cpuburst[i] == 0)continue;
			if (vruntime[i] == vrunmin) {
				count++;
				check[i] = true;
			}
		}
		
		// vruntime ���� �ߺ��� �ִ� ���
		if (count > 1) {
			//readyȮ�� -> priority Ȯ��
			int readymax = 0;		// ready �ִ� ã��
			int readymax_idx = 0;	// �׶��� �ε���
			int ready_count = 0;	// ready�� �ߺ��� �ִ��� Ȯ���ϱ�
			//ready �ִ� ã��
			for (int i = 0; i < a; i++) {
				if (cla[i] != 1)continue;
				if (cpuburst[i] == 0)continue;
				if (check[i] == false)continue;
				if (ready[i] > readymax) {
					readymax = ready[i];
					readymax_idx = i;
				}
			}
			//ready �ߺ��� ã��
			for (int i = 0; i < a; i++) {
				if (cla[i] != 1)continue;
				if (cpuburst[i] == 0)continue;
				if (ready[i] == readymax) {
					ready_count++;
					ready_check[i] = true;
				}
			}
			//ready �ߺ��� �ִ� ��� -> priority�� ���� �������μ��� ����
			if (ready_count > 1) {
				int primin = 12;
				int primin_idx = 0;
				for (int i = 0; i < a; i++) {
					if (cla[i] != 1)continue;
					if (cpuburst[i] == 0) continue;
					if (check[i] == false) continue;
					if (ready_check[i] == false) continue;
					if (pri[i] < primin) {
						primin = pri[i];
						primin_idx = i;
					}
				}
				if (prior != readymax_idx) cout << endl;
				cout << pid[primin_idx] << " ";
				prior = primin_idx;
				delta_exec[primin_idx]++;
				cpuburst[primin_idx]--;
				for (int i = 0; i < a; i++) {
					if (cla[i] != 1)continue;
					if (i == primin_idx) continue;
					ready[i]++;
				}
			}
			//ready �ߺ��� ���� ���, ready ���� ū ���μ��� ����
			else {
				if (prior != readymax_idx) cout << endl;
				cout << pid[readymax_idx] << " ";
				prior = readymax_idx;
				delta_exec[readymax_idx]++;
				cpuburst[readymax_idx]--;
				for (int i = 0; i < a; i++) {
					if (cla[i] != 1)continue;
					if (i == readymax_idx) continue;
					ready[i]++;
				}
			}

		}
		// vruntime ���� �ߺ��� ���� ���
		else {
			if (prior != vrunmin_idx) cout << endl;
			cout << pid[vrunmin_idx] <<" ";
			prior = vrunmin_idx;
			delta_exec[vrunmin_idx]++;
			cpuburst[vrunmin_idx]--;
			for (int i = 0; i < a; i++) {
				if (cla[i] != 1)continue;
				if (i == vrunmin_idx) continue;
				ready[i]++;
			}
		}
		int rest = 0;
		for (int i = 0; i < a; i++) {
			if (cla[i] != 1)continue;
			rest += cpuburst[i];
		}
		if (rest == 0) break;
		
	}
	cout << endl;
	pthread_exit(0);
}

void *S_J_F(void* arg) {
	pair<int, int> s[MAX];
	for (int i = 0; i < a; i++) {
		if (cla[i] != 2) continue;
		s[i].first = i;
		s[i].second = cpuburst[i];
	}
	while (1) {
		//�ּ� cpu burst ã��
		int min = 987654321;
		int min_idx = 0;
		for (int i = 0; i < a; i++) {
			if (cla[i] != 2) continue;
			if (cpuburst[i] == 0) continue;
			if (s[i].second < min) {
				min = s[i].second;
				min_idx = s[i].first;
			}
		}
		for (int i = 0; i < cpuburst[min_idx]; i++) { cout << pid[min_idx] << " "; }
		cpuburst[min_idx] = 0;
		cout << endl;
		
		int rest = 0;
		for (int i = 0; i < a; i++) {
			if (cla[i] != 2) continue;
			rest += cpuburst[i];
		}
		if (rest == 0) break;
	}
	pthread_exit(0);
}
