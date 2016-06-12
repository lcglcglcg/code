
SRCPATH = src/
OBJPATH = obj/
INCLUDE = -I./include -I/usr/local/libevent2/include/
CFLAG	= -Wall -g
LIBPATH	= -L/usr/local/libevent2/lib/ -levent -lcurl

OBJNAME += $(OBJPATH)lg_list.o
OBJNAME += $(OBJPATH)lg_rbtree.o
OBJNAME += $(OBJPATH)lg_string.o
OBJNAME += $(OBJPATH)lg_network.o
OBJNAME += $(OBJPATH)main.o

all:clean $(OBJNAME)
	gcc $(INCLUDE) $(OBJNAME) $(LIBPATH) $(CFLAG) -o app_spider

$(OBJPATH)%.o:$(SRCPATH)%.cpp
	g++ $(INCLUDE) -c $< -o $@ $(CFLAG)

$(OBJPATH)%.o:$(SRCPATH)%.c
	gcc $(INCLUDE) -c $< -o $@ $(CFLAG)

cscope:
	@rm -f cscope.*
	@cscope -Rbkq

clean:
	@rm -f $(OBJNAME)
	@rm -f app_spider

