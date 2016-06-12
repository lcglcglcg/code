
INCLUDE	+= -I/usr/lib/jvm/java-1.6.0-openjdk-1.6.0.0/include/
INCLUDE	+= -I/usr/lib/jvm/java-1.6.0-openjdk-1.6.0.0/include/linux/
INCLUDE += -I./include

SRC += bmp_code_load.cpp
SRC += bmp_code_font.cpp
SRC += bmp_code_analysis.cpp
SRC += bmp_code.cpp

all:app

java:
#linux
	g++ $(INCLUDE) $(SRC) main.cpp -o libbmpcode.so -rdynamic -shared -Wall 
#windows
#	g++ $(INCLUDE) $(SRC) main.cpp -o bmpcode.dll -D_JNI_IMPLEMENTATION_ -Wl,--kill-at -shared -Wall
	javac demo.java
	java demo
	
app:
	g++ $(SRC) -o app.exe -Wall -g

clean:
	rm -f app.exe
	rm -f libbmpcode.so
	rm -f bmpcode.dll
	rm -f *.class

