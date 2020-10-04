//Jonathon Lewis
//5/11/2019
//Date.cpp
//Implements the Date class

#include "Date.h"
#include <stdexcept>

//Easy constructor
Date::Date(int day, int month, int year) {
	if (day > daysPerMonth(month, year) || day < 1) throw std::out_of_range("Date is invalid");
	else if (month < 1 || month > 12) throw std::out_of_range("Date is invalid");
	else if (year < 0 || year > 9999) throw std::out_of_range("Date is invalid");
	this->day = day;
	this->month = month;
	this->year = year;
}

//assumes string coming in is in format "DD:MM:YYYY"
//parses string and makes sure the dates are within the correct range.
Date::Date(std::string date) {
	int day = std::stoi(date.substr(0, 2));
	int month = std::stoi(date.substr(3, 2));
	int year = std::stoi(date.substr(6, date.size() - 6));

	if (month < 1 || month > 12) throw std::out_of_range("Date is not in range");
	if (year < 0 || year > 9999) throw std::out_of_range("Date is not in range");

	if (day < 1 || (month == 1 || month == 3 || month == 5 || month == 7 || month == 8 || month == 10 || month == 12) && day > 31 ||
		(month == 4 || month == 6 || month == 9 || month == 11) && day > 30 || month == 2 && year % 4 == 0 && day > 29 || month == 2 && day > 28)
		throw std::out_of_range("Date is not in range");

	this->day = day;
	this->month = month;
	this->year = year;
}

Date& Date::operator+=(const int &days) {
	return calculateNewDate(days);
}

//works
const int Date::operator-(Date &otherday) {
	long totalDays = 0;

	if (otherday <= *this) {
		return this->calculateAllDays() - otherday.calculateAllDays();
	}
	else
	{
		return otherday - *this;
	}

	return totalDays;
}

int Date::daysPerMonth(int month, int year) {
	if (month == 1 || month == 3 || month == 5 || month == 7 || month == 8 || month == 10 || month == 12) return 31;
	else if (month == 4 || month == 6 || month == 9 || month == 11) return 30;

	//checks for leap year
	else return 28 + (year % 4 == 0);
}

long Date::calculateAllDays() {
	long totalDays = this->year * 365;
	totalDays += year / 4;

	for (int i = 12; i > this->month; i--) {
		totalDays -= daysPerMonth(this->month, this->year);
	}

	for (int i = daysPerMonth(this->month, this->year); i > this->day; i--) {
		totalDays -= 1;
	}

	return totalDays;
}

//now works!
Date& Date::calculateNewDate(int days) {
	this->day += days;
	while (day > daysPerMonth(this->month, this->year)) {
		if (this->month == 12) {
			day -= daysPerMonth(this->month, this->year);
			this->month = 1;
			this->year += 1;
		}
		else {
			day -= daysPerMonth(this->month++, this->year);
		}
	}

	return *this;
}

//works
bool operator<=(const Date &thisDate, const Date &otherDate) {
	if (thisDate.year < otherDate.year) return true;
	else if (thisDate.year == otherDate.year) {
		if (thisDate.month < otherDate.month) return true;
		else if (thisDate.month == otherDate.month) {
			if (thisDate.day <= otherDate.day) return true;
			else return false;
		}
		else return false;
	}
	else return false;
}

std::string Date::toString() {
	return (this->day < 10 ? "0" : "") + std::to_string(this->day) + ":" + (this->month < 10 ? "0" : "") + std::to_string(this->month) + ":" + std::to_string(this->year);
}
