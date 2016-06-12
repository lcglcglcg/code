all:app

app:add.o main_c.o main_cpp.o
	g++ main_c.o add.o -Wall -o app_c
	g++ main_cpp.o add.o -Wall -o app_cpp

main_c.o:main.c main.cpp
	gcc main.c -c -o main_c.o

main_cpp.o:
	g++ main.cpp -c -o main_cpp.o

add.o:add.c
	gcc add.c -c -o add.o

clean:
	rm -f *.o
	rm -f app_c
	rm -f app_cpp
