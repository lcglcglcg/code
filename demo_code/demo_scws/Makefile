
CFLAG = -Wall -g
INCLUDE = -I/usr/local/scws/include/scws/ -I/usr/local/include/hiredis/
LIBS = -L/usr/local/scws/lib/ -lscws -lhiredis -liconv

all:
	gcc $(INCLUDE) main.c $(LIBS) $(CFLAG)
