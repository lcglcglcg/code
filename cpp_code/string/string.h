#include <iostream>

using namespace std;

class mystring{
    friend ostream &operator<<(ostream &, const mystring &);
    friend mystring operator+(const mystring &, const mystring &);
    public:
	mystring(char *);
	mystring(const mystring &);
	~mystring();
    private:
	char *data;
};
