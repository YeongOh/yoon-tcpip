#include <stdio.h>
#include <windows.h>
#include <process.h>

#define NUM_THREAD 50

long long num = 0;
CRITICAL_SECTION cs;

unsigned WINAPI ThreadInc(void* arg);
unsigned WINAPI ThreadDec(void* arg);

int main(int argc, char* argv[]) {
	HANDLE tHandles[NUM_THREAD];
	int i;

	printf("size long long: %d\n", sizeof(long long));

	InitializeCriticalSection(&cs);

	for (i = 0; i < NUM_THREAD; i++) {
		if (i % 2)
			tHandles[i] = (HANDLE)_beginthreadex(NULL, 0, ThreadInc, NULL, 0, NULL);
		else
			tHandles[i] = (HANDLE)_beginthreadex(NULL, 0, ThreadDec, NULL, 0, NULL);
	}

	WaitForMultipleObjects(NUM_THREAD, tHandles, TRUE, INFINITE);

	DeleteCriticalSection(&cs);

	printf("result: %lld\n", num);
	return 0;
}

unsigned WINAPI ThreadInc(void* arg) {
	int i;
	EnterCriticalSection(&cs);
	for (i = 0; i < 50000000; i++) {
		num++;
	}
	LeaveCriticalSection(&cs);
	return 0;
}

unsigned WINAPI ThreadDec(void* arg) {
	int i;
	EnterCriticalSection(&cs);
	for (i = 0; i < 50000000; i++) {
		num--;
	}
	LeaveCriticalSection(&cs);
	return 0;
}