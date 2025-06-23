#include <Windows.h>

DWORD WINAPI ThreadMain(LPVOID lpArgs)
{
	volatile double x = 1.0;
	while (true) {
		x = x * x + 1.1 * 91.452814;
		malloc(1024 * 1024);
	}
}

int WINAPI wWinMain(HINSTANCE hInstance, HINSTANCE hPrevInstance, PWSTR pCmdLine, int nCmdShow)
{
	DWORD ProcessorsCount;
	{
		SYSTEM_INFO SystemInfo{};
		GetSystemInfo(&SystemInfo);
		ProcessorsCount = SystemInfo.dwNumberOfProcessors;

	}

	HANDLE CurProc = GetCurrentProcess();
	if (!SetPriorityClass(CurProc, REALTIME_PRIORITY_CLASS)) return 1;

	HANDLE* hThreads = new HANDLE[ProcessorsCount];

	for (short i = 0; i < ProcessorsCount; i++)
	{
		hThreads[i] = CreateThread(NULL, NULL, &ThreadMain, NULL, CREATE_SUSPENDED, NULL);
		SetThreadAffinityMask(hThreads[i], 1ULL << i);
		if (!SetThreadPriority(hThreads[i], THREAD_PRIORITY_TIME_CRITICAL)) return 2;
	}
	for (short i = 0; i < ProcessorsCount; i++) ResumeThread(hThreads[i]);

	volatile double x = 1.0;
	while (true) {
		x = x * x + 1.1 * 91.452814;
		malloc(1024 * 1024);
	}
}