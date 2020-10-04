//Jonathon Lewis
//5/11/2019
//LibraryMaterial.cpp
//Implement the LibraryMaterial class and all of its related subclasses.

#include "LibraryMaterial.h"
#include <typeinfo>

/* ABSTRACT CLASS LIBRARY MATERIAL
----------------------------------------------------------------------------------------------------------------*/

LibraryMaterial::LibraryMaterial(std::string title, std::string location, std::string author, double price) {
	this->title = title;
	this->location = location;
	this->author = author;
	this->price = price;
}

const std::string LibraryMaterial::getTitle() {
	return this->title;
}

const std::string LibraryMaterial::getLocation() {
	return this->location;
}

const std::string LibraryMaterial::getAuthor() {
	return this->author;
}

const double LibraryMaterial::getPrice() {
	return this->price;
}

void LibraryMaterial::setLocation(std::string location) {
	this->location = location;
}

const bool LibraryMaterial::operator<(LibraryMaterial &otherMaterial) {

	if (!(typeid(*this) == typeid(otherMaterial))) {
		if (typeid(*this).before(typeid(otherMaterial))) return true;
		else return false;
	}

	std::string className = typeid(*this).name();

	if (className == "class ReferenceMaterial") {
		ReferenceMaterial *thisBook = dynamic_cast<ReferenceMaterial*>(this);
		ReferenceMaterial *otherBook = dynamic_cast<ReferenceMaterial*>(&otherMaterial);

		return (*thisBook < *otherBook);
	}

	else if (className == "class EBook") {
		EBook *thisBook = dynamic_cast<EBook*>(this);
		EBook *otherBook = dynamic_cast<EBook*>(&otherMaterial);

		return (*thisBook < *otherBook);
	}

	else if (className == "class Book") {
		Book *thisBook = dynamic_cast<Book*>(this);
		Book *otherBook = dynamic_cast<Book*>(&otherMaterial);

		return (*thisBook < *otherBook);
	}

	else if (className == "class Magazine") {
		Magazine *thisBook = dynamic_cast<Magazine*>(this);
		Magazine *otherBook = dynamic_cast<Magazine*>(&otherMaterial);

		return (*thisBook < *otherBook);
	}

	else if (className == "class Movie") {
		Movie *thisBook = dynamic_cast<Movie*>(this);
		Movie *otherBook = dynamic_cast<Movie*>(&otherMaterial);

		return (*thisBook < *otherBook);
	}

	else if (className == "class CD") {
		CD *thisBook = dynamic_cast<CD*>(this);
		CD *otherBook = dynamic_cast<CD*>(&otherMaterial);

		return (*thisBook < *otherBook);
	}

	else {
		Game *thisBook = dynamic_cast<Game*>(this);
		Game *otherBook = dynamic_cast<Game*>(&otherMaterial);

		return (*thisBook < *otherBook);
	}
}

const bool LibraryMaterial::operator==(LibraryMaterial &otherMaterial) {
	if (!(typeid(*this) == typeid(otherMaterial))) return false;

	std::string className = typeid(*this).name();

	if (className == "class ReferenceMaterial") {
		ReferenceMaterial *thisBook = dynamic_cast<ReferenceMaterial*>(this);
		ReferenceMaterial *otherBook = dynamic_cast<ReferenceMaterial*>(&otherMaterial);

		return (*thisBook == *otherBook);
	}

	else if (className == "class EBook") {
		EBook *thisBook = dynamic_cast<EBook*>(this);
		EBook *otherBook = dynamic_cast<EBook*>(&otherMaterial);

		return (*thisBook == *otherBook);
	}

	else if (className == "class Book") {
		Book *thisBook = dynamic_cast<Book*>(this);
		Book *otherBook = dynamic_cast<Book*>(&otherMaterial);

		return (*thisBook == *otherBook);
	}

	else if (className == "class Magazine") {
		Magazine *thisBook = dynamic_cast<Magazine*>(this);
		Magazine *otherBook = dynamic_cast<Magazine*>(&otherMaterial);

		return (*thisBook == *otherBook);
	}

	else if (className == "class Movie") {
		Movie *thisBook = dynamic_cast<Movie*>(this);
		Movie *otherBook = dynamic_cast<Movie*>(&otherMaterial);

		return (*thisBook == *otherBook);
	}

	else if (className == "class CD") {
		CD *thisBook = dynamic_cast<CD*>(this);
		CD *otherBook = dynamic_cast<CD*>(&otherMaterial);

		return (*thisBook == *otherBook);
	}

	else {
		Game *thisBook = dynamic_cast<Game*>(this);
		Game *otherBook = dynamic_cast<Game*>(&otherMaterial);

		return (*thisBook == *otherBook);
	}
}


