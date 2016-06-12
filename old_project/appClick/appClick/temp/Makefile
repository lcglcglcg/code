
CFLAG += -luuid 
CFLAG += -lole32 
CFLAG += -loleaut32 
CFLAG += -lcomdlg32 
CFLAG += -lgdi32
	
all:
	windres -i ./ico/ico.rc -o ./ico/ico.o --input-format=rc
	g++ main.cpp ./ico/ico.o -o appClick.exe $(CFLAG)
	gcc manager_dll.c -o appManager.dll $(CFLAG) -shared
	gcc json*.c network.c -o appClick.exe -lwsock32 -Wall -g
