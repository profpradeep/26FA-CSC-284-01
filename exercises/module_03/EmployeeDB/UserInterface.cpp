#include "Employee.h"
#include "Database.h"

#include <iostream>
#include <string>
#include <format>

using namespace std;
using namespace Records;

int displayMenu();
void doHire(Database& db);
void doFire(Database& db);
void doPromote(Database& db);

int main()
{
	Database employeeDB;

	bool done{ false };
	while (!done) {
		int selection{ displayMenu() };
		switch (selection) {
		case 0:
			done = true;
			break;
		case 1:
			doHire(employeeDB);
			break;
		case 2:
			doFire(employeeDB);
			break;
		case 3:
			doPromote(employeeDB);
			break;
		case 4:
			employeeDB.displayAll();
			break;
		case 5:
			employeeDB.displayCurrent();
			break;
		case 6:
			employeeDB.displayFormer();
			break;
		default:
			cerr << "Unknown command" << endl;
			break;
		}
	}
}

int displayMenu()
{
	// Note:
	//		One important note is that this code assumes that the user will
	//		"play nice" and type a number when a number is requested.
	//		When you read about I/O in Chapter 13, you will learn how to
	//		protect against bad input.

	int selection;

	cout << "\n"
	<< "Employee Database\n"
	<< "-----------------\n"
	<< "1) Hire a new employee\n"
	<< "2) Fire an employee\n"
	<< "3) Promote an employee\n"
	<< "4) List all employees\n"
	<< "5) List all current employees\n"
	<< "6) List all former employees\n"
	<< "0) Quit\n"
	<< "\n"
	<<"---> ";
	
	cin >> selection;
	
	return selection;
}

void doHire(Database& db)
{
	string firstName;
	string lastName;

	cout << "First name? ";
	cin >> firstName;

	cout << "Last name? ";
	cin >> lastName;

	auto& employee{ db.addEmployee(firstName, lastName) };
	cout << std::format("Hired employee {} {} with employee number {}.",
		firstName, lastName, employee.getEmployeeNumber());
}

void doFire(Database& db)
{
	int employeeNumber;
	cout << "Employee number? ";
	cin >> employeeNumber;

	try {
		auto& emp{ db.getEmployee(employeeNumber) };
		emp.fire();
		cout << format("Employee {} terminated.", employeeNumber);
	} catch (const std::logic_error& exception) {
		cerr << format("Unable to terminate employee: {}", exception.what());
	}
}

void doPromote(Database& db)
{
	int employeeNumber;
	cout << "Employee number? ";
	cin >> employeeNumber;

	int raiseAmount;
	cout << "How much of a raise? ";
	cin >> raiseAmount;

	try {
		auto& emp{ db.getEmployee(employeeNumber) };
		emp.promote(raiseAmount);
	} catch (const std::logic_error& exception) {
		cerr << format("Unable to promote employee: {}", exception.what());
	}
}
