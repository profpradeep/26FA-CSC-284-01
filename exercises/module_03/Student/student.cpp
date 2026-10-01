#include <iostream>
#include <string>

class Student
{
private:
    std::string name;

public:
    Student(const std::string& n) : name(n)
    {
        std::cout << "Student created: " << name << '\n';
    }

    ~Student()
    {
        std::cout << "Student destroyed: " << name << '\n';
    }

    void introduce() const
    {
        std::cout << "Hi, I'm " << name << '\n';
    }
};

void introduceStudent()
{
    Student* student = new Student("Alice");

    student->introduce();

    delete student;
}

int main()
{
    introduceStudent();
}