/*----------------------------------------------------------------------------------------------------------------

ABSTRACT CLASS BORROWABLE MATERIAL
----------------------------------------------------------------------------------------------------------------*/

BorrowableMaterial::BorrowableMaterial(std::string title, std::string location, std::string author, double price) : LibraryMaterial(title, location, author, price) {
	this->available = true;
}

BorrowableMaterial::BorrowableMaterial(std::string title, std::string location, std::string author, double price, Date* dueDate, BasicAccount* borrowedBy) : LibraryMaterial(title, location, author, price) {
	this->available = false;
	this->dueDate = dueDate;
	this->borrowedBy = borrowedBy;

	borrowedBy->addMaterial(this);
}

const bool BorrowableMaterial::isAvailabe() {
	return this->available;
}

Date* BorrowableMaterial::getDueDate() {
	return this->dueDate;
}

BasicAccount* BorrowableMaterial::getBorrowedBy() {
	return this->borrowedBy;
}

void BorrowableMaterial::checkOut(BasicAccount &account) {
	if (this->available) {
		this->borrowedBy = &account;
		this->available = false;


		this->dueDate = new Date(DateOperation::TODAY);
		*(this->dueDate) += 21;
	}
}

void BorrowableMaterial::returnThis() {
	this->borrowedBy = NULL;
	this->available = true;

	//deletes the Date that it stores
	delete this->dueDate;
}

/*----------------------------------------------------------------------------------------------------------------

ABSTRACT CLASS READABLE MATERIAL
----------------------------------------------------------------------------------------------------------------*/

ReadableMaterial::ReadableMaterial(std::string name, std::string location, std::string author, double price, int numPages) : BorrowableMaterial(name, location, author, price) {
	this->numPages = numPages;
}

ReadableMaterial::ReadableMaterial(std::string name, std::string location, std::string author, double price, int numPages, Date *dueDate, BasicAccount *borrowedBy) : BorrowableMaterial(name, location, author, price, dueDate, borrowedBy) {
	this->numPages = numPages;
}

int ReadableMaterial::getNumPages() {
	return this->numPages;
}

/*----------------------------------------------------------------------------------------------------------------

ABSTRACT CLASS MULTIMEDIA MATERIAL
----------------------------------------------------------------------------------------------------------------*/

MultimediaMaterial::MultimediaMaterial(std::string title, std::string location, std::string author, double price, int runTime) : BorrowableMaterial(title, location, author, price) {
	this->runTime = runTime;
}

MultimediaMaterial::MultimediaMaterial(std::string title, std::string location, std::string author, double price, int runTime, Date *dueDate, BasicAccount *borrowedBy) : BorrowableMaterial(title, location, author, price, dueDate, borrowedBy) {
	this->runTime = runTime;
}


std::string MultimediaMaterial::getRunTime() {
	int seconds = this->runTime;

	int hours = seconds / 3600;
	seconds -= hours * 3600;

	int minutes = seconds / 60;
	seconds -= minutes * 60;

	return std::to_string(hours) + ":" + (minutes > 10 ? std::to_string(minutes) : "0" + std::to_string(minutes)) + ":" + (seconds > 10 ? std::to_string(seconds) : "0" + std::to_string(seconds));
}

