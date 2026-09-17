# C++ Classes and Inheritance — Quick Review

## 1 — Classes and Objects

**Class**

* A class is a blueprint for creating objects.
* It defines the **data** an object contains.
* It defines the **operations** an object can perform.
* Data is usually represented by member variables.
* Operations are represented by member functions.

**Object**

* An object is an instance of a class.
* Each object has its own copy of the class's data.

```cpp
class Person {
public:
    string name;
    int age;

    void introduce() {
        cout << "Hi, I'm " << name << endl;
    }
};

Person person1;
person1.name = "Alice";
person1.age = 20;
```

---

## 2 — Encapsulation

**Encapsulation** means keeping an object's data and the operations that work on that data together.

It also commonly means **controlling access** to the object's internal state.

```cpp
class BankAccount {
private:
    double balance;

public:
    void deposit(double amount) {
        balance += amount;
    }

    double getBalance() {
        return balance;
    }
};
```

The user of the class does not directly manipulate `balance`.

```cpp
BankAccount account;

account.deposit(100);
cout << account.getBalance();
```

---

## 3 — Access Specifiers

C++ provides three major access specifiers:

| Specifier   | Accessible from class | Derived class | Outside class |
| ----------- | --------------------- | ------------- | ------------- |
| `private`   | Yes                   | No            | No            |
| `protected` | Yes                   | Yes           | No            |
| `public`    | Yes                   | Yes           | Yes           |

### Typical approach

```cpp
class Person {
private:
    string name;

protected:
    int age;

public:
    void introduce();
};
```

* `private` → implementation details
* `protected` → available to derived classes
* `public` → class interface

**Important:** Prefer `private` data when possible. `protected` exposes implementation details to subclasses.

---

## 4 — Constructors

A **constructor** initializes an object when it is created.

* Same name as the class.
* Has no return type.
* Runs automatically when an object is created.
* Can have parameters.
* A class can have multiple constructors.

```cpp
class Person {
private:
    string name;
    int age;

public:
    Person(string n, int a) {
        name = n;
        age = a;
    }
};
```

Creating an object:

```cpp
Person person("Alice", 20);
```

---

## 5 — Constructor Initializer Lists

A preferred way to initialize member variables is with an **initializer list**.

```cpp
class Person {
private:
    string name;
    int age;

public:
    Person(string n, int a)
        : name(n), age(a) {
    }
};
```

The initializer list:

```cpp
: name(n), age(a)
```

initializes the members **before the constructor body executes**.

Initializer lists are required for:

* `const` data members
* reference members
* members without a default constructor

They are also generally the preferred style.

---

## 6 — Member Functions

Member functions operate on an object.

```cpp
class Rectangle {
private:
    double width;
    double height;

public:
    double area() {
        return width * height;
    }

    void resize(double w, double h) {
        width = w;
        height = h;
    }
};
```

Usage:

```cpp
Rectangle r;

r.resize(10, 5);

cout << r.area();
```

Inside a member function, the object's members can be accessed directly.

---

# Inheritance

## 7 — What Is Inheritance?

**Inheritance** allows one class to build upon another class.

The existing class is the:

**Base class / Parent class / Superclass**

The new class is the:

**Derived class / Child class / Subclass**

Example:

```text
             Person
                |
        ----------------
        |              |
      Student        Employee
```

A `Student` **is a** `Person`.

An `Employee` **is a** `Person`.

---

## 8 — Basic Inheritance Syntax

```cpp
class Person {
public:
    void introduce() {
        cout << "I am a person." << endl;
    }
};

class Student : public Person {
public:
    void study() {
        cout << "I am studying." << endl;
    }
};
```

Now a `Student` has access to the public interface inherited from `Person`.

```cpp
Student student;

student.introduce();
student.study();
```

The `Student` class did not have to redefine `introduce()`.

---

## 9 — What Does a Derived Class Get?

A derived class can use accessible members of its base class.

```cpp
class Person {
public:
    void introduce() {
        cout << "Hello" << endl;
    }

protected:
    string name;
};
```

```cpp
class Student : public Person {
public:
    void printName() {
        cout << name << endl;
    }
};
```

`Student` can access `name` because it is `protected`.

It cannot directly access a `private` member of `Person`.

---

## 10 — Public Inheritance

Most inheritance examples use:

```cpp
class Student : public Person
```

This expresses an **is-a relationship**.

If:

```text
Student is a Person
```

then public inheritance may make sense.

The public interface of `Person` remains public in `Student`.

```cpp
Person
  |
  | public inheritance
  v
Student
```

### Think carefully before using inheritance

Inheritance represents a relationship between types, not simply code reuse.

---

## 11 — Calling the Base Constructor

When a derived object is created, the **base portion is constructed first**.

```cpp
class Person {
public:
    Person(string name) {
        cout << "Person constructor" << endl;
    }
};

class Student : public Person {
public:
    Student(string name)
        : Person(name) {
        cout << "Student constructor" << endl;
    }
};
```

Creating:

```cpp
Student student("Alice");
```

Results in:

```text
Person constructor
Student constructor
```

The derived class is responsible for passing arguments to the base constructor.

---

## 12 — Method Overriding

A derived class can provide its own implementation of a base-class function.

```cpp
class Animal {
public:
    void speak() {
        cout << "Some sound" << endl;
    }
};

class Dog : public Animal {
public:
    void speak() {
        cout << "Woof!" << endl;
    }
};
```

