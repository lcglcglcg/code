
all:
	gcc main.c -o app.exe -Wall
	gcc -O3 -c mydev.c -o mydev.obj 
	ld mydev.obj --subsystem=native --image-base=0x10000 --file-alignment=0x20 --section-alignment=0x20 --entry=_DriverEntry@8 -nostartfiles --nostdlib -shared libntoskrnl.a -o mydev.sys
	
clean:
	rm -f app.exe
	rm -f mydev.obj
	rm -f mydev.sys

start:
	copy mydev.sys c:\windows\system32\drivers
	net start mydev
stop:
	net stop mydev
