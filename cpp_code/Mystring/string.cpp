#include <iostream>
#include "string.h"

using namespace std;

my_string::my_string(const char *str){
    cout<<"call create"<<endl;

    if(str == NULL){
	n = 0;
	data = new char[1];
	*data = '\0';
    }else{
	n = strlen(str);
	data = new char[n + 1];
	strcpy(data, str);
    }
}

my_string::my_string(const my_string &str){
    cout<<"call copy"<<endl;
    
    n = str.n;
    data = new char[n + 1];
    strcpy(data, str.data);
}

my_string::~my_string(void){
    cout<<"call delete"<<endl;
    
    delete[] data;
}

my_string &my_string::operator=(const my_string &str){
    cout<<"call ="<<endl;
    
    if(this == &str){
	return *this;
    }

    delete[] data;
    n = str.n;
    strcpy(data, str.data);
    return *this;
}

int my_string::len(void)const{
    cout<<"call len"<<endl;
    
    return n;
}

bool my_string::isEmpty(void)const{
    cout<<"call isEmpty"<<endl;
    
    return n == 0;
}

ostream &operator<<(ostream &out, const my_string &str){
   
    out<<str.data;
    return out;
}

my_string operator+(const my_string &a, const my_string &b){
    cout<<"call const +"<<endl;
    
    my_string temp("");
    delete[] temp.data;
    temp.n = a.n + b.n;
    temp.data = new char[temp.n + 1];
    strcpy(temp.data, a.data);
    strcat(temp.data, b.data);
    return temp;
}
