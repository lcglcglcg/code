#include <stdio.h>
#include <windows.h>
#include <winioctl.h>

int main (void)
{
    HANDLE hDevice = CreateFile("\\\\.\\MyEvent",
	    GENERIC_READ | GENERIC_WRITE,
	    0,
	    NULL,
	    OPEN_EXISTING,
	    FILE_ATTRIBUTE_NORMAL,
	    NULL);

    if(hDevice == INVALID_HANDLE_VALUE) {

	printf("CreateFile error!\n");
	return 0;
    }

    ULONG RetCode;
    DeviceIoControl(hDevice,
	    1584,
	    NULL,
	    0,
	    NULL,
	    0,
	    &RetCode,
	    NULL);

    CloseHandle(hDevice);
    return 0;
}
