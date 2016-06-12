all:app

app:string.o main.o
	g++ main.o string.o -Wall

string.o:string.cpp
	g++ string.cpp -c -o string.o

main.o:main.cpp
	g++ main.cpp -c -o main.o

clean:
	rm -f *.o
	rm -f a.out
