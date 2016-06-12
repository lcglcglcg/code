
CFLAG = -g
INCLUDE = -I./include
SRCPATH = ./src/
OBJPATH = ./obj/

LIBPATH += -luuid 
LIBPATH += -lole32 
LIBPATH += -loleaut32 
LIBPATH += -lcomdlg32 
LIBPATH += -lgdi32
LIBPATH += -lwsock32

ICOPATH = ./obj/ico.o

OBJNAME += $(OBJPATH)lg_list.o
OBJNAME += $(OBJPATH)lg_config_file.o
OBJNAME += $(OBJPATH)webbrowser.o
OBJNAME += $(OBJPATH)main.o

all:clean $(OBJNAME)
	@windres -i ./ico/ico.rc -o $(ICOPATH) --input-format=rc
	g++ $(INCLUDE) $(ICOPATH) $(OBJNAME) -o appClick.exe $(LIBPATH) $(CFLAG)

$(OBJPATH)%.o:$(SRCPATH)%.c
	gcc $(INCLUDE) -c $< -o $@ $(CFLAG)

$(OBJPATH)%.o:$(SRCPATH)%.cpp 
	g++ $(INCLUDE) -c $< -o $@ $(CFLAG)

clean:
	@del /Q obj\*
	@del /Q appClick.exe

