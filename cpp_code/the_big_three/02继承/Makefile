all:app

app:person.o main.o
	g++ -g main.o person.o -Wall

main.o:main.cpp
	g++ -g main.cpp -c -o main.o

person.o:person.cpp
	g++ -g person.cpp -c -o person.o

clean:
	rm -f *.o
	rm -f a.out
