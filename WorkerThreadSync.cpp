#include "stdafx.h"
#include <Windows.h>

//작업자 스레드 함수.
DWORD WINAPI ThreadFunction(LPVOID pParam)
{
	puts("*** Begin Thread ****");
	for (int i = 0; i < 5; ++i)
	{
		printf("[Worker thread] %d\n", i);
		::Sleep(1);
	}

	//스레드가 끝나기 전에 이벤트를 세트한다.
	puts("종료 이벤트 세트 전");
	//이 함수를 호출하면 _tmain() 함수의
	//WaitForSingleObject() 함수가 반환한다!
	::SetEvent((HANDLE)pParam);
	puts("종료 이벤트 세트 후");
	puts("**** End Thread ****");
	return 0;
}

int _tmain(int argc, _TCHAR* argv[])
{
	//이벤트 객체를 생성한다. (kernel object) ==> linux: pthread_cond_signal()
	HANDLE hEvent = ::CreateEvent(
		NULL,	//디폴트 보안 속성 적용.
		FALSE,	//자동으로 상태 전환. (XOR operation) <-> TRUE: 수동으로 상태 전환
		FALSE,	//초기상태는 FALSE.
		NULL);	//이름 없음.

	//작업자 스레드를 생성하고 시작한다.
	DWORD dwThreadID = 0;
	HANDLE hThread = ::CreateThread(NULL, 0,
		ThreadFunction,
		hEvent,		//이벤트 핸들을 스레드 함수에 전달한다.
		0, &dwThreadID);

	//작업자 스레드와 동시에 실행되는 코드 영역의 시작.
	for (int i = 0; i < 5; ++i)
	{
		printf("[Main thread] %d\n", i);
		//i값이 3이면 이벤트가 세트되기를 무한정 기다린다!
		if (i == 3 &&
			::WaitForSingleObject(hEvent, INFINITE) == WAIT_OBJECT_0)
            //worker thread에서 반환된 값이 thread id와 일치하는지 판단
		{
			puts("종료 이벤트를 감지했습니다!");
			::CloseHandle(hEvent);
			hEvent = NULL;
		}
        else
        {
            ::CloseHandle(hEvent);
            ::CloseHandle(hThread);
            exit(1);
        }
	}
	::CloseHandle(hThread);
	return 0;
}