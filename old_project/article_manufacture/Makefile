
SRCPATH = src/
OBJPATH = obj/
INCLUDE = -I./include -I/usr/include/libxml2 -I/usr/local/include/scws
CFLAG	= -Wall -g
LIBPATH = lib/librabbitmq.a -lhiredis -lxml2 -lscws -lcurl

OBJNAME += $(OBJPATH)lg_list.o
OBJNAME += $(OBJPATH)lg_rbtree.o
OBJNAME += $(OBJPATH)lg_string.o
OBJNAME += $(OBJPATH)lg_thread_pool.o
OBJNAME += $(OBJPATH)lg_config_file.o
OBJNAME += $(OBJPATH)lg_rabbitmq.o
OBJNAME += $(OBJPATH)lg_election.o
OBJNAME += $(OBJPATH)lg_filter_keyword.o
OBJNAME += $(OBJPATH)lg_redis.o

OBJNAME += $(OBJPATH)lg_json.o
OBJNAME += $(OBJPATH)json_array.o
OBJNAME += $(OBJPATH)json_hash.o
OBJNAME += $(OBJPATH)json_object.o
OBJNAME += $(OBJPATH)json_print.o
OBJNAME += $(OBJPATH)json_tokener.o

DISORDERLY = $(OBJPATH)disorderly.o
BAIDU_TRANS = $(OBJPATH)baidu_trans.o
SEND_QUQUE = $(OBJPATH)send_queue.o
GET_QUQUE = $(OBJPATH)utils.o
GET_QUQUE += $(OBJPATH)get_queue.o

All:clean $(OBJNAME) $(DISORDERLY) $(BAIDU_TRANS) $(SEND_QUQUE) $(GET_QUQUE)
	gcc $(INCLUDE) $(OBJNAME) $(DISORDERLY) 	$(LIBPATH) $(CFLAG) -o app_disorderly
	gcc $(INCLUDE) $(OBJNAME) $(BAIDU_TRANS) 	$(LIBPATH) $(CFLAG) -o app_baidu_trans
	gcc $(INCLUDE) $(OBJNAME) $(SEND_QUQUE) 	$(LIBPATH) $(CFLAG) -o send_queue
	gcc $(INCLUDE) $(OBJNAME) $(GET_QUQUE) 		$(LIBPATH) $(CFLAG) -o get_queue

$(OBJPATH)%.o:$(SRCPATH)%.cpp
	g++ $(INCLUDE) -c $< -o $@ $(CFLAG)

$(OBJPATH)%.o:$(SRCPATH)%.c
	gcc $(INCLUDE) -c $< -o $@ $(CFLAG)

cscope:
	@rm -f cscope.*
	@cscope -Rbkq

clean:
	@rm -f $(OBJNAME)
	@rm -f $(DISORDERLY)
	@rm -f $(BAIDU_TRANS)
	@rm -f $(SEND_QUQUE)
	@rm -f $(GET_QUQUE)
	@rm -f app_disorderly
	@rm -f app_baidu_trans
	