/*----------------------------------------------------------------------------------------------------------------

CLASS REFERENCE MATERIAL
----------------------------------------------------------------------------------------------------------------*/

ReferenceMaterial::ReferenceMaterial(std::string title, std::string location, std::string author, double price, std::string isbn, int numPages) : LibraryMaterial(title, location, author, price) {
	this->isbn = isbn;
	this->numPages = numPages;
}

std::string ReferenceMaterial::getISBN() {
	return this->isbn;
}

int ReferenceMaterial::getNumPages() {
	return this->numPages;
}

std::string ReferenceMaterial::toString() {
	return "Type: Reference Material - Title: " + this->getTitle() + " - Author: " + this->getAuthor() + " - Location: " + this->getLocation() + "\n" +
		"ISBN: " + this->isbn + " - Number of pages: " + std::to_string(this->numPages);
}

const bool ReferenceMaterial::operator<(ReferenceMaterial &otherMaterial) {
	if (this->isbn < otherMaterial.isbn) return true;
	else return false;
}

const bool ReferenceMaterial::operator==(ReferenceMaterial &otherMaterial) {
	return (this->isbn == otherMaterial.isbn);
}

/*----------------------------------------------------------------------------------------------------------------

CLASS EBOOK
----------------------------------------------------------------------------------------------------------------*/

const std::map<EBook::Format, std::string> EBook::FormatMap = { {EBook::epub, "Epub"}, {EBook::ibook, "Ibook"}, {EBook::pdf, "PDF"} };
const std::map<std::string, EBook::Format> EBook::StringMap = { {"Epub", EBook::epub}, {"Ibook", EBook::ibook}, {"PDF", EBook::pdf} };

EBook::EBook(std::string title, std::string location, std::string author, double price, int numPages, std::string isbn, Format bookFormat) : ReferenceMaterial(title, location, author, price, isbn, numPages) {
	this->bookFormat = bookFormat;
}

EBook::Format EBook::getFormat() {
	return this->bookFormat;
}

std::string EBook::toString() {
	return "Type: EBook - Title: " + this->getTitle() + " - Author: " + this->getAuthor() + " - Format: " + EBook::FormatMap.at(this->bookFormat) + "\n" +
		"ISBN: " + this->getISBN() + " - Number of pages: " + std::to_string(this->getNumPages());
}

const bool EBook::operator<(EBook &otherMaterial) {
	if (this->bookFormat == otherMaterial.bookFormat) {
		if (this->getISBN() < otherMaterial.getISBN()) return true;
		else return false;
	}
	else return (this->bookFormat < otherMaterial.bookFormat);
}

const bool EBook::operator==(EBook &otherMaterial) {
	if (this->bookFormat == otherMaterial.bookFormat) {
		if (this->getISBN() == otherMaterial.getISBN()) return true;
		else return false;
	}
}

/*----------------------------------------------------------------------------------------------------------------

CLASS BOOK
----------------------------------------------------------------------------------------------------------------*/

Book::Book(std::string title, std::string location, std::string author, double price, int numPages, std::string isbn) : ReadableMaterial(title, location, author, price, numPages) {
	this->isbn = isbn;
}

Book::Book(std::string title, std::string location, std::string author, double price, int numPages, std::string isbn, Date *dueDate, BasicAccount *borrowedBy) : ReadableMaterial(title, location, author, price, numPages, dueDate, borrowedBy) {
	this->isbn = isbn;
}

std::string Book::getISBN() {
	return this->isbn;
}

std::string Book::toString() {
	return "Type: Book - Title: " + this->getTitle() + " - Author: " + this->getAuthor() + " - Location: " + this->getLocation() + "\n" +
		"ISBN: " + this->getISBN() + " - Number of pages: " + std::to_string(this->getNumPages()) + " [Is " + (this->isAvailabe() ? "" : "not ") + "available]";
}

const bool Book::operator<(Book &otherMaterial) {
	return (this->isbn < otherMaterial.isbn);
}

