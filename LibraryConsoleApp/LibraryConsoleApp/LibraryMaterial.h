//Jonathon Lewis
//5/11/2019
//LibraryMaterial.h
//The description for the LibraryMaterial classes and its subclasses

#pragma once

#include <string>
#include "Account.h"
#include <vector>
#include <map>
#include "Date.h"

//forward declaration of the BasicAccount class, resolved some circular dependencies.
class BasicAccount;

class LibraryMaterial {
private:
	//private members that all materials have
	std::string title;
	std::string location;
	std::string author;

	double price;

public:
	//constructor
	LibraryMaterial(std::string title, std::string location, std::string author, double price);

	//getters
	const std::string getTitle();
	const std::string getLocation();
	const std::string getAuthor();
	const double getPrice();

	//setter
	void setLocation(std::string newLocation);

	//virtual function
	virtual std::string toString() = 0;

	//so we can sort
	const bool operator<(LibraryMaterial &otherMaterial);
	const bool operator==(LibraryMaterial &otherMaterial);
};

//Reference material cannot be checked out and thusly 
class ReferenceMaterial : public LibraryMaterial {
private:
	//reference material has an ISBN
	std::string isbn;
	int numPages;

public:
	//constructor
	ReferenceMaterial(std::string title, std::string location, std::string author, double price, std::string isbn, int numPages);

	//getters
	std::string getISBN();
	int getNumPages();

	//implementation of parent virtual function
	virtual std::string toString();
	const bool operator<(ReferenceMaterial &otherMaterial);
	const bool operator==(ReferenceMaterial &otherMaterial);
};

//An eBook is essentially just a book that has unlimited check-outs and a file format.
//However, to "borrow" it is mostly just to have access to it thanks to your library account,
//and therefore it's more similar to a reference material than an actual book (which inherits from borrowable material)
class EBook : public ReferenceMaterial {
public:
	//Enumeration type for different book formats
	enum Format { epub, ibook, pdf };

	//A map that will link Format enum-types to strings and vice versa
	const static std::map<Format, std::string> FormatMap;
	const static std::map<std::string, Format> StringMap;

private:
	Format bookFormat;

public:
	//constructor
	EBook(std::string title, std::string location, std::string author, double price, int numPages, std::string isbn, Format bookFormat);

	//getter
	Format getFormat();

	//implement toString
	std::string toString();
	const bool operator<(EBook &otherMaterial);
	const bool operator==(EBook &otherMaterial);
};

//Virtual function that encompasses all borrowable materials
class BorrowableMaterial : public LibraryMaterial {
private:
	bool available;
	Date* dueDate;
	BasicAccount* borrowedBy;

public:
	BorrowableMaterial(std::string title, std::string location, std::string author, double price);
	BorrowableMaterial(std::string title, std::string location, std::string author, double price, Date* dueDate, BasicAccount* borrowedBy);

	//getters
	const bool isAvailabe();
	Date* getDueDate();
	BasicAccount* getBorrowedBy();
	//getAccount

	void checkOut(BasicAccount &account);
	void returnThis();

	//pure virtual function
	virtual std::string toString() = 0;
};

//Basically a virtual implemenatation of the ReferenceMaterials class.
//Also can be borrowed. Will encompass all readable material.

//Does not have an ISBN because not all readable material has that.
class ReadableMaterial : public BorrowableMaterial {
private:
	int numPages;

public:
	//constructor
	ReadableMaterial(std::string title, std::string location, std::string author, double price, int numPages);
	ReadableMaterial(std::string title, std::string location, std::string author, double price, int numPages, Date *dueDate, BasicAccount *borrowedBy);

	//getter
	int getNumPages();

	//still a virtual class
	virtual std::string toString() = 0;
};

//Virtual class MultimediaMaterial extends BorrowableMaterial. All multimedia will come from here.
class MultimediaMaterial : public BorrowableMaterial {
private:
	//runtime stored in seconds
	int runTime;

public:
	//constructor
	MultimediaMaterial(std::string title, std::string location, std::string author, double price, int runTime);
	MultimediaMaterial(std::string title, std::string location, std::string author, double price, int runTime, Date *dueDate, BasicAccount *borrowedBy);

	//gets runtime as a string of "HH:MM:SS"
	std::string getRunTime();

	//All multimedia will extend this virtual class
	virtual std::string toString() = 0;
};

class Book : public ReadableMaterial {
private:
	std::string isbn;

public:
	//constructor
	Book(std::string title, std::string location, std::string author, double price, int numPages, std::string isbn);
	Book(std::string title, std::string location, std::string author, double price, int numPages, std::string isbn, Date* dueDate, BasicAccount* borrowedBy);

	//getter
	std::string getISBN();

	//implements toString
	std::string toString();

	const bool operator<(Book &otherMaterial);
	const bool operator==(Book &otherMaterial);
};

//Class magazine
class Magazine : public ReadableMaterial {
private:
	int issue;

public:
	//constructor
	Magazine(std::string title, std::string location, std::string author, double price, int numPages, int issue);
	Magazine(std::string title, std::string location, std::string author, double price, int numPages, int issue, Date* dueDate, BasicAccount* borrowedBy);

	//getters
	int getIssue();

	//implements toString
	std::string toString();

	const bool operator<(Magazine &otherMaterial);
	const bool operator==(Magazine &otherMaterial);
};

//Assumes that these movies are physical and not digital
class Movie : public MultimediaMaterial {
public:
	enum Format { vhs, dvd, hddvd, bluray };
	const static std::map<Format, std::string> FormatMap;
	const static std::map<std::string, Format> StringMap;

private:
	Format movieFormat;

public:
	Movie(std::string title, std::string location, std::string author, double price, int runTime, Format movieFormat);
	Movie(std::string title, std::string location, std::string author, double price, int runTime, Format movieFormat, Date* dueDate, BasicAccount* borrowedBy);

	//getter
	Format getFormat();

	//implementation of toString
	std::string toString();

	const bool operator<(Movie &otherMaterial);
	const bool operator==(Movie &otherMaterial);
};

//Again an assumption that these are physical mediums
class CD : public MultimediaMaterial {
private:
	int numTracks;
	std::vector<std::string> trackNames;

public:
	//overloaded constructor allows the writer of the program to input a number of tracks and then name them later, or
	//send in track names later
	CD(std::string title, std::string location, std::string author, double price, int runTime, int numTracks);
	CD(std::string title, std::string location, std::string author, double price, int runTime, std::vector<std::string> &trackNames);
	CD(std::string title, std::string location, std::string author, double price, int runTime, std::vector<std::string> &trackNames, Date *dueDate, BasicAccount *borrowedBy);

	//getters
	int getNumTracks();
	std::string getTrackName(int index);

	void setTrackName(int index, std::string trackName);

	//implement toString
	std::string toString();

	const bool operator<(CD &otherMaterial);
	const bool operator==(CD &otherMaterial);
};

//if borrowing from the library, a game is physical
class Game : public MultimediaMaterial {
public:
	enum Format { xbox360, xbone, ps3, ps4, ninwii, ninwiiu, ninswitch };
	const static std::map<Format, std::string> FormatMap;
	const static std::map<std::string, Format> StringMap;

private:
	Format gameConsole;
	std::string publisher;

public:
	Game(std::string title, std::string location, std::string author, double price, int runTime , Format gameConsole);
	Game(std::string title, std::string location, std::string author, double price, int runTime, Format gameConsole, Date* dueDate, BasicAccount* borrowedBy);

	//getters
	Format getFormat();

	//implement toString
	std::string toString();

	const bool operator<(Game &otherMaterial);
	const bool operator==(Game &otherMaterial);
};
