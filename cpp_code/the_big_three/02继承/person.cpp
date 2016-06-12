#include <iostream>
#include <cstring>
#include "person.h"

using namespace std;

person::person(const char *name, int age):name(NULL),age(age)
{
    cout<<"create person"<<endl;
    this->name = new char[strlen(name) + 1];
    strcpy(this->name, name);
}

person::~person(){
    cout<<"delete person"<<endl;
    delete[] name;
}

void person::display()const
{
    cout<<"display person"<<endl;
    cout<<name<<endl;
    cout<<age<<endl;
}

student::student(const char *name, int age, int score):person(name, age),score(score){
    cout<<"create student"<<endl;
}

student::~student(){
    cout<<"delete student"<<endl;
}

void student::display()const{
    cout<<"display student"<<endl;
    cout<<name<<endl;
    cout<<age<<endl;
    cout<<score<<endl;
}
