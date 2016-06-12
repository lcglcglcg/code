CC = gcc
CFLAG += -lwsock32
CFLAG += -lgdi32
CFLAG += -Wall 

all:
	$(CC) main.c -o app.exe $(CFLAG)
	$(CC) dll.c -o hook.dll -shared $(CFLAG)

clean:
	rm -f app.exe
	rm -f hook.dll
	

	
