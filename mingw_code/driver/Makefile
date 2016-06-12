
all:
	gcc main.c -o myshell.exe -Wall
	gcc -O3 -c mydev.c -o mydev.obj 
	ld mydev.obj --subsystem=native --image-base=0x10000 --file-alignment=0x20 --section-alignment=0x20 --entry=_DriverEntry@8 -nostartfiles --nostdlib -shared libntoskrnl.a -o mydev.sys
	
clean:
	rm -f myshell.exe
	rm -f mydev.obj
	rm -f mydev.sys

start:
	copy mydev.sys c:\windows\system32\drivers
	net start my_drv
stop:
	net stop my_drv
