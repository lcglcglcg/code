CXX = gcc

CFLAG += -Wall
#CFLAG += -g
#CFLAG += -mwindows

LIB += -lwsock32
LIB += -liphlpapi
LIB += -limagehlp
LIB += -lpsapi

all:
	$(CXX) dll.c -o mydll.dll -shared $(LIB) $(CFLAG)
	$(CXX) main.c -o temp $(LIB) $(CFLAG)

clean:
	rm -f temp.exe
	rm -f mydll.dll
