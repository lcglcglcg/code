
CC = gcc
CFLAG = -Wall -g
INCLUDE = -I./include 
LIBPATH = -pthread
SRCPATH = ./src/
OBJPATH = ./obj/
APPNAME = app_route

OBJNAME += $(OBJPATH)lg_queue.o
OBJNAME += $(OBJPATH)lg_rbtree.o
OBJNAME += $(OBJPATH)lg_string.o
OBJNAME += $(OBJPATH)lg_network.o
OBJNAME += $(OBJPATH)lg_thread.o

#OBJNAME += $(OBJPATH)json_hash.o
#OBJNAME += $(OBJPATH)json_print.o
#OBJNAME += $(OBJPATH)json_array.o
#OBJNAME += $(OBJPATH)json_object.o
#OBJNAME += $(OBJPATH)json_tokener.o

OBJNAME += $(OBJPATH)ip_restrict.o
OBJNAME += $(OBJPATH)main.o

all:clean $(OBJNAME)
	$(CC) $(INCLUDE) $(OBJNAME) -o $(APPNAME) $(LIBPATH) $(CFLAG)

$(OBJPATH)%.o:$(SRCPATH)%.c
	$(CC) $(INCLUDE) -c $< -o $@ $(CFLAG)

clean:
	rm -f $(OBJNAME)
	rm -f $(APPNAME)

