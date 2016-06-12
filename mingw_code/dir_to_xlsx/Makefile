
SRCPATH = src/
OBJPATH = obj/
INCLUDE += -I./include
INCLUDE += -I./include_c
CFLAG = -Wall -g

OBJNAME += $(OBJPATH)lg_list.o
OBJNAME += $(OBJPATH)lg_config_file.o
OBJNAME += $(OBJPATH)main.o

all:$(OBJNAME)
ifeq ($(OS), Windows_NT)
	gcc $(INCLUDE) $(OBJNAME) $(CFLAG) bin/libxl.dll -o bin/app.exe 
else
	gcc $(INCLUDE) $(OBJNAME) $(CFLAG) -lxl -o bin/app.exe 
endif	

$(OBJPATH)%.o:$(SRCPATH)%.c
	gcc $(INCLUDE) -c $< -o $@ $(CFLAG)

clean:
	rm -f $(OBJNAME)
	rm -f bin/app.exe


