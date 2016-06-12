all:app

app:person.o main.o
	g++ main.o person.o -Wall

main.o:main.cpp
	g++ main.cpp -c -o main.o

person.o:person.cpp
	g++ person.cpp -c -o person.o

clean:
	rm -f *.o
	rm -f a.out
