//Jonathon Lewis
//5/11/2019
//Source.cpp
//Implement the library program.

#include <iostream>
#include <vector>
#include "Account.h"
#include "LibraryMaterial.h"
#include <algorithm>
#include <typeinfo>
#include <map>
#include <fstream>

using namespace std;

bool compareLibraryMaterials(LibraryMaterial* firstMaterial, LibraryMaterial* secondMaterial);
bool showBasicMenu(BasicAccount& user, vector<LibraryMaterial*>& library);
void showLibraryMaterials(Account* user, vector<LibraryMaterial*>& library);
void viewBook(Account* user, vector<LibraryMaterial*> library, int index);
int validateInt(int min, int max, string message);
void borrowBook(BasicAccount& user, BorrowableMaterial* toBorrow);
void showBorrowedMaterials(BasicAccount& user);
void showFeesMenu(BasicAccount& user);
double validateDouble(double min, double max, string message);
vector<LibraryMaterial*> readLibraryFromFile(vector<Account*>& accounts);
vector<Account*> readAccountsFromFile();
LibraryMaterial* createNewMaterial();

bool showAdminMenu(AdminAccount& user, vector<LibraryMaterial*>& library);

int loginScreen(vector<Account*>& accountsVector, vector<LibraryMaterial*>& library);
int loginUser(vector<Account*>& accountsVector);

Account* createAccount();
bool attemptLogin(Account* user);

void writeAccountsToFile(vector<Account*>& accounts);
void writeLibraryToFile(vector<LibraryMaterial*> library);

string clearScreen();

int main()
{
	cout << clearScreen();
	Date* todayDate;
	while (true) {
		cout << "Enter today's date in the format (DD:MM:YYYY): ";
		string date;
		getline(cin, date);

		try {
			todayDate = new Date(date);
			break;
		}
		catch (exception& e) {
			cout << e.what() << endl << endl;
		}
	}

	DateOperation::TODAY = *todayDate;
	delete todayDate;

	vector<Account*> allAccounts = readAccountsFromFile();
	vector<LibraryMaterial*> thisLibrary = readLibraryFromFile(allAccounts);

	sort(thisLibrary.begin(), thisLibrary.end(), compareLibraryMaterials);

	while (true) {
		int choice = loginScreen(allAccounts, thisLibrary);
		if (choice == -1) break;
	}

	writeAccountsToFile(allAccounts);
	writeLibraryToFile(thisLibrary);

	system("pause");
}

//Used as our method for the standard library's sort method.
bool compareLibraryMaterials(LibraryMaterial* firstMaterial, LibraryMaterial* secondMaterial) {
	return (*firstMaterial < *secondMaterial);
}

bool showBasicMenu(BasicAccount& user, vector<LibraryMaterial*>& library) {
	cout << "1) Show Library Materials" << endl
		<< "2) Show My Borrowed Materials" << endl
		<< "3) Pay Fees" << endl
		<< "4) Logout" << endl << endl;

	int input = validateInt(1, 4, "Enter an integer representing your command: ");

	if (input == 1) showLibraryMaterials(&user, library);
	else if (input == 2) showBorrowedMaterials(user);
	else if (input == 3) showFeesMenu(user);
	else return false;
}

