#include <iostream>
#include "string.h"

using namespace std;

int main(int argc, char **argv)
{
    mystring str1("hello");
    mystring str2 = "world";
    mystring str3("");

    cout<<str1 + "_world"<<endl<<endl;
    cout<<"Hello" + str2<<endl<<endl;
    cout<<str1 + str2<<endl<<endl;



    str3 = str1;
    cout<<str3<<endl;

    return 0;
}
