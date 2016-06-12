MAKEFILE      = Makefile
RM            = rm -f
CP            = cp
CXX           = g++
LD            = $(CXX)
COMPFLAGS     = -I. -W -Wall
LDFLAGS       =	-s
LIBS          =


#################
TARGET        = Tproxy.exe

OBJS          =	tcp_listen.o Connect_serv.o tcp_connect.o DataTransAlpha.o Data_Trans.o writen.o error.o Tp.o 


#################
# Target Rules
#################
.SUFFIXES :
.SUFFIXES : .h .c .o
.cpp.o:
	@echo "------------------"
	$(RM) $*.o
	$(CXX) $(COMPFLAGS) -c $<

#################
all:	first

first : $(TARGET)


#################
$(TARGET):	$(OBJS) $(LIBS) $(MAKEFILE)
		@echo "=================="
		$(RM) $(TARGET)
		$(LD) -o $(TARGET) $(LDFLAGS) $(OBJS) $(LIBS) -l pthread
		@echo " "


#################
cleano:	        
	$(RM) $(OBJS)
	$(RM) *.i
	$(RM) *.ii
	$(RM) *.s
	$(RM) *.o
	$(RM) *.d
	$(RM) *.obj
	$(RM) *.map
	$(RM) *.asm
	$(RM) *.tds
	$(RM) *.dep
	$(RM) core
	$(RM) *.exe.*

cleane:
	$(RM) $(TARGET)
	$(RM) *.exe

clean:	cleano cleane


#################
error.o :$(MAKEFILE)  error.c Tp.h
tcp_listen.o : $(MAKEFILE) tcp_listen.c Tp.h
tcp_connect.o   : $(MAKEFILE) tcp_connect.c Tp.h
Connect_serv.o : $(MAKEFILE) Connect_serv.c Tp.h clinum.h
writen.o:$(MAKEFILE) writen.c Tp.h 
DataTransAlpha.o : $(MAKEFILE) DataTransAlpha.c Tp.h 
Data_Trans.o:$(MAKEFILE) Data_Trans.c Tp.h 
Tp.o :$(MAKEFILE) Tp.c Tp.h