void showLibraryMaterials(Account* user, vector<LibraryMaterial*>& library) {

	map<int, int> allowedIndexes;

	cout << clearScreen() <<
		"1) Show all materials" << endl
		<< "2) Show materials by type" << endl
		<< "3) Back to user menu" << endl << endl;

	int input = validateInt(1, 3, "Enter an integer representing your command: ");

	if (input == 1) {

		cout << clearScreen();
		for (unsigned int i = 0; i < library.size(); i++) {
			allowedIndexes[i + 1] = i;
			cout << i + 1 << ") " << library[i]->toString() << endl << endl;
		}
	}
	else if (input == 2) {

		cout << clearScreen();
		cout << "1) Show all books" << endl
			<< "2) Show all reference material" << endl
			<< "3) Show all eBooks" << endl
			<< "4) Show all Movies" << endl
			<< "5) Show all CDs" << endl
			<< "6) Show all Games" << endl
			<< "7) Go back to user menu" << endl << endl;

		input = validateInt(1, 7, "Enter an integer representing your command: ");

		string toFind = "class ";
		if (input == 7) {
			cout << clearScreen();
			return;
		}

		else if (input == 1) {
			toFind += "Book";
		}

		else if (input == 2) {
			toFind += "ReferenceMaterial";
		}

		else if (input == 3) {
			toFind += "EBook";
		}

		else if (input == 4) {
			toFind += "Movie";
		}

		else if (input == 5) {
			toFind += "CD";
		}

		else toFind += "Game";

		int found = 0;
		for (int i = 0; i < library.size(); i++) {
			string className = typeid(*library[i]).name();
			if (className == toFind) {
				cout << ++found << ") " << library[i]->toString() << endl << endl;
				allowedIndexes[found] = i;
			}
		}
	}
	else { cout << clearScreen(); return; }

	cout << "1) View Book" << endl;
	cout << "2) Back to user menu" << endl << endl;

	input = validateInt(1, 2, "Enter an integer representing your command: ");
	if (input == 2) { cout << clearScreen(); return; }
	else {
		cout << endl;

		auto beginIndexes = allowedIndexes.end();
		beginIndexes--;

		input = validateInt(0, (*beginIndexes).first, "Enter an integer representing the material you would like to view (0 to cancel): ");
		if (input == 0) { cout << clearScreen();  return; }
		else viewBook(user, library, allowedIndexes.at(input));
	}
}

void viewBook(Account* user, vector<LibraryMaterial*> library, int index) {
	cout << clearScreen() << library[index]->toString() << endl << endl;

	string accountType = typeid(*user).name();
	string classType = typeid(*library[index]).name();

	if (accountType == "class AdminAccount") {
		//Show deletion option
		cout << "1) Erase this material" << endl
			<< "2) Back to user menu" << endl << endl;

		int input = validateInt(1, 2, "Enter an integer representing your command: ");
		if (input == 2) {
			cout << clearScreen();
			return;
		}
		else {
			AdminAccount* thisUser = dynamic_cast<AdminAccount*>(user);
			//call delete and send user
		}
	}
	else if (classType == "class Book" || classType == "class Movie" || classType == "class Magazine" || classType == "class CD" || classType == "class Game") {
		//Show borrowable option if it is an available book.
		BorrowableMaterial* thisMaterial = dynamic_cast<BorrowableMaterial*>(library[index]);


		if (thisMaterial->isAvailabe()) {
			BasicAccount* thisUser = dynamic_cast<BasicAccount*>(user);
			borrowBook(*thisUser, thisMaterial);
		}
		else {
			cout << "This material is already being borrowed. Please enter any integer to exit this viewing: ";
			int input;

			cin >> input;
			cin.clear();
			cin.ignore(INT_MAX, '\n');

			cout << clearScreen();
		}
	}
	else {
		cout << "This material is read only. Please enter any integer to exit this viewing: ";
		int input;

		cin >> input;
		cin.clear();
		cin.ignore(INT_MAX, '\n');

		cout << clearScreen();
	}
}

void borrowBook(BasicAccount& user, BorrowableMaterial* toBorrow) {
	cout << "1) Borrow material" << endl
		<< "2) Back to user menu" << endl << endl;

	int input = validateInt(1, 2, "Enter an integer representing your command: ");
	if (input == 2) {
		cout << clearScreen();
		return;
	}
	else {
		try {
			user += *toBorrow;
			Date dueDate = *toBorrow->getDueDate();
			cout << clearScreen() << "Material successfully borrowed! Due date is: " << dueDate.toString() << endl << endl;
		}
		catch (exception& e) {
			cout << clearScreen() << e.what() << endl << endl;
		}
	}
}

void showBorrowedMaterials(BasicAccount& user) {
	cout << clearScreen();

	cout << "Borrowed " << user.getAmountBorrowed() << " out of a max of " << user.getMax() << " items." << endl << endl;
	for (int i = 0; i < user.getAmountBorrowed(); i++) {
		Date dueDate = *user.getBorrowed(i)->getDueDate();

		cout << i + 1 << ") " << (DateOperation::TODAY <= dueDate ? "" : "LATE ITEM\n") << user.getBorrowed(i)->toString() << endl << endl;
	}

	int input = validateInt(0, user.getAmountBorrowed(), "Enter an integer to return the corresponding material (0 to exit): ");

	if (input == 0) {
		cout << clearScreen();
		return;
	}
	else {
		user -= *user.getBorrowed(input - 1);
		cout << clearScreen() << "Material successfully returned!" << endl << endl;
	}
}

