#include <iostream>

using namespace std;

class Complex
{
    public:
	double x, y;
};

int add(int x, int y)

{
    return x + y;
}

double add(double x, double y)
{
    return x + y;
}

Complex operator+(const Complex &a, const Complex &b)
{
    Complex sum;
    sum.x = a.x + b.x;
    sum.y = a.y + b.y;
    return sum;
}

Complex operator++(Complex &a)
{
    a.x += 1;
    a.y += 1;
    return a;
}

int main()
{
    cout<<add(1, 1)<<add(1.1, 1.1)<<endl;

    Complex a = {1.1, 1.1}, b = {1.1, 1.1}, c;
    c = a + b;
    cout<<c.x<<c.y<<endl;
    ++c;
    cout<<c.x<<c.y<<endl;
    return 0;
}
