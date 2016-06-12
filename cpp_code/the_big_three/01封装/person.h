#ifndef PERSON_H
#define PERSON_H

class person
{
    public:
	person(const char *a, int b);
	~person();
	void display()const;
    private:
	char *name;
	int age;
};

#endif
