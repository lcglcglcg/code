
CC	= gcc
CFLAG 	= -Wall -g
SRCPATH = src/
OBJPATH = obj/
APP	= ad_system_display_db

INCLUDE += -I./include
INCLUDE += -I./include/oci
INCLUDE += -I./include/ocilib
INCLUDE += -I./include/core

LIBS	+=-L/usr/local/lib
LIBS	+=-lnnz -lclntsh
LIBS	+=-luuid

OBJS 	+= $(OBJPATH)ocilib_agent.o
OBJS 	+= $(OBJPATH)ocilib_array.o
OBJS 	+= $(OBJPATH)ocilib_bind.o
OBJS 	+= $(OBJPATH)ocilib_callback.o
OBJS 	+= $(OBJPATH)ocilib_collection.o
OBJS 	+= $(OBJPATH)ocilib_column.o
OBJS 	+= $(OBJPATH)ocilib_connection.o
OBJS 	+= $(OBJPATH)ocilib_date.o
OBJS 	+= $(OBJPATH)ocilib_define.o
OBJS 	+= $(OBJPATH)ocilib_dequeue.o
OBJS 	+= $(OBJPATH)ocilib_dirpath.o
OBJS 	+= $(OBJPATH)ocilib_element.o
OBJS 	+= $(OBJPATH)ocilib_enqueue.o
OBJS 	+= $(OBJPATH)ocilib_error.o
OBJS 	+= $(OBJPATH)ocilib_event.o
OBJS 	+= $(OBJPATH)ocilib_exception.o
OBJS 	+= $(OBJPATH)ocilib_file.o
OBJS 	+= $(OBJPATH)ocilib_format.o
OBJS 	+= $(OBJPATH)ocilib_handle.o
OBJS 	+= $(OBJPATH)ocilib_hash.o
OBJS 	+= $(OBJPATH)ocilib_interval.o
OBJS 	+= $(OBJPATH)ocilib_iterator.o
OBJS 	+= $(OBJPATH)ocilib_library.o
OBJS 	+= $(OBJPATH)ocilib_list.o
OBJS 	+= $(OBJPATH)ocilib_lob.o
OBJS 	+= $(OBJPATH)ocilib_long.o
OBJS 	+= $(OBJPATH)ocilib_memory.o
OBJS 	+= $(OBJPATH)ocilib_msg.o
OBJS 	+= $(OBJPATH)ocilib_mutex.o
OBJS 	+= $(OBJPATH)ocilib_number.o
OBJS 	+= $(OBJPATH)ocilib_object.o
OBJS 	+= $(OBJPATH)ocilib_pool.o
OBJS 	+= $(OBJPATH)ocilib_queue.o
OBJS 	+= $(OBJPATH)ocilib_ref.o
OBJS 	+= $(OBJPATH)ocilib_resultset.o
OBJS 	+= $(OBJPATH)ocilib_statement.o
OBJS 	+= $(OBJPATH)ocilib_string.o
OBJS 	+= $(OBJPATH)ocilib_subscription.o
OBJS 	+= $(OBJPATH)ocilib_thread.o
OBJS 	+= $(OBJPATH)ocilib_threadkey.o
OBJS 	+= $(OBJPATH)ocilib_timestamp.o
OBJS 	+= $(OBJPATH)ocilib_transaction.o
OBJS 	+= $(OBJPATH)ocilib_typeinfo.o

OBJS 	+= $(OBJPATH)lg_ocilib.o
OBJS 	+= $(OBJPATH)lg_list.o
OBJS 	+= $(OBJPATH)lg_rbtree.o
OBJS 	+= $(OBJPATH)lg_string.o
OBJS 	+= $(OBJPATH)lg_db.o
OBJS 	+= $(OBJPATH)lg_config_file.o


OBJS 	+= $(OBJPATH)ad_system_db_load.o
OBJS 	+= $(OBJPATH)ad_system_db_element.o
OBJS 	+= $(OBJPATH)ad_system_db_netwrok.o
OBJS 	+= $(OBJPATH)ad_system_db_print.o
OBJS 	+= $(OBJPATH)main.o

all:$(OBJS)
	$(CC) $(INCLUDE) $(OBJS) $(LIBS) $(CFLAG) -o $(APP)

$(OBJPATH)%.o:$(SRCPATH)%.c
	$(CC) $(INCLUDE) -c $< -o $@ $(CFLAG)

$(OBJPATH)%.o:$(SRCPATH)ocilib/%.c
	$(CC) $(INCLUDE) -c $< -o $@ $(CFLAG)

$(OBJPATH)%.o:$(SRCPATH)core/%.c
	$(CC) $(INCLUDE) -c $< -o $@ $(CFLAG)

clean:
	rm -f $(OBJS)
	rm -f $(APP)


	


