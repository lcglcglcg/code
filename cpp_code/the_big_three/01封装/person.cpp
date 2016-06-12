#include <iostream>
#include <cstring>
#include "person.h"

using namespace std;

person::person(const char *a, int b):name(NULL),age(b)
{
    cout<<"create person"<<endl;
    //printf("this = %p\n", this);
    this->name = new char[strlen(a) + 1];
    strcpy(this->name, a);
    cout<<endl;
}

void person::display()const
{
    cout<<"display person"<<endl;
    //cout<<"name = "<<this->name<<endl;
    cout<<"name = "<<name<<endl;
    //cout<<"age = "<<this->age<<endl;
    cout<<"age = "<<age<<endl;
    //printf("this = %p\n", this);
    //printf("%p \n%p\n", this->name, name);
    cout<<endl;
}

person::~person()
{
    cout<<"delete person"<<endl;
    //printf("this = %p\n", this);
    delete name;
    cout<<endl;
}