void showFeesMenu(BasicAccount& user) {

	string fees = to_string(user.getFeesOwed());
	fees = fees.substr(0, fees.find('.') + 3);

	cout << clearScreen()
		<< "Account owned by " << user.getName() << " owes $" << fees << " in fees to the library." << endl << endl;

	double input = validateDouble(0.00, user.getFeesOwed(), "Enter an amount you would like to pay (in the range 0.00 - " + fees + "): ");
	try {
		user.payFine(input);
		cout << clearScreen() << "Amount successfully paid" << endl << endl;
	}
	catch (exception& e) {
		cout << clearScreen() << e.what() << endl << endl;
	}
}

//gets input of type int and validates it against a range.
int validateInt(int min, int max, string message) {
	cout << message;

	int input;
	while (!(cin >> input) || input < min || input > max) {
		if (!cin) {
			cin.clear();
			cin.ignore(INT_MAX, '\n');
		}
		cout << "Invalid input. " << message;
	}

	return input;
}

double validateDouble(double min, double max, string message) {
	cout << message;

	double input;
	while (!(cin >> input) || input < min || input > max) {
		if (!cin) {
			cin.clear();
			cin.ignore(INT_MAX, '\n');
		}
		cout << "Invalid input. " << message;
	}

	return input;
}

void writeLibraryToFile(vector<LibraryMaterial*> library) {
	ofstream outputStream;

	outputStream.open("library.txt", ifstream::in | ifstream::trunc);

	for (int i = 0; i < library.size(); i++) {
		string classType = typeid(*library[i]).name();

		outputStream << classType << endl;

		if (classType == "class ReferenceMaterial") {
			ReferenceMaterial* thisMaterial = dynamic_cast<ReferenceMaterial*>(library[i]);
			outputStream << thisMaterial->getTitle() << endl << thisMaterial->getLocation() << endl
				<< thisMaterial->getAuthor() << endl << thisMaterial->getPrice() << endl << thisMaterial->getISBN() << endl << thisMaterial->getNumPages() << endl;
		}

		else if (classType == "class EBook") {
			EBook* thisMaterial = dynamic_cast<EBook*>(library[i]);
			outputStream << thisMaterial->getTitle() << endl << thisMaterial->getLocation() << endl
				<< thisMaterial->getAuthor() << endl << thisMaterial->getPrice() << endl << thisMaterial->getISBN() << endl << thisMaterial->getNumPages() << endl
				<< EBook::FormatMap.at(thisMaterial->getFormat()) << endl;
		}

		else if (classType == "class Book") {
			Book* thisMaterial = dynamic_cast<Book*>(library[i]);
			outputStream << thisMaterial->getTitle() << endl << thisMaterial->getLocation() << endl
				<< thisMaterial->getAuthor() << endl << thisMaterial->getPrice() << endl << thisMaterial->getNumPages() << endl << thisMaterial->getISBN() << endl
				<< thisMaterial->isAvailabe() << endl;

			if (!thisMaterial->isAvailabe()) {
				Date* dueDate = thisMaterial->getDueDate();
				outputStream << dueDate->toString() << endl;
				outputStream << thisMaterial->getBorrowedBy()->getUsername() << endl;
			}
		}

		else if (classType == "class Magazine") {
			Magazine* thisMaterial = dynamic_cast<Magazine*>(library[i]);
			outputStream << thisMaterial->getTitle() << endl << thisMaterial->getLocation() << endl
				<< thisMaterial->getAuthor() << endl << thisMaterial->getPrice() << endl << thisMaterial->getNumPages() << endl << thisMaterial->getIssue() << endl
				<< thisMaterial->isAvailabe() << endl;

			if (!thisMaterial->isAvailabe()) {
				Date* dueDate = thisMaterial->getDueDate();
				outputStream << dueDate->toString() << endl;
				outputStream << thisMaterial->getBorrowedBy()->getUsername() << endl;
			}
		}

		else if (classType == "class Movie") {
			Movie* thisMaterial = dynamic_cast<Movie*>(library[i]);
			outputStream << thisMaterial->getTitle() << endl << thisMaterial->getLocation() << endl
				<< thisMaterial->getAuthor() << endl << thisMaterial->getPrice() << endl << thisMaterial->getRunTime() << endl << Movie::FormatMap.at(thisMaterial->getFormat()) << endl
				<< thisMaterial->isAvailabe() << endl;

			if (!thisMaterial->isAvailabe()) {
				Date* dueDate = thisMaterial->getDueDate();
				outputStream << dueDate->toString() << endl;
				outputStream << thisMaterial->getBorrowedBy()->getUsername() << endl;
			}
		}

		else if (classType == "class CD") {
			CD* thisMaterial = dynamic_cast<CD*>(library[i]);
			outputStream << thisMaterial->getTitle() << endl << thisMaterial->getLocation() << endl
				<< thisMaterial->getAuthor() << endl << thisMaterial->getPrice() << endl << thisMaterial->getRunTime() << endl << thisMaterial->getNumTracks() << endl;

			for (int i = 0; i < thisMaterial->getNumTracks(); i++) {
				outputStream << thisMaterial->getTrackName(i) << endl;
			}

			outputStream << thisMaterial->isAvailabe() << endl;

			if (!thisMaterial->isAvailabe()) {
				Date* dueDate = thisMaterial->getDueDate();
				outputStream << dueDate->toString() << endl;
				outputStream << thisMaterial->getBorrowedBy()->getUsername() << endl;
			}
		}

		else {
			Game* thisMaterial = dynamic_cast<Game*>(library[i]);
			outputStream << thisMaterial->getTitle() << endl << thisMaterial->getLocation() << endl
				<< thisMaterial->getAuthor() << endl << thisMaterial->getPrice() << endl << thisMaterial->getRunTime() << endl << Game::FormatMap.at(thisMaterial->getFormat()) << endl
				<< thisMaterial->isAvailabe() << endl;

			if (!thisMaterial->isAvailabe()) {
				Date* dueDate = thisMaterial->getDueDate();
				outputStream << dueDate->toString() << endl;
				outputStream << thisMaterial->getBorrowedBy()->getUsername() << endl;
			}
		}
	}

	outputStream.close();
}

