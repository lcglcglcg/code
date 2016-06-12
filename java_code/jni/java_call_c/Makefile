#################################################
#
#
#
#################################################

CC	= gcc
CFLAG	= -Wall -g
LIBFLAG	= -rdynamic -shared
INCLUDE	= -I$(JAVA_HOME)/include -I$(JAVA_HOME)/include/linux
LIBSRC	= main.c
LIBNAME	= /lib/libtest.so

JAVA	= java
JAVAC	= javac
JAVAH	= javah
JAVASRC	= network

all:JAVACC LIBCC

JAVACC:
	$(JAVAC) $(JAVASRC).java
	$(JAVAH) $(JAVASRC)

LIBCC:
	$(CC) $(CFLAG) $(INCLUDE) -c $(LIBSRC)
	$(CC) $(CFLAG) $(LIBFLAG) -o $(LIBNAME) main.o
	$(CC) $(CFLAG) -o app main.o
	$(CC) $(CFLAG) -o client client.c
#g++ $(CFLAG) $(INCLUDE) test.c

DISPLAY:
	$(JAVA) $(JAVASRC)

clean:
	rm -f *.o
	rm -f app
	rm -f client
	rm -f *.class
	rm -f $(LIBNAME)