const bool Book::operator==(Book &otherMaterial) {
	return (this->isbn == otherMaterial.isbn);
}

/*----------------------------------------------------------------------------------------------------------------

CLASS MAGAZINE
----------------------------------------------------------------------------------------------------------------*/

Magazine::Magazine(std::string title, std::string location, std::string author, double price, int numPages, int issue) : ReadableMaterial(title, location, author, price, numPages) {
	this->issue = issue;
}

Magazine::Magazine(std::string title, std::string location, std::string author, double price, int numPages, int issue, Date *dueDate, BasicAccount *borrowedBy) : ReadableMaterial(title, location, author, price, numPages, dueDate, borrowedBy) {
	this->issue = issue;
}

int Magazine::getIssue() {
	return this->issue;
}

std::string Magazine::toString() {
	return "Type: Magazine - Title: " + this->getTitle() + " #" + std::to_string(this->issue) + " - Location: " + this->getLocation() + "\n" +
		"Publisher: " + this->getAuthor() + " - Number of pages: " + std::to_string(this->getNumPages()) + " [Is " + (this->isAvailabe() ? "" : "not ") + "available]";
}

const bool Magazine::operator<(Magazine &otherMaterial) {
	if (this->getTitle() == otherMaterial.getTitle()) return (this->issue < otherMaterial.issue);
	else return (this->getTitle() < otherMaterial.getTitle());
}

const bool Magazine::operator==(Magazine &otherMaterial) {
	if (this->getTitle() == otherMaterial.getTitle()) return (this->issue == otherMaterial.issue);
	else return false;
}

/*----------------------------------------------------------------------------------------------------------------

CLASS MOVIE
----------------------------------------------------------------------------------------------------------------*/

const std::map<Movie::Format, std::string> Movie::FormatMap = { {vhs, "VHS"}, {dvd, "DVD"}, {hddvd, "HD-DVD"}, {bluray, "BluRay"} };
const std::map<std::string, Movie::Format> Movie::StringMap = { { "VHS", vhs }, {"DVD", dvd}, {"HD-DVD", hddvd}, {"BluRay", bluray} };

Movie::Movie(std::string title, std::string location, std::string author, double price, int runTime, Format movieFormat) : MultimediaMaterial(title, location, author, price, runTime) {
	this->movieFormat = movieFormat;
}

Movie::Movie(std::string title, std::string location, std::string author, double price, int runTime, Format movieFormat, Date *dueDate, BasicAccount *borrowedBy) : MultimediaMaterial(title, location, author, price, runTime, dueDate, borrowedBy) {
	this->movieFormat = movieFormat;
}

Movie::Format Movie::getFormat() {
	return this->movieFormat;
}

std::string Movie::toString() {
	return "Type: Movie - Title: " + this->getTitle() + " - Publisher: " + this->getAuthor() + " - Location: " + this->getLocation() + "\n" +
		"Format: " + Movie::FormatMap.at(this->movieFormat) + " - Run Time: " + this->getRunTime() + " [Is " + (this->isAvailabe() ? "" : "not ") + "available]";
}

const bool Movie::operator<(Movie &otherMaterial) {
	if (this->movieFormat == otherMaterial.movieFormat) return (this->getTitle() < otherMaterial.getTitle());
	else return (this->movieFormat < otherMaterial.movieFormat);
}

const bool Movie::operator==(Movie &otherMaterial) {
	if (this->movieFormat == otherMaterial.movieFormat) return (this->getTitle() == otherMaterial.getTitle());
	else return false;
}

/*----------------------------------------------------------------------------------------------------------------

CLASS CD
----------------------------------------------------------------------------------------------------------------*/

CD::CD(std::string title, std::string location, std::string author, double price, int runTime, int numTracks) : MultimediaMaterial(title, location, author, price, runTime) {
	this->numTracks = numTracks;

	this->trackNames = std::vector<std::string>(numTracks);
}

