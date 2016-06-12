
CC = gcc
CFLAG = -pthread -funroll-loops -lm -lxl -Wall -g
LIBPATH = -L./soap_baidu_account -lsoapaccount -L./soap_baidu_report -lsoapreport
INCLUDE = -I./include
SRCPATH = ./src/
OBJPATH = ./obj/

OBJNAME += $(OBJPATH)lg_queue.o
OBJNAME += $(OBJPATH)network.o
OBJNAME += $(OBJPATH)thread_pool.o
OBJNAME += $(OBJPATH)log_file.o

OBJNAME += $(OBJPATH)sem_key.o
OBJNAME += $(OBJPATH)calendar.o
OBJNAME += $(OBJPATH)baidu_user.o
OBJNAME += $(OBJPATH)sem_user.o
OBJNAME += $(OBJPATH)task_list.o

OBJNAME += $(OBJPATH)baidu_download.o
OBJNAME += $(OBJPATH)baidu_report.o
OBJNAME += $(OBJPATH)zoosnet_report.o
OBJNAME += $(OBJPATH)complex_report.o

WEBOBJ += $(OBJPATH)packet_web.o
WEBOBJ += $(OBJPATH)main_web.o
WEBNAME = semtools_web_online

ONLYOBJ += $(OBJPATH)packet_only.o
ONLYOBJ += $(OBJPATH)main_only.o
ONLYNAME = semtools_only_online

all:clean $(OBJNAME) $(WEBOBJ) $(ONLYOBJ)
	$(CC) $(INCLUDE) $(OBJNAME) $(WEBOBJ) $(LIBPATH) -o $(WEBNAME) $(CFLAG)
	$(CC) $(INCLUDE) $(OBJNAME) $(ONLYOBJ) $(LIBPATH) -o $(ONLYNAME) $(CFLAG)

$(OBJPATH)%.o:$(SRCPATH)%.c 
	$(CC) $(INCLUDE) -c $< -o $@ $(CFLAG)

clean:
	rm -f $(OBJNAME)
	rm -f $(WEBOBJ)
	rm -f $(WEBNAME)
	rm -f $(ONLYOBJ)
	rm -f $(ONLYNAME)
    


