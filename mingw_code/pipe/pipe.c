#include <stdio.h>
#include <windows.h>
#define	BUFSIZE 1024
#define	PIPE_TIMEOUT 5000

int main(void)
{
    HANDLE hPipe = CreateNamedPipe("\\\\.\\pipe\\MyPipe",
	    PIPE_ACCESS_DUPLEX,
	    PIPE_TYPE_MESSAGE | PIPE_READMODE_MESSAGE | PIPE_WAIT,
	    PIPE_UNLIMITED_INSTANCES,
	    BUFSIZE,
	    BUFSIZE,
	    PIPE_TIMEOUT,
	    NULL);

    if (hPipe == INVALID_HANDLE_VALUE) {

	return -1;
    }

    for (;;) {

	if (ConnectNamedPipe(hPipe, NULL) || (GetLastError() == ERROR_PIPE_CONNECTED)) {

	    DWORD ReadSize;
	    char buffer[BUFSIZE] = {0};
	    int retval = ReadFile(hPipe, buffer, BUFSIZE, &ReadSize, NULL);
	    if (!retval || ReadSize == 0)
		break;

	    DisconnectNamedPipe(hPipe);
	    fprintf(stdout, "%s\n",buffer);

	} else {

	    CloseHandle(hPipe);
	    return -1;
	}
    }

    CloseHandle(hPipe);
    return 0;
}