CD::CD(std::string title, std::string location, std::string author, double price, int runTime, std::vector<std::string> &trackNames) : MultimediaMaterial(title, location, author, price, runTime) {
	this->trackNames = trackNames;

	this->numTracks = trackNames.size();
}

CD::CD(std::string title, std::string location, std::string author, double price, int runTime, std::vector<std::string> &trackNames, Date *dueDate, BasicAccount *borrowedBy) : MultimediaMaterial(title, location, author, price, runTime, dueDate, borrowedBy) {
	this->trackNames = trackNames;

	this->numTracks = trackNames.size();
}

int CD::getNumTracks() {
	return this->numTracks;
}

std::string CD::getTrackName(int index) {
	return this->trackNames[index];
}

void CD::setTrackName(int index, std::string trackName) {
	this->trackNames[index] = trackName;
}

std::string CD::toString() {
	std::string ret = "Type: CD - Title: " + this->getTitle() + " - Artist: " + this->getAuthor() + " - Location: " + this->getLocation() + "\n" +
		"Total Length: " + this->getRunTime() + " [Is " + (this->isAvailabe() ? "" : "not ") + "available]";

	for (int i = 0; i < this->numTracks; i++) {
		ret += "\nTrack " + std::to_string(i + 1) + ": " + this->getTrackName(i);
	}

	return ret;
}

const bool CD::operator<(CD &otherMaterial) {
	if (this->getAuthor() == otherMaterial.getAuthor()) return (this->getTitle() < otherMaterial.getTitle());
	else return (this->getAuthor() < otherMaterial.getAuthor());
}

const bool CD::operator==(CD &otherMaterial) {
	if (this->getAuthor() == otherMaterial.getAuthor()) return (this->getTitle() == otherMaterial.getTitle());
	else return false;
}

/*----------------------------------------------------------------------------------------------------------------

CLASS GAME
----------------------------------------------------------------------------------------------------------------*/

const std::map<Game::Format, std::string> Game::FormatMap = { {xbox360, "Xbox 360"}, {xbone, "Xbox One"}, {ps3, "Playstation 3"}, {ps4, "Playstation 4"}, {ninwii, "Nintendo Wii"},
{ninwiiu, "Nintendo Wii U"}, {ninswitch, "Nintendo Switch"} };

const std::map<std::string, Game::Format> Game::StringMap = { {"Xbox 360", xbox360}, {"Xbox One", xbone}, {"Playstation 3", ps3}, {"Playstation 4", ps4}, {"Nintendo Wii", ninwii},
{"Nintendo Wii U", ninwiiu}, {"Nintendo Switch", ninswitch} };

Game::Game(std::string title, std::string location, std::string author, double price, int runTime, Format gameConsole) : MultimediaMaterial(title, location, author, price, runTime) {
	this->gameConsole = gameConsole;
}

Game::Game(std::string title, std::string location, std::string author, double price, int runTime, Format gameConsole, Date *dueDate, BasicAccount *borrowedBy) : MultimediaMaterial(title, location, author, price, runTime, dueDate, borrowedBy) {
	this->gameConsole = gameConsole;
}

Game::Format Game::getFormat() {
	return this->gameConsole;
}

std::string Game::toString() {
	return "Type: Game - Title : " + this->getTitle() + " - Publisher : " + this->getAuthor() + " - Location : " + this->getLocation() + "\n" +
		"Console: " + Game::FormatMap.at(this->gameConsole) + " - Playtime: " + this->getRunTime() + " [Is " + (this->isAvailabe() ? "" : "not ") + "available]";
}

const bool Game::operator<(Game &otherMaterial) {
	if (this->gameConsole == otherMaterial.gameConsole) {
		return (this->getTitle() < otherMaterial.getTitle());
	}
	else return (this->gameConsole < otherMaterial.gameConsole);
}

const bool Game::operator==(Game &otherMaterial) {
	if (this->gameConsole == otherMaterial.gameConsole) {
		return (this->getTitle() == otherMaterial.getTitle());
	}
	else return false;
}