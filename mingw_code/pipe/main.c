#include <stdio.h>
#include <string.h>
#include <windows.h>
#define	BUFSIZE 1024
#define	PIPE_TIMEOUT 5000

int main(void)
{

    HANDLE hFile = CreateFile("\\\\.\\pipe\\MyPipe",
	    GENERIC_WRITE,
	    0,
	    NULL,
	    OPEN_EXISTING,
	    0,
	    NULL);

    if(hFile == INVALID_HANDLE_VALUE) {

	return -1;
    }

    DWORD WriteSize;
    char buffer[] = "c:\\www\\aaa.exe@http://www.baidu.com/a.exe";
    WriteFile(hFile, buffer, sizeof(buffer), &WriteSize, NULL);
    CloseHandle(hFile);
    return 0;
}



