
CC = gcc
CFLAG = -fPIC -shared -DWITH_OPENSSL -lssl -lcurl -Wall
INCLUDE = -I./include
SRCPATH = src/
OBJPATH = obj/
APPNAME = libsoapreport.so

OBJNAME += $(OBJPATH)soapC.o
OBJNAME += $(OBJPATH)soapClient.o
OBJNAME += $(OBJPATH)stdsoap2.o
OBJNAME += $(OBJPATH)baidu_download.o
#OBJNAME += $(OBJPATH)main.o

all:clean $(OBJNAME)
	$(CC) $(INCLUDE) $(OBJNAME) $(LIBPATH) -o $(APPNAME) $(CFLAG)


$(OBJPATH)%.o:$(SRCPATH)%.c 
	$(CC) $(INCLUDE) -c $< -o $@ $(CFLAG)

clean:
	rm -f $(OBJNAME)
	rm -f $(APPNAME)
    


