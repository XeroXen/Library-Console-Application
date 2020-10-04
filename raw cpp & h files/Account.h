//Jonathon Lewis
//5/11/2019
//Account.h
//The description for the Account class and its subclasses

#pragma once

#include <string>
#include <vector>
#include "LibraryMaterial.h"

class BorrowableMaterial;

class Account {
private:
	std::string name;
	long passwordHash;
	std::string username;
public:
	Account(std::string name, std::string password, std::string username);
	Account(std::string name, std::string username, long passwordHash);

	std::string getName();
	std::string getUsername();
	long getPasswordHash();

	bool login(std::string password);

	//pure virtual function toString
	virtual std::string toString() = 0;

protected:
	long passwordToHash(std::string password);
};

//In this implementation, administrator accounts do not need to do much except login to access
//the admin menu. So their implementation is mostly just an override of the toString method.
class AdminAccount : public Account {
public:
	AdminAccount(std::string name, std::string password, std::string username);
	AdminAccount(std::string name, std::string username, long passwordHash);

	//implementation of login. Basically will just return true.
	bool login(std::string password);

	//implementation of toString
	std::string toString();
};


//Basic accounts can borrow books, return books, and pay fines.
class BasicAccount : public Account {
private:
	double fines;
	int maxBorrowed;

	std::vector<BorrowableMaterial*> borrowed;

public:
	BasicAccount(std::string name, std::string password, std::string username);
	BasicAccount(std::string name, std::string username, long passwordHash);

	//getters
	const double getFeesOwed();
	const int getAmountBorrowed();
	BorrowableMaterial* getBorrowed(int index);
	const int getMax();

	//Manipulators
	void checkOut(BorrowableMaterial &toBorrow);
	void returnMaterial(BorrowableMaterial &toReturn);
	double fine(double amount);
	double payFine(double amount);
	void addMaterial(BorrowableMaterial *toBorrow);

	//inherited virtual functions
	bool login(std::string password);
	std::string toString();

	//Operator overloading.
	BasicAccount& operator+=(BorrowableMaterial &toBorrow);

	BasicAccount& operator+=(const double fee);

	BasicAccount& operator-=(BorrowableMaterial &toReturn);

	BasicAccount& operator-=(const double fee);
};