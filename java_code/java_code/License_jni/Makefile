
SRCPATH = src/
OBJPATH = obj/
INCLUDE = -I./include
CFLAG = -Wall -g

ifeq ($(OS), Windows_NT)
INCLUDE += -I./include/javasdk
INCLUDE += -I./include/javasdk/win32
CFLAG += -Wl,--add-stdcall-alias -shared -lwsock32
JMSLIBNAME = jms.dll
else
INCLUDE += -I/usr/lib/jvm/java-1.6.0-openjdk-1.6.0.0.x86_64/include
INCLUDE += -I/usr/lib/jvm/java-1.6.0-openjdk-1.6.0.0.x86_64/include/linux
CFLAG += -rdynamic -shared -fPIC
JMSLIBNAME = libjms.so
endif 

OBJNAME += $(OBJPATH)License_drivers.o
OBJNAME += $(OBJPATH)License_control.o
OBJNAME += $(OBJPATH)License_center.o
OBJNAME += $(OBJPATH)License_packet.o
OBJNAME += $(OBJPATH)main.o

all:clean $(OBJNAME)
	gcc $(INCLUDE) $(OBJNAME) -o $(JMSLIBNAME) $(CFLAG)
ifeq ($(OS), Windows_NT)
	gcc $(INCLUDE) $(OBJNAME) -o app.exe -lwsock32
else
	gcc $(INCLUDE) $(OBJNAME) -o app.exe 
endif

$(OBJPATH)%.o:$(SRCPATH)%.c
	gcc $(INCLUDE) -c $< -o $@ $(CFLAG)

clean:
ifeq ($(OS), Windows_NT)
	@del /Q obj\*
	@del /Q $(JMSLIBNAME)
	@del /Q app.exe
else
	@rm -f $(OBJNAME)
	@rm -f $(JMSLIBNAME)
	@rm -f app.exe
endif


