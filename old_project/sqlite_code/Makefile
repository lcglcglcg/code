	
CC	= gcc
CFLAG	= -pthread -ldl -lcrypt -Wall -g
INCLUDE = -I./include
SRCPATH = ./src/
OBJPATH = ./obj/
APPNAME = app_sqlite

#OBJNAME += $(OBJPATH)interop.o
#OBJNAME += $(OBJPATH)crypt.o
OBJNAME += $(OBJPATH)sqlite3.o
OBJNAME += $(OBJPATH)main.o

all:$(OBJNAME)
	$(CC) $(INCLUDE) $(OBJNAME) -o $(APPNAME) $(CFLAG)

$(OBJPATH)%.o:$(SRCPATH)%.c
	$(CC) $(INCLUDE) -c $< -o $@ $(CFLAG)

clean:
	rm -f $(OBJNAME)
	rm -f $(APPNAME)
