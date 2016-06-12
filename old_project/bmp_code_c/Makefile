
INCLUDE += -I./include
INCLUDE += -I./libgd-2.1.0/src/

SRC += lg_list.c
SRC += bmp_code.c
SRC += font_code.c
SRC += parse_code.c
SRC += main.c
LIB=libgd-2.1.0/src/.libs/libgd.a -ljpeg -lm
all:
	gcc $(INCLUDE) $(SRC) -o app.exe $(LIB) -Wall -g

clean:
	rm -f app.exe

