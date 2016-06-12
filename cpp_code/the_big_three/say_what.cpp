#include <iostream>

using namespace std;

class animal{
    public:
	virtual char* call_what(void)=0;
};

class cow:public animal{
    public:
	virtual char* call_what(void){
	    return "Moo";
	};
};

class dog:public animal{
    public:
	virtual char* call_what(void){
	    return "bark";
	};
};

void whosay(const char *who, animal *p){

    cout<<who<<" = "<<p->call_what()<<endl;
}

int main(int argc, char **argv){

    cow Debby;
    dog Puppy;

    whosay("Debby", &Debby);
    whosay("Puppy", &Puppy);    

    cout<<"sizeof = "<<sizeof(cow)<<endl;
    cout<<"sizeof = "<<sizeof(dog)<<endl;

    return 0;
}