vector<LibraryMaterial*> readLibraryFromFile(vector<Account*>& accounts) {
	vector<LibraryMaterial*> library;

	ifstream inputFile;

	inputFile.open("library.txt");

	while (!(inputFile.eof())) {
		string className;
		string name;
		string location;
		string author;
		double price;

		getline(inputFile, className);
		if (className == "") break;

		getline(inputFile, name);
		getline(inputFile, location);
		getline(inputFile, author);
		inputFile >> price;
		inputFile.ignore();

		if (className == "class ReferenceMaterial") {
			string isbn;
			int numPages;

			getline(inputFile, isbn);
			inputFile >> numPages;
			inputFile.ignore();

			library.push_back(new ReferenceMaterial(name, location, author, price, isbn, numPages));
		}

		else if (className == "class EBook") {

			int numPages;
			string isbn;
			string stringFormat;
			EBook::Format format;

			getline(inputFile, isbn);
			inputFile >> numPages;
			inputFile.ignore();
			getline(inputFile, stringFormat);

			format = EBook::StringMap.at(stringFormat);

			library.push_back(new EBook(name, location, author, price, numPages, isbn, format));
		}

		else if (className == "class Book") {

			int numPages;
			string isbn;

			inputFile >> numPages;
			inputFile.ignore();
			getline(inputFile, isbn);

			//we need to check to see if this book "is available" if not, we have to store account ID and whatnot.
			//we'll need a vector of accounts first, though.

			bool isAvailable;

			inputFile >> isAvailable;
			inputFile.ignore();

			if (isAvailable) library.push_back(new Book(name, location, author, price, numPages, isbn));
			else {
				//is not available
				string due;
				string username;

				getline(inputFile, due);
				getline(inputFile, username);

				Date* dueDate = new Date(due);
				Account* borrowingAccount = accounts[0];

				for (int i = 0; i < accounts.size(); i++) {
					if (accounts[i]->getUsername() == username) {
						borrowingAccount = accounts[i];
						break;
					}
				}

				BasicAccount* thisAccount = dynamic_cast<BasicAccount*>(borrowingAccount);
				library.push_back(new Book(name, location, author, price, numPages, isbn, dueDate, thisAccount));

				if (!(*dueDate <= DateOperation::TODAY)) {
					thisAccount->fine((0.25 * (DateOperation::TODAY - *dueDate)));
				}
			}
		}

		else if (className == "class Magazine") {

			int numPages;
			int issue;

			inputFile >> numPages;
			inputFile.ignore();
			inputFile >> issue;

			bool isAvailable;

			inputFile >> isAvailable;
			inputFile.ignore();

			if (isAvailable) library.push_back(new Magazine(name, location, author, price, numPages, issue));
			else {
				//is not available
				string due;
				string username;

				getline(inputFile, due);
				getline(inputFile, username);

				Date* dueDate = new Date(due);
				Account* borrowingAccount = accounts[0];

				for (int i = 0; i < accounts.size(); i++) {
					if (accounts[i]->getUsername() == username) {
						borrowingAccount = accounts[i];
						break;
					}
				}

				BasicAccount* thisAccount = dynamic_cast<BasicAccount*>(borrowingAccount);
				library.push_back(new Magazine(name, location, author, price, numPages, issue, dueDate, thisAccount));

				if (!(*dueDate <= DateOperation::TODAY)) {
					thisAccount->fine((0.25 * (DateOperation::TODAY - *dueDate)));
				}
			}
		}

		else if (className == "class Movie") {

			string runTime;
			Movie::Format format;
			string movieFormat;

			getline(inputFile, runTime);
			getline(inputFile, movieFormat);

			int actualRunTime = 0;
			actualRunTime += stoi(runTime.substr(0, 2)) * 3600;
			actualRunTime += stoi(runTime.substr(3, 2)) * 60;
			actualRunTime += stoi(runTime.substr(6, 2));

			format = Movie::StringMap.at(movieFormat);

			bool isAvailable;

			inputFile >> isAvailable;
			inputFile.ignore();

			if (isAvailable) library.push_back(new Movie(name, location, author, price, actualRunTime, format));
			else {
				//is not available
				string due;
				string username;

				getline(inputFile, due);
				getline(inputFile, username);

				Date* dueDate = new Date(due);
				Account* borrowingAccount = accounts[0];

				for (int i = 0; i < accounts.size(); i++) {
					if (accounts[i]->getUsername() == username) {
						borrowingAccount = accounts[i];
						break;
					}
				}

				BasicAccount* thisAccount = dynamic_cast<BasicAccount*>(borrowingAccount);
				library.push_back(new Movie(name, location, author, price, actualRunTime, format, dueDate, thisAccount));

				if (!(*dueDate <= DateOperation::TODAY)) {
					thisAccount->fine((0.25 * (DateOperation::TODAY - *dueDate)));
				}
			}
		}

		else if (className == "class CD") {

			string runTime;
			int numTracks;

			getline(inputFile, runTime);
			inputFile >> numTracks;
			inputFile.ignore();

			int actualRunTime = 0;
			actualRunTime += stoi(runTime.substr(0, 2)) * 3600;
			actualRunTime += stoi(runTime.substr(3, 2)) * 60;
			actualRunTime += stoi(runTime.substr(6, 2));

			vector<string> trackNames(numTracks);

			for (int i = 0; i < numTracks; i++) {
				getline(inputFile, trackNames[i]);
			}

			bool isAvailable;

			inputFile >> isAvailable;
			inputFile.ignore();

			if (isAvailable) library.push_back(new CD(name, location, author, price, actualRunTime, trackNames));
			else {
				//is not available
				string due;
				string username;

				getline(inputFile, due);
				getline(inputFile, username);

				Date* dueDate = new Date(due);
				Account* borrowingAccount = accounts[0];

				for (int i = 0; i < accounts.size(); i++) {
					if (accounts[i]->getUsername() == username) {
						borrowingAccount = accounts[i];
						break;
					}
				}

				BasicAccount* thisAccount = dynamic_cast<BasicAccount*>(borrowingAccount);
				library.push_back(new CD(name, location, author, price, actualRunTime, trackNames, dueDate, thisAccount));

				if (!(*dueDate <= DateOperation::TODAY)) {
					thisAccount->fine((0.25 * (DateOperation::TODAY - *dueDate)));
				}
			}
		}

		else {

			string runTime;
			string stringFormat;
			Game::Format format;

			getline(inputFile, runTime);
			getline(inputFile, stringFormat);
			format = Game::StringMap.at(stringFormat);

			int actualRunTime = 0;
			actualRunTime += stoi(runTime.substr(0, 2)) * 3600;
			actualRunTime += stoi(runTime.substr(3, 2)) * 60;
			actualRunTime += stoi(runTime.substr(6, 2));

			bool isAvailable;

			inputFile >> isAvailable;
			inputFile.ignore();

			if (isAvailable) library.push_back(new Game(name, location, author, price, actualRunTime, format));
			else {
				//is not available
				string due;
				string username;

				getline(inputFile, due);
				getline(inputFile, username);

				Date* dueDate = new Date(due);
				Account* borrowingAccount = accounts[0];

				for (int i = 0; i < accounts.size(); i++) {
					if (accounts[i]->getUsername() == username) {
						borrowingAccount = accounts[i];
						break;
					}
				}

				BasicAccount* thisAccount = dynamic_cast<BasicAccount*>(borrowingAccount);
				library.push_back(new Game(name, location, author, price, actualRunTime, format, dueDate, thisAccount));

				if (!(*dueDate <= DateOperation::TODAY)) {
					thisAccount->fine((0.25 * (DateOperation::TODAY - *dueDate)));
				}
			}
		}
	}

	inputFile.close();
	return library;
}

