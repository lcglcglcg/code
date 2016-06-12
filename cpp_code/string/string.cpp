#include <iostream>
#include "string.h"

using namespace std;

mystring::mystring(char *str){
    cout<<"call create"<<endl;

    if(str == NULL){
	str = new char[1];
	*str = '\0';
    }else{
	data = new char(strlen(str) + 1);
	strcpy(data, str);
    }
}

mystring::mystring(const mystring &str){
    cout<<"call copy"<<endl;

    data = new char(strlen(str.data) + 1);
    strcpy(data, str.data);
}

mystring::~mystring(){
    cout<<"call delete"<<endl;

    delete[] data;
}

ostream &operator<<(ostream &out, const mystring &str){
    out<<str.data;
    return out;
}

mystring operator+(const mystring &dest, const mystring &str){
    cout<<"call add"<<endl;
    
    mystring tmp("");
    delete[] tmp.data;
    tmp.data = new char(strlen(dest.data) + strlen(str.data)+ 1);
    strcpy(tmp.data, dest.data);
    strcat(tmp.data, str.data);
    return tmp;
}
