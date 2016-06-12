
INCLUDE	= -I/usr/lib/jvm/java-1.6.0-openjdk-1.6.0.0/include/
INCLUDE	+= -I/usr/lib/jvm/java-1.6.0-openjdk-1.6.0.0/include/linux/

all:clean
	javac demo.java
	gcc $(INCLUDE) main.c -o /lib/libdemo.so -rdynamic -shared

clean:
	rm -f *.h
	rm -f *.log
	rm -f *.class
	rm -f /lib/libdemo.so

