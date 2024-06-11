#include <stdio.h>
#include <windows.h>
#include <process.h>

#define NUM_THREAD 50

long long num = 0;
HANDLE hMutex;

unsigned WINAPI ThreadInc(void* arg);
unsigned WINAPI ThreadDec(void* arg);

int main(int argc, char* argv[]) {
	HANDLE tHandles[NUM_THREAD];
	int i;

	hMutex = CreateMutex(NULL, FALSE, NULL);

	for (i = 0; i < NUM_THREAD; i++) {
		if (i % 2)
			tHandles[i] = (HANDLE)_beginthreadex(NULL, 0, ThreadInc, NULL, 0, NULL);
		else
			tHandles[i] = (HANDLE)_beginthreadex(NULL, 0, ThreadDec, NULL, 0, NULL);
	}

	WaitForMultipleObjects(NUM_THREAD, tHandles, TRUE, INFINITE);

	CloseHandle(hMutex);

	printf("result: %lld\n", num);
	return 0;
}

unsigned WINAPI ThreadInc(void* arg) {
	int i;
	WaitForSingleObject(hMutex, INFINITE);
	for (i = 0; i < 50000000; i++) {
		num++;
	}
	ReleaseMutex(hMutex);
	return 0;
}

unsigned WINAPI ThreadDec(void* arg) {
	int i;
	WaitForSingleObject(hMutex, INFINITE);
	for (i = 0; i < 50000000; i++) {
		num--;
	}
	ReleaseMutex(hMutex);
	return 0;
}