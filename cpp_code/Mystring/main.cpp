#include <iostream>
#include "string.h"

using namespace std;

int main(int argc, char **argv)
{
    my_string str("hello");
    my_string str1 = str;
    cout<<str1<<endl;
    cout<<str1.len()<<endl;

    my_string str2 = "";
    cout<<(str2.isEmpty()?"true":"false")<<endl;
    str2 = "world";
    cout<<(str2.isEmpty()?"true":"false")<<endl;
    cout<<str2<<endl;

    cout<<str1 + str2<<endl;
    cout<<str1 + "_world"<<endl;
    cout<<"Hello" + str2<<endl;
    return 0;
}
