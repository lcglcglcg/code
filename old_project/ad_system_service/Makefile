
SRCPATH = src/
OBJPATH = obj/
INCLUDE = -I./include
CFLAG = -Wall -g

# core
INCLUDE += -I./include/core
LIBPATH += -pthread -luuid -lssl -ltar
OBJNAME += $(OBJPATH)lg_list.o
OBJNAME += $(OBJPATH)lg_queue.o
OBJNAME += $(OBJPATH)lg_rbtree.o
OBJNAME += $(OBJPATH)lg_string.o
OBJNAME += $(OBJPATH)lg_network.o
OBJNAME += $(OBJPATH)lg_thread.o
OBJNAME += $(OBJPATH)lg_config_file.o
OBJNAME += $(OBJPATH)lg_log_file.o

# mongodb
INCLUDE += -I./include/libmongoc-1.0/
INCLUDE += -I./include/libbson-1.0/
LIBPATH += -lmongoc-1.0
OBJNAME += $(OBJPATH)ad_system_mongo.o

# oci
INCLUDE += -I./include/oci
LIBPATH	+= -lnnz10
LIBPATH	+= -lclntsh
OBJNAME += $(OBJPATH)lg_oci.o

# ocilib
INCLUDE += -I./include/ocilib
OBJNAME += $(OBJPATH)ocilib_agent.o
OBJNAME += $(OBJPATH)ocilib_array.o
OBJNAME += $(OBJPATH)ocilib_bind.o
OBJNAME += $(OBJPATH)ocilib_callback.o
OBJNAME += $(OBJPATH)ocilib_collection.o
OBJNAME += $(OBJPATH)ocilib_column.o
OBJNAME += $(OBJPATH)ocilib_connection.o
OBJNAME += $(OBJPATH)ocilib_date.o
OBJNAME += $(OBJPATH)ocilib_define.o
OBJNAME += $(OBJPATH)ocilib_dequeue.o
OBJNAME += $(OBJPATH)ocilib_dirpath.o
OBJNAME += $(OBJPATH)ocilib_element.o
OBJNAME += $(OBJPATH)ocilib_enqueue.o
OBJNAME += $(OBJPATH)ocilib_error.o
OBJNAME += $(OBJPATH)ocilib_event.o
OBJNAME += $(OBJPATH)ocilib_exception.o
OBJNAME += $(OBJPATH)ocilib_file.o
OBJNAME += $(OBJPATH)ocilib_format.o
OBJNAME += $(OBJPATH)ocilib_handle.o
OBJNAME += $(OBJPATH)ocilib_hash.o
OBJNAME += $(OBJPATH)ocilib_interval.o
OBJNAME += $(OBJPATH)ocilib_iterator.o
OBJNAME += $(OBJPATH)ocilib_library.o
OBJNAME += $(OBJPATH)ocilib_list.o
OBJNAME += $(OBJPATH)ocilib_lob.o
OBJNAME += $(OBJPATH)ocilib_long.o
OBJNAME += $(OBJPATH)ocilib_memory.o
OBJNAME += $(OBJPATH)ocilib_msg.o
OBJNAME += $(OBJPATH)ocilib_mutex.o
OBJNAME += $(OBJPATH)ocilib_number.o
OBJNAME += $(OBJPATH)ocilib_object.o
OBJNAME += $(OBJPATH)ocilib_pool.o
OBJNAME += $(OBJPATH)ocilib_queue.o
OBJNAME += $(OBJPATH)ocilib_ref.o
OBJNAME += $(OBJPATH)ocilib_resultset.o
OBJNAME += $(OBJPATH)ocilib_statement.o
OBJNAME += $(OBJPATH)ocilib_string.o
OBJNAME += $(OBJPATH)ocilib_subscription.o
OBJNAME += $(OBJPATH)ocilib_thread.o
OBJNAME += $(OBJPATH)ocilib_threadkey.o
OBJNAME += $(OBJPATH)ocilib_timestamp.o
OBJNAME += $(OBJPATH)ocilib_transaction.o
OBJNAME += $(OBJPATH)ocilib_typeinfo.o

# execl
INCLUDE += -I./include/libxl
LIBPATH	+= -lxl

# img captcha
INCLUDE += -I./include/libgd
OBJNAME += $(OBJPATH)img_libgd_captcha.o
OBJNAME += $(OBJPATH)img_libgd_fontl.o
LIBPATH += -ljpeg -lpng -lgd

# json
INCLUDE += -I./include/json
OBJNAME += $(OBJPATH)json_hash.o
OBJNAME += $(OBJPATH)json_print.o
OBJNAME += $(OBJPATH)json_array.o
OBJNAME += $(OBJPATH)json_object.o
OBJNAME += $(OBJPATH)json_tokener.o
OBJNAME += $(OBJPATH)lg_string_json.o

# ad_system_service 
OBJNAME += $(OBJPATH)ad_system_sql.o
OBJNAME += $(OBJPATH)ad_system_oci.o
OBJNAME += $(OBJPATH)ad_system_session.o
OBJNAME += $(OBJPATH)ad_system_network.o
OBJNAME += $(OBJPATH)ad_system_global_var.o
OBJNAME += $(OBJPATH)ad_system_upload_keyword.o
OBJNAME += $(OBJPATH)ad_system_upload_image.o
OBJNAME += $(OBJPATH)ad_system_image_list.o
OBJNAME += $(OBJPATH)ad_system_disease_code.o
OBJNAME += $(OBJPATH)ad_system_packet.o
OBJNAME += $(OBJPATH)ad_system_merger.o
OBJNAME += $(OBJPATH)ad_system_report.o
OBJNAME += $(OBJPATH)ad_system_report_account_info.o
OBJNAME += $(OBJPATH)ad_system_region_code.o
OBJNAME += $(OBJPATH)ad_system_report_xls_file.o
OBJNAME += $(OBJPATH)ad_system_domain_parse.o
OBJNAME += $(OBJPATH)main.o

#all:clean $(OBJNAME)
all:$(OBJNAME)
	gcc $(INCLUDE) $(OBJNAME) $(LIBPATH) $(CFLAG) -o ad_system_service

#$(OBJPATH)%.o:$(SRCPATH)%.cpp
#	g++ $(INCLUDE) -c $< -o $@ $(CFLAG)

$(OBJPATH)%.o:$(SRCPATH)%.c
	gcc $(INCLUDE) -c $< -o $@ $(CFLAG)

cscope:
	rm -f cscope.*
	cscope -Rbkq

clean:
	rm -f $(OBJNAME)
	rm -f ad_system_service


