#include <iostream>
#include "person.h"

using namespace std;

int main(int argc, char **argv)
{
    student *a = new student("li si", 20, 30);
    person  *b = new person("zhang san", 10);
    person  *p = NULL;

    p = a;
    p->display();

    p = b;
    p->display();

    delete a;
    delete b;
    return 0;
}
