//Jonathon Lewis
//5/11/2019
//Account.cpp
//Implements the Account class and all of its related subclasses.

#include "Account.h"
#include <stdexcept>

long Account::passwordToHash(std::string password) {
	long hash = 0;

	for (int i = 0; i < password.size(); i++) {
		hash += password[i] * i + 2;
	}

	return hash;
}

Account::Account(std::string name, std::string password, std::string username) {
	this->name = name;
	this->passwordHash = passwordToHash(password);
	this->username = username;
}

Account::Account(std::string name, std::string username, long passwordHash) {
	this->name = name;
	this->passwordHash = passwordHash;
	this->username = username;
}

std::string Account::getName() {
	return this->name;
}

long Account::getPasswordHash() {
	return this->passwordHash;
}

std::string Account::getUsername() {
	return this->username;
}

bool Account::login(std::string password) {
	if (passwordToHash(password) == getPasswordHash()) {
		return true;
	}

	else return false;
}

/*------------------------------------------------
ADMIN ACCOUNT
--------------------------------------------------*/

//Admin accounts don't initialize anything new, so we just call the parent class's constructor.
AdminAccount::AdminAccount(std::string name, std::string password, std::string username) : Account::Account(name, password, username) {}
AdminAccount::AdminAccount(std::string name, std::string username, long passwordHash) : Account::Account(name, username, passwordHash) {}

//returns a string representation of the administrator account.
std::string AdminAccount::toString() {
	return "Account Type: Admin - Name: " + this->getName();
}

/*------------------------------------------------
BASIC ACCOUNT
--------------------------------------------------*/

//Initialize ID for accounts

//Initialize variables and call parent constructor
BasicAccount::BasicAccount(std::string name, std::string password, std::string username) : Account::Account(name, password, username) {
	this->fines = 0.0;
	this->maxBorrowed = 4;

	this->borrowed = std::vector<BorrowableMaterial*>();
}

BasicAccount::BasicAccount(std::string name, std::string username, long passwordHash) : Account::Account(name, username, passwordHash) {
	this->fines = 0.0;
	this->maxBorrowed = 4;

	this->borrowed = std::vector<BorrowableMaterial*>();
}

//getters
//------------------
const double BasicAccount::getFeesOwed() {
	return this->fines;
}

const int BasicAccount::getAmountBorrowed() {
	return this->borrowed.size();
}

BorrowableMaterial* BasicAccount::getBorrowed(int index) {
	return this->borrowed[index];
}

const int BasicAccount::getMax() {
	return this->maxBorrowed;
}

//-----------------

//Borrows a book
void BasicAccount::checkOut(BorrowableMaterial &toBorrow) {

	if (this->fines == 0) {

		if (this->borrowed.size() < this->maxBorrowed) {

			if (toBorrow.isAvailabe()) {

				toBorrow.checkOut(*this);
				this->borrowed.push_back(&toBorrow);
			}

			else throw std::invalid_argument(toBorrow.getTitle() + " is not available");
		}

		else throw std::out_of_range(this->getName() + " has already borrowed their maximum amount of materials");
	}

	else {
		std::string fines = std::to_string(this->fines);
		throw std::out_of_range(this->getName() + " must repay their fine of $" + fines.substr(0, fines.find('.') + 3));
	}
}

//Returns a material.
void BasicAccount::returnMaterial(BorrowableMaterial &toReturn) {

	//Using auto to automatically retrieve the iterator type. We are using a simple sequential search
	//If we could borrow large amounts of items, it would be wise to use a binary search.
	for (auto it = this->borrowed.begin(); it != this->borrowed.end(); it++) {
		if (&toReturn == *it) {

			toReturn.returnThis();

			//will remove the pointer from the vector but DOES NOT destroy the pointer.
			this->borrowed.erase(it);
			break;
		}
	}
}

//Adds a fine to the account and returns the total fines that they owe.
double BasicAccount::fine(double amount) {
	if (this->fines >= 0) this->fines += amount;
	else throw std::invalid_argument("You cannot fine a negative amount");

	return this->fines;
}

//Pays fines on account, returns the amount that a person attempted to spend but did not (I don't think I'll use this functionality though)
double BasicAccount::payFine(double amount) {
	int amountRemaining = amount;

	if (amount >= 0) {
		amountRemaining -= fines;
		this->fines -= amount;

		if (this->fines < 0) this->fines = 0;
	}
	else throw std::invalid_argument("You cannot pay a negative amount");

	if (amountRemaining < 0) amountRemaining = 0;
	return amountRemaining;
}

void BasicAccount::addMaterial(BorrowableMaterial *toBorrow) {
	this->borrowed.push_back(toBorrow);
}

//Returns a string representation of this BasicAccount object
std::string BasicAccount::toString() {
	std::string fines = std::to_string(this->fines);

	return "Account Type: Basic - Name: " + this->getName() + " - Borrowed " + std::to_string(this->borrowed.size()) + " items - Owes $" +
		fines.substr(0, fines.find('.') + 3);
}


//operator overloads
//the += and -= operators are designed to work with both BorrowableMaterials and doubles
BasicAccount& BasicAccount::operator+=(BorrowableMaterial &toBorrow) {
	this->checkOut(toBorrow);

	return *this;
}

BasicAccount& BasicAccount::operator+=(const double fee) {
	this->fine(fee);

	return *this;
}

BasicAccount& BasicAccount::operator-=(BorrowableMaterial &toReturn) {
	this->returnMaterial(toReturn);

	return *this;
}

BasicAccount& BasicAccount::operator-=(const double fee) {
	this->payFine(fee);

	return *this;
}