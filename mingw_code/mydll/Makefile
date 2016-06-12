CC = gcc
#CFLAG += -mwindows
#CFLAG += -lwsock32
CFLAG += -Wall

all:
	$(CC) mydll.c -o mydll.dll -shared $(CFLAG)
	$(CC) main.c -o app.exe $(CFLAG)

clean:
	rm -f app.exe
	rm -f mydll.dll
