
CC = gcc
CFLAG = -pthread -DWITH_OPENSSL -lssl -lcurl -Wall -g
#CFLAG = -fPIC -shared -DWITH_OPENSSL -lssl -lcurl -Wall
INCLUDE = -I./include
SRCPATH = src/
OBJPATH = obj/
#APPNAME = libsoapmanager.so
APPNAME = app_manager

OBJNAME += $(OBJPATH)soapC.o
OBJNAME += $(OBJPATH)soapClient.o
OBJNAME += $(OBJPATH)stdsoap2.o
OBJNAME += $(OBJPATH)soap_manager.o
OBJNAME += $(OBJPATH)main.o

all:clean $(OBJNAME)
	$(CC) $(INCLUDE) $(OBJNAME) $(LIBPATH) -o $(APPNAME) $(CFLAG)


$(OBJPATH)%.o:$(SRCPATH)%.c 
	$(CC) $(INCLUDE) -c $< -o $@ $(CFLAG)

clean:
	rm -f $(OBJNAME)
	rm -f $(APPNAME)
    


