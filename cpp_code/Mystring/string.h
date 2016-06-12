#include <iostream>

using namespace std;

class my_string
{
    friend ostream &operator<<(ostream &, const my_string &);
    friend my_string operator+(const my_string &, const my_string &);
    public:
    my_string(const char *);
    my_string(const my_string &);
    ~my_string(void);
    my_string &operator=(const my_string &);
    int len(void)const;
    bool isEmpty(void)const;
    private:
    char *data;
    int n;
};
