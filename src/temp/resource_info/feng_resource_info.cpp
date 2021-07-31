#define _CRT_SECURE_NO_WARNINGS
#include <string>
#include <windows.h>
#include <stdint.h>
#include <tlhelp32.h>
#include <iostream>
#include <vector>
#include <time.h>
#include <fstream>
#pragma comment( linker, "/subsystem:windows /entry:mainCRTStartup" )
using namespace std;

//»ñµÃ¿ÉÓÃÎïÀíÄÚ´æ
DWORD getWin_MemUsage(){
    MEMORYSTATUS ms;
    ::GlobalMemoryStatus(&ms);
    return ms.dwAvailPhys;
}

//»ñµÃÈ«²¿ÎïÀíÄÚ´æ
DWORD getWin_MemUsage_all(){
    MEMORYSTATUS ms;
    ::GlobalMemoryStatus(&ms);
    return ms.dwTotalPhys;
}

//»ñµÃÄÚ´æÊ¹ÓÃÂÊ
DWORD getWin_MemUsage_shiyonglv(){
    MEMORYSTATUS ms;
    ::GlobalMemoryStatus(&ms);
    return ms.dwMemoryLoad;
}


int main()
{
    HANDLE hEvent;
    hEvent = CreateEvent(NULL, FALSE, FALSE, NULL);

    while (1){
        WaitForSingleObject( hEvent,1000 );  //µÈ´ý500ms
        DWORD a = getWin_MemUsage_shiyonglv();
        cout << "ÄÚ´æÕ¼ÓÃÂÊ:" << a << "%" << endl;
    }

    return 0;
}