string clearScreen() {
	return string(100, '\n');
}

int loginScreen(vector<Account*>& accountsVector, vector<LibraryMaterial*>& library) {
	cout << clearScreen() << "Welcome to the library program, please enter a command." << endl << endl
		<< "1) Log-in" << endl
		<< "2) Create Account" << endl
		<< "3) Exit Application" << endl << endl;

	int input = validateInt(1, 3, "Enter an integer to select a command: ");
	if (input == 3) return -1;
	else if (input == 1) {
		int loginCode = loginUser(accountsVector);
		if (loginCode >= 0) {
			string className = typeid(*accountsVector[loginCode]).name();
			if (className == "class BasicAccount") {
				BasicAccount* thisAccount = dynamic_cast<BasicAccount*>(accountsVector[loginCode]);
				cout << clearScreen() << endl;
				while (showBasicMenu(*thisAccount, library));
			}
			else if (className == "class AdminAccount") {
				AdminAccount* thisAccount = dynamic_cast<AdminAccount*>(accountsVector[loginCode]);
				cout << clearScreen() << endl;
				while (showAdminMenu(*thisAccount, library));
			}
		}
		else if (loginCode == -1) cout << "This username was not found" << endl;
	}
	else accountsVector.push_back(createAccount());

	return 0;
}