`Dog` has its own version of `speak()`.

This is called **overriding** when the base function is virtual and the derived function overrides it.

---

## 13 — `virtual` Functions

The `virtual` keyword enables **runtime polymorphism**.

```cpp
class Animal {
public:
    virtual void speak() {
        cout << "Some sound" << endl;
    }
};

class Dog : public Animal {
public:
    void speak() override {
        cout << "Woof!" << endl;
    }
};
```

Now:

```cpp
Animal* animal = new Dog();

animal->speak();
```

prints:

```text
Woof!
```

Even though the pointer is an `Animal*`, C++ calls the `Dog` implementation.

---

## 14 — Why `override`?

Use `override` when a derived class intends to override a virtual function.

```cpp
class Dog : public Animal {
public:
    void speak() override {
        cout << "Woof!" << endl;
    }
};
```

`override` tells the compiler:

> "I expect this function to override a virtual function from the base class."

If you accidentally make a mistake:

```cpp
void speek() override {
}
```

the compiler reports an error.

Without `override`, the mistake might silently create a different function.

**Recommendation:** Use `override` whenever overriding a virtual function.

---

## 15 — Polymorphism

**Polymorphism** means that the same interface can represent different behavior.

```cpp
class Animal {
public:
    virtual void speak() = 0;
};
```

Different animals provide different implementations:

```cpp
class Dog : public Animal {
public:
    void speak() override {
        cout << "Woof!" << endl;
    }
};

class Cat : public Animal {
public:
    void speak() override {
        cout << "Meow!" << endl;
    }
};
```

We can treat both as `Animal` objects:

```cpp
Animal* a = new Dog();
Animal* b = new Cat();

a->speak();
b->speak();
```

Output:

```text
Woof!
Meow!
```

---

## 16 — Abstract Classes

A class containing a **pure virtual function** is an abstract class.

```cpp
class Shape {
public:
    virtual double area() = 0;
};
```

The `= 0` means:

> Derived classes must provide an implementation.

You cannot create a `Shape` object:

```cpp
Shape shape;       // ERROR
```

But you can create derived objects:

```cpp
class Circle : public Shape {
public:
    double area() override {
        return 3.14 * radius * radius;
    }

private:
    double radius;
};
```

---

## 17 — Base-Class Pointers and References

A base-class pointer can point to a derived object.

```cpp
Dog dog;

Animal* animal = &dog;
```

A base-class reference can also refer to a derived object.

```cpp
Dog dog;

Animal& animal = dog;
```

This is fundamental to polymorphism.

```text
          Animal
             ^
             |
          Animal*
             |
             v
            Dog
```

The pointer/reference determines the **interface** we can use.

Virtual functions determine the **implementation** that executes.

---

## 18 — Virtual Destructors

When using polymorphism, a base class should generally have a **virtual destructor**.

```cpp
class Animal {
public:
    virtual ~Animal() = default;

    virtual void speak() = 0;
};
```

Why?

Consider:

```cpp
Animal* animal = new Dog();

delete animal;
```

A virtual destructor ensures that the appropriate derived destructor is called.

### Rule of thumb

If a class has virtual functions and objects may be deleted through a base pointer, give the base class a virtual destructor.

---

## 19 — Inheritance vs. Composition

Inheritance:

```cpp
class Student : public Person
```

means:

> Student **is a** Person.

Composition:

```cpp
class Car {
private:
    Engine engine;
};
```

means:

> Car **has an** Engine.

### Ask:

**Is-a?**

→ Consider inheritance.

**Has-a?**

→ Consider composition.

For example:

```text
Student IS-A Person
Car HAS-A Engine
Computer HAS-A CPU
Dog IS-A Animal
```

---

## 20 — Putting It Together

A common object-oriented design might look like:

```text
                    Shape
                      |
          -------------------------
          |                       |
        Circle                  Rectangle
          |                       |
       area()                   area()
```

Base class:

```cpp
class Shape {
public:
    virtual double area() const = 0;
    virtual ~Shape() = default;
};
```

Derived class:

```cpp
class Circle : public Shape {
public:
    double area() const override {
        return 3.14159 * radius * radius;
    }

private:
    double radius;
};
```

The important ideas are:

* `Shape` defines a common interface.
* `Circle` implements that interface.
* `virtual` enables runtime polymorphism.
* `override` verifies the derived implementation.
* `= 0` makes `Shape` abstract.
* `public` inheritance expresses an **is-a** relationship.

---

# Quick Check

## 21 — Can You Explain These?

1. What is the difference between a **class** and an **object**?
2. Why would a member variable be `private`?
3. What does `public` inheritance mean?
4. What is the difference between a base class and a derived class?
5. Why do we use `virtual`?
6. What does `override` tell the compiler?
7. What does `= 0` mean in a virtual function?
8. Why might a base class need a virtual destructor?
9. When should you consider composition instead of inheritance?
10. Why can an `Animal*` point to a `Dog` object?

### The big picture

```text
Classes
   |
   +-- Encapsulation
   |      |
   |      +-- private data
   |      +-- public interface
   |
   +-- Inheritance
   |      |
   |      +-- Base class
   |      +-- Derived class
   |
   +-- Polymorphism
          |
          +-- virtual
          +-- override
          +-- abstract classes
          +-- base pointers/references
```
