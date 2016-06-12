CC = g++
#CFLAG += -lwsock32
#CFLAG += -liphlpapi
#CFLAG += -limagehlp
#CFLAG += -lpsapi
#CFLAG += -mwindows

CFLAG += -Wall

all:
	$(CC) manager.c service.c -o app_manager $(CFLAG)

clean:
	rm -f app_manager.exe

