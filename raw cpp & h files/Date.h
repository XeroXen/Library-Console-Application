//Jonathon Lewis
//5/11/2019
//Date.h
//The description for the Date class

#pragma once

#include <string>

//custom-made Date object to make working with dates in the program easier

class Date {

private:
	//store day, month, year
	int day;
	int month;
	int year;

	long calculateAllDays();
	int daysPerMonth(int month, int year);
	Date& calculateNewDate(int days);

public:
	//constructors
	Date(int day, int month, int year);
	Date(std::string date);

	//increments this date by a number of days
	Date& operator+=(const int &days);

	//Will return the difference in days between one Date and another.
	const int operator-(Date &otherDay);

	//Returns a boolean value if Date1 is before Date2
	std::string toString();

	//Compares two dates to determine if one is less than or equal to another.
	friend bool operator<=(const Date &otherDay, const Date &thisDay);
};

namespace DateOperation {
	static Date TODAY = Date(1, 1, 1);
}