int loginUser(vector<Account*>& accountsVector) {

	cin.ignore(INT_MAX, '\n');

	cout << clearScreen() << "Enter a username (or enter 0 to exit): ";

	string userName;
	getline(cin, userName);

	if (userName == "0") return -1;

	for (int i = 0; i < accountsVector.size(); i++) {
		if (accountsVector[i]->getUsername() == userName) {

			while (!(attemptLogin(accountsVector[i]))) {
				cout << "Invalid password. Would you like to try again? (y/n): ";

				char getInput;
				cin >> getInput;

				cin.clear();
				cin.ignore(INT_MAX, '\n');

				if (!(getInput == 'Y' || getInput == 'y')) return -1;
			}

			return i;
		}
	}

	return -2;
}

bool attemptLogin(Account* user) {
	cout << "Enter the password for account " << user->getUsername() << ": ";
	string password;

	getline(cin, password);

	return user->login(password);
}

Account* createAccount() {
	cout << "What is the new user's (unique, no-spaces) username?: ";
	string username;

	cin >> username;
	cin.ignore(INT_MAX, '\n');

	cout << "\nEnter a password for account " << username << ": ";
	string password;
	getline(cin, password);

	cout << "\nWhat is this user's full name?: ";
	string name;
	getline(cin, name);

	cout << "\nAccount Type" << endl
		<< "1) Basic Account" << endl
		<< "2) Admin Account" << endl << endl;

	int input = validateInt(1, 2, "Enter an integer that corresponds to the account type you'd like to create: ");

	if (input == 1) {
		return new BasicAccount(name, password, username);
	}
	else return new AdminAccount(name, password, username);
}

