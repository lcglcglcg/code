class person
{
    public:
	person(const char *name, int age);
	~person();
	void display()const;
    protected:
	char *name;
	int age;
};

class student:public person
{
    public:
	student(const char *name, int age, int score);
	~student();
	void display()const;
    protected:
	int score;
};
