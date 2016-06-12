#include <iostream>
#include "person.h"

using namespace std;

int main(int argc, char **argv)
{
    person *person_t = new person("zhang san", 10);
    person_t->display();
    delete person_t;
    cout<<"sizeof = "<<sizeof(*person_t)<<endl<<endl;

    student *student_t = new student("li si", 20, 30);
    student_t->display();
    delete student_t;
    cout<<"sizeof = "<<sizeof(*student_t)<<endl<<endl;

    return 0;
}
