#include <iostream>
#include "person.h"

int main()
{
    person *person_t = new person("zhang ming", 12);
    person_t->display();
    delete person_t;

    //person person_t_node("xiao ming", 12);
    //person_t_node.display();
    return 0;
}