void writeAccountsToFile(vector<Account*>& accountsVector) {
	ofstream outputFile;

	outputFile.open("accounts.txt", ifstream::in | ifstream::trunc);
	for (int i = 0; i < accountsVector.size(); i++) {
		outputFile << typeid(*accountsVector[i]).name() << endl << accountsVector[i]->getUsername() << endl << accountsVector[i]->getName() << endl << accountsVector[i]->getPasswordHash() << endl;
	}

	outputFile.close();
}

vector<Account*> readAccountsFromFile() {
	vector<Account*> accounts = vector<Account*>();

	ifstream inputFile;

	inputFile.open("accounts.txt");
	if (inputFile.is_open()) {
		while (!(inputFile.eof())) {
			string className;
			string userName;
			string name;
			long passwordHash;

			getline(inputFile, className);
			if (className == "") break;
			getline(inputFile, userName);
			getline(inputFile, name);
			inputFile >> passwordHash;
			inputFile.ignore();

			if (className == "class AdminAccount") accounts.push_back(new AdminAccount(name, userName, passwordHash));
			else accounts.push_back(new BasicAccount(name, userName, passwordHash));
		}
	}

	return accounts;
}

bool showAdminMenu(AdminAccount& user, vector<LibraryMaterial*>& library) {
	cout << clearScreen() << "1) Add new material to library" << endl
		<< "2) Log-out" << endl << endl;

	int input = validateInt(1, 2, "Enter an integer to select an option: ");

	if (input == 2) return false;

	else {
		LibraryMaterial* newMaterial = createNewMaterial();
		if (newMaterial != nullptr) library.push_back(newMaterial);
		return true;
	}
}

