
INCLUDE=#JNI include
CC=gcc
CFLAG= -Wall -g
SHARED_FLAG=-rdynamic -shared -fPIC
JAVAC=javac
C_SRC=app_daemon.c
J_SRC=app_daemon.java

all:clean
	$(CC) $(INCLUDE) $(C_SRC) -o app_daemon $(CFLAG)
	$(CC) $(INCLUDE) $(C_SRC) -o libapp_daemon.so $(SHARED_FLAG) $(CFLAG)
	$(JAVAC) $(J_SRC)

clean:
	rm -f app_daemon
	rm -f libapp_daemon.so
	rm -f app_daemon.class
	


