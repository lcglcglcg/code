
SRCPATH = src/
OBJPATH = obj/
INCLUDE = -I./include
CFLAG = -Wall -g

ifeq ($(OS), Windows_NT)
INCLUDE += -I"C:\Program Files\Java\jdk1.6.0_32\include"
INCLUDE += -I"C:\Program Files\Java\jdk1.6.0_32\include\win32"
CFLAG += -Wl,--add-stdcall-alias -shared
PRO_NAME = license_15talk.dll
else
INCLUDE += -I/usr/java/jdk1.6.0_12/include/
INCLUDE += -I/usr/java/jdk1.6.0_12/include/linux/
CFLAG += -rdynamic -shared -fPIC
PRO_NAME = /lib64/liblicense_15talk.so
endif 

OBJNAME += $(OBJPATH)license_15talk_drivers.o
OBJNAME += $(OBJPATH)license_15talk_encryption.o
OBJNAME += $(OBJPATH)license_15talk_jni.o
OBJNAME += $(OBJPATH)license_15talk_run.o

all:clean $(OBJNAME)
	gcc $(INCLUDE) $(OBJNAME) -o $(PRO_NAME) $(CFLAG)
	gcc $(INCLUDE) src/main.c $(OBJNAME) -o app.exe -Wall -g

java:    
#	@javac StringUtil.java
	@java StringUtil

$(OBJPATH)%.o:$(SRCPATH)%.c
	gcc $(INCLUDE) -c $< -o $@ $(CFLAG)

clean:
ifeq ($(OS), Windows_NT)
	del /Q obj\*
	del /Q $(PRO_NAME)
	del /Q app.exe
else
	rm -f $(OBJNAME)
	rm -f $(PRO_NAME)
	rm -f app.exe
endif


