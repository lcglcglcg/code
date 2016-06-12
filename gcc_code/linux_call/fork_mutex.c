#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
#include <errno.h>
#include <fcntl.h>
#include <sys/types.h>
#include <sys/stat.h>
#include <sys/wait.h>


int file_lock(int fd)
{

    struct flock s_flock;
    s_flock.l_type = F_WRLCK;

    s_flock.l_whence = SEEK_SET;
    s_flock.l_start = 0;
    s_flock.l_len = 0;
    s_flock.l_pid = getpid();

    //F_SETLKW对加锁操作进行阻塞，
    //F_SETLK不对加锁操作进行阻塞，立即返回
    return fcntl(fd, F_SETLKW, &s_flock);
}


int file_unlock(int fd)
{
    return fcntl(fd, F_SETLKW, F_UNLCK);
}

int main(int argc,char *argv[]) 
{

    int pid = -1;
    int i = 5;
    while(i--){
	if ((pid = fork()) < 0) { 

	    puts("fork1 error");

	} else if (pid > 0) {

	    sleep(5);
	    if (waitpid(pid, NULL, 0) < 0)
		puts("waitpid error");

	} else {

	    sleep(1);
	    int li_file = 0;
	    int li_lck_st = -1;
	    li_file = open("tt.txt", O_WRONLY|O_CREAT, 0777);
	    if( li_file < 0 ) {
		printf("file open error\n");
	    }
	    printf("li_file=[%d] pid=[%d]\n", li_file , getpid() );
	    li_lck_st = file_lock(li_file);
	    sleep(5);
	    printf("li_lck_st=%d pid =%d\n", li_lck_st, getpid() );
	    file_unlock(li_file);
	    close(li_file);
	    printf("close file pid=%d unlock\n", getpid());
	    return 0;
	}
    }

    return 0;
}


/*
 * 
#########################################################
#Project: test
#By: Khan.Lau
#Description: test
#########################################################

CC = gcc
CXX = g++
STRIP = strip
AR = ar

PROJ = $(shell pwd)
DLLPATH = $(shell find $(PROJ) -name *.so*)

LIBPATH = /usr

INCS = -I$(LIBPATH)/include \
-I$(PROJ)/src/inc \
-I$(PROJ)/src


CXXINCS = -I$(LIBPATH)/include \
-I$(LIBPATH)/include/c++/4.3 \
-I$(LIBPATH)/include/c++/4.3/backward \
-I$(PROJ)/src/inc \
-I$(PROJ)/src


CFLAGS = -g -Wall $(CXXINCS) 
#
CONFIGURATION = release
#
BIN = test.exe
#LIB = lib/HuaWeiBsc.a

#包路径
LIBS = -L$(PROJ)/lib  \
-L$(LIBPATH)/lib -lz

SRCS = $(wildcard \
src/lib/*.cpp \
src/kernel/*.cpp)


OBJBINS = $(SRCS:.cpp=.o)


.c.o:
$(CC) -o $@ -c $< $(CFLAGS)

.cpp.o:
$(CXX) -o $@ -c $< $(CFLAGS) 


COLORH=echo -en "\033[1;32;40m"
COLORN=echo -en "\033[m"


all : $(BIN)

$(BIN) : $(OBJBINS)
$(CXX) -o $(CONFIGURATION)/$(BIN) $(OBJBINS) $(LIBS)
@$(COLORN)     $(shell echo "$(BIN) build complete.")

#$(LIB) : $(OBJBINS)
#    ar rc $(LIB) $(OBJBINS)


clean :
rm -f $(OBJBINS)
rm -f $(BIN)
*/
