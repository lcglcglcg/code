
CFLAG = 
INCLUDE = -I./include
SRCPATH = ./src/
OBJPATH = ./obj/

LIBPATH += -luuid 
LIBPATH += -lole32 
LIBPATH += -loleaut32 
LIBPATH += -lcomdlg32 
LIBPATH += -lgdi32
LIBPATH += -lwsock32

OBJNAME += $(OBJPATH)json_hash.o
OBJNAME += $(OBJPATH)json_print.o
OBJNAME += $(OBJPATH)json_array.o
OBJNAME += $(OBJPATH)json_object.o
OBJNAME += $(OBJPATH)json_tokener.o
OBJNAME += $(OBJPATH)network.o
OBJNAME += $(OBJPATH)webbrowser.o
OBJNAME += $(OBJPATH)main.o

all:clean $(OBJNAME)
	gcc ./src/dll_appManager.c -o appManager.dll $(LIBPATH) -shared
	windres -i ./ico/ico.rc -o ./obj/ico.o --input-format=rc
	g++ $(INCLUDE) $(OBJNAME) ./obj/ico.o -o appClick.exe $(LIBPATH) $(CFLAG)

$(OBJPATH)%.o:$(SRCPATH)%.c
	gcc $(INCLUDE) -c $< -o $@ $(CFLAG)

$(OBJPATH)%.o:$(SRCPATH)%.cpp 
	g++ $(INCLUDE) -c $< -o $@ $(CFLAG)

clean:
	@del /Q obj\*
	@del /Q appClick.exe