LibraryMaterial* createNewMaterial() {
	cin.ignore(INT_MAX, '\n');

	cout << clearScreen() << "1) Create Reference Material" << endl
		<< "2) Create eBook" << endl
		<< "3) Create Book" << endl
		<< "4) Create Magazine" << endl
		<< "5) Create Movie" << endl
		<< "6) Create CD" << endl
		<< "7) Create Game" << endl
		<< "8) Exit to menu" << endl << endl;

	int input = validateInt(1, 8, "Enter an integer to create that type of object: ");
	if (input == 8) return nullptr;

	cin.ignore(INT_MAX, '\n');

	string title;
	string author;
	string location = "Library";
	double price = 10.00;

	if (input == 1) {
		int numPages;
		string isbn;

		cout << "Enter title for reference material: ";
		getline(cin, title);

		cout << "Enter author for reference material: ";
		getline(cin, author);

		cout << "Enter the number of pages the reference material has: ";
		cin >> numPages;
		cin.ignore();

		cout << "Enter the ISBN for the reference material: ";
		getline(cin, isbn);

		return new ReferenceMaterial(title, author, location, price, isbn, numPages);
	}

	if (input == 2) {
		int numPages;
		string isbn;
		EBook::Format format;

		cout << "Enter title for eBook: ";
		getline(cin, title);

		cout << "Enter author for eBook: ";
		getline(cin, author);

		cout << "Enter the number of pages the eBook has: ";
		cin >> numPages;
		cin.ignore();

		cout << "Enter the ISBN for the eBook: ";
		getline(cin, isbn);

		for (unsigned int i = 0; i <= EBook::pdf; i++) {
			cout << i + 1 << ") " << EBook::FormatMap.at(static_cast<EBook::Format>(i)) << endl;
		}

		cout << endl;
		input = validateInt(1, static_cast<int>(EBook::pdf) + 1, "Enter an integer corresponding to the format of the eBook: ");
		format = static_cast<EBook::Format>(input - 1);

		return new EBook(title, author, location, price, numPages, isbn, format);
	}

	if (input == 3) {
		int numPages;
		string isbn;

		cout << "Enter title for book: ";
		getline(cin, title);

		cout << "Enter author for book: ";
		getline(cin, author);

		cout << "Enter the number of pages the book has: ";
		cin >> numPages;
		cin.ignore();

		cout << "Enter the ISBN for the book: ";
		getline(cin, isbn);

		return new Book(title, author, location, price, numPages, isbn);
	}

	if (input == 4) {
		int numPages;
		int issue;

		cout << "Enter title for Magazine: ";
		getline(cin, title);

		cout << "Enter author for Magazine: ";
		getline(cin, author);

		cout << "Enter the number of pages the Magazine has: ";
		cin >> numPages;
		cin.ignore();

		cout << "Enter the issue number for the Magazine: ";
		cin >> issue;

		return new Magazine(title, author, location, price, numPages, issue);
	}

	if (input == 5) {
		int runTime, hours, mins, seconds;
		Movie::Format format;

		cout << "Enter title for movie: ";
		getline(cin, title);

		cout << "Enter publisher for movie: ";
		getline(cin, author);

		cout << "Enter the amount of hours the movie runs: ";
		cin >> hours;

		cout << "Enter the amount of minutes the movie runs: ";
		cin >> mins;

		cout << "Enter the amount of seconds the movie runs: ";
		cin >> seconds;

		runTime = (hours * 3600) + (mins * 60) + seconds;

		for (unsigned int i = 0; i <= Movie::bluray; i++) {
			cout << i + 1 << ") " << Movie::FormatMap.at(static_cast<Movie::Format>(i)) << endl;
		}

		cout << endl;
		input = validateInt(1, static_cast<int>(Movie::bluray) + 1, "Enter an integer corresponding to the format of the Movie: ");
		format = static_cast<Movie::Format>(input - 1);

		return new Movie(title, location, author, price, runTime, format);
	}

	if (input == 6) {
		int runTime, hours, mins, seconds;

		cout << "Enter title for CD: ";
		getline(cin, title);

		cout << "Enter artist for CD: ";
		getline(cin, author);

		cout << "Enter the amount of hours the CD runs: ";
		cin >> hours;

		cout << "Enter the amount of minutes the CD runs: ";
		cin >> mins;

		cout << "Enter the amount of seconds the CD runs: ";
		cin >> seconds;

		runTime = (hours * 3600) + (mins * 60) + seconds;

		int numTracks;
		cout << "Enter the number of tracks on the CD: ";
		cin >> numTracks;
		cin.ignore(INT_MAX, '\n');

		vector<string> trackNames = vector<string>(numTracks);

		for (int i = 0; i < numTracks; i++) {
			cout << "Enter the title of track " << i + 1 << ": ";
			getline(cin, trackNames[i]);
		}

		return new CD(title, location, author, price, runTime, trackNames);
	}

	if (input == 7) {
		int runTime, hours, mins, seconds;
		Game::Format format;

		cout << "Enter title for Game: ";
		getline(cin, title);

		cout << "Enter publisher for Game: ";
		getline(cin, author);

		cout << "Enter the amount of hours the Game runs: ";
		cin >> hours;

		cout << "Enter the amount of minutes the Game runs: ";
		cin >> mins;

		cout << "Enter the amount of seconds the Game runs: ";
		cin >> seconds;

		runTime = (hours * 3600) + (mins * 60) + seconds;

		for (unsigned int i = 0; i <= Game::ninswitch; i++) {
			cout << i + 1 << ") " << Game::FormatMap.at(static_cast<Game::Format>(i)) << endl;
		}

		cout << endl;
		input = validateInt(1, static_cast<int>(Game::ninswitch) + 1, "Enter an integer corresponding to the format of the Game: ");
		format = static_cast<Game::Format>(input - 1);

		return new Game(title, location, author, price, runTime, format);
	}
}