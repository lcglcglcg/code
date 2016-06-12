
CC = gcc
INCLUDE = -I./include
CFLAG = -pthread -Wall -g
SRCPATH = ./src/
OBJPATH = ./obj/
APPNAME = appServer

OBJNAME += $(OBJPATH)json_hash.o
OBJNAME += $(OBJPATH)json_print.o
OBJNAME += $(OBJPATH)json_array.o
OBJNAME += $(OBJPATH)json_object.o
OBJNAME += $(OBJPATH)json_tokener.o

OBJNAME += $(OBJPATH)lg_queue.o
OBJNAME += $(OBJPATH)lg_rbtree.o
OBJNAME += $(OBJPATH)lg_string.o
OBJNAME += $(OBJPATH)lg_network.o
OBJNAME += $(OBJPATH)lg_thread.o

OBJNAME += $(OBJPATH)main.o

all:$(OBJNAME)
	$(CC) $(INCLUDE) $(OBJNAME) $(LIBPATH) $(CFLAG) -o $(APPNAME)

$(OBJPATH)%.o:$(SRCPATH)%.c
	$(CC) $(INCLUDE) -c $< -o $@ $(CFLAG)

clean:
	rm -f $(OBJNAME)
	rm -f $(APPNAME)


