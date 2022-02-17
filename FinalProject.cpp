/*
Progammer: Allister Aniceto
Purpose: Final Project
Date Modified: 11/27/21
Compiler: MS VC++ 2019
*/

//use program 13-21 as an example for serialization/deserialization

#include <iostream>
#include <iomanip>
#include <fstream>
#include <cmath>
using namespace std;


//bank account 
class CheckingAccountCls {
private:
	double accountBal;
	double interestRate;
	double annualInterestEarned;
public:
	CheckingAccountCls() {
		accountBal = 0;
		interestRate = 0;
		annualInterestEarned = 0;
	}
	CheckingAccountCls(double aB, int iR, double aIE) :
		annualInterestEarned(aIE), interestRate(iR) {
		this->accountBal = aB; 
	}
	//set member functions
	void setBalance(double aB) { this->accountBal = aB; }
	void setRate(double iR) { this->interestRate = iR; }
	void setInterestEarned(double iE) { this->annualInterestEarned = iE; }

	//get member functions
	double getBalance() { return accountBal; }
	double getRate() { return interestRate; }
	double getInterestEarned() { return annualInterestEarned; }

	double calcInterestEarned(int years) {
		return (accountBal*pow(1+interestRate, years))-accountBal;
	}
};

//customer
class CustomerCls {
private:
	string fname;
	string lname;
	int uniqueID;
	CheckingAccountCls account;
public:
	CustomerCls() {
		fname = "first name";
		lname = "last name";
		uniqueID = 0000000000000;
		account.setBalance(0);
		account.setRate(0);
	}

	CustomerCls(string fname,string lname, int uniqueID, double accountBal, double rate){
		this->fname = fname;
		this->lname = lname;
		this->uniqueID = uniqueID;
		this->account.setBalance(accountBal);
		this->account.setRate(rate);
	}

	//set member functions
	void setFirstName(string fname) { this->fname = fname; }
	void setLastName(string lname) { this->lname = lname; }
	void setUniqueId(int uID) { this->uniqueID = uID; }
	void setRate(double r) { this->account.setRate(r); }
	void setAccountBal(double aB) {this->account.setBalance(aB); }
	void setInterestEarned(double iE) { this->account.setInterestEarned(iE); }

	//get member functions
	string getFName() {return fname;}
 	string getLName() {return lname;}
	int getID() {return uniqueID;}
	double getAccBalance() { return this->account.getBalance();}
	double getInterestRate() {return account.getRate();}
	double getInterestEarned() {return account.getInterestEarned();}

	//calculate interest
	double calcInterest(int years) {return this->account.calcInterestEarned(years);}

	//serialization function
	void store(ofstream& outFile) {
		outFile.open("bankaccount.dat", ios::binary);

		if (!outFile) {
			cout << "ERROR OPENING A FILE!";
			cin.ignore();
			cin.get();
			exit(1);
		}
		else {
			int iD = uniqueID;
			outFile.write(reinterpret_cast<char*>(&uniqueID), sizeof(uniqueID));

			double balance = account.getBalance();
			outFile.write(reinterpret_cast<char*>(&balance), sizeof(balance));

			double interestRate = account.getRate();
			outFile.write(reinterpret_cast<char*>(&interestRate), sizeof(interestRate));

			double interestEarned = account.getInterestEarned();
			outFile.write(reinterpret_cast<char*>(&interestEarned), sizeof(interestEarned));

			//writes first name and last name (gets exact size of fname and lname and stores onto disk)
			int fnameLength = fname.length();
			outFile.write(reinterpret_cast<char*>(&fnameLength), sizeof(int));
			outFile.write(fname.data(), fnameLength);

			int lnameLength = lname.length();
			outFile.write(reinterpret_cast<char*>(&lnameLength), sizeof(int));
			outFile.write(lname.data(), lnameLength);
		}
		outFile.close();
	}

	//deserialization function
	void load(ifstream& inFile) {
		inFile.open("bankaccount.dat", ios::binary);

		if(!inFile){
			cout << "ERROR OPENING A FILE!";
			cin.ignore();
			cin.get();
			exit(1);
		}
		else {
			const int BUFFER_SIZE = 256;
			static char buffer[BUFFER_SIZE]; //used to read names

			inFile.read(reinterpret_cast<char*>(&uniqueID), sizeof(uniqueID)); //unique id

			double bankBalance = account.getBalance();
			inFile.read(reinterpret_cast<char*>(&bankBalance), sizeof(bankBalance)); //balance

			double interestRate = account.getRate();
			inFile.read(reinterpret_cast<char*>(&interestRate), sizeof(interestRate)); //interest rate

			double interestEarned = account.getInterestEarned();
			inFile.read(reinterpret_cast<char*>(&interestEarned), sizeof(interestEarned)); //interestEarned

			////get length and data for fname
			int fnameLength;
			fnameLength = fname.length();
			inFile.read(reinterpret_cast<char*>(&fnameLength), sizeof(int));
			//read the data for fname into a local buffer
			inFile.read(buffer, fnameLength);
			//null terminate the buffer
			buffer[fnameLength] = '\0';
			fname = buffer;

			////same thing for lname
			int lnameLength;
			lnameLength = lname.length();
			inFile.read(reinterpret_cast<char*>(&lnameLength), sizeof(int));
			inFile.read(buffer, lnameLength);
			buffer[lnameLength] = '\0';
			lname = buffer;
		}
		inFile.close();
	}
	
	//display functions
	void display() {
		cout << "\nUnique ID: " << uniqueID << endl;
		cout << "Name: " << fname << " " << lname << endl;
		cout << "Total Balance: $" << fixed << setprecision(2) << account.getBalance() << endl;
		cout << "Interest Rate: " << fixed << setprecision(2) << account.getRate() * 100 << "%" << endl;
		cout << "Interest Earned: $" << fixed << setprecision(2) << account.getInterestEarned() << endl;
	}
	void displayCustomer() {
		cout << "Customer: " << fname << " " << lname << endl;
	}
	void displayBalance() {
		cout << "Total Balance: $" << fixed << setprecision(2) << account.getBalance() << endl;
	} 
};

//menu for the banker
void menu() {
	cout << endl;
	cout << "*--------------------------------------------------------*\n";
	cout << "|0. Exit                                                 |\n";
	cout << "|1. Enter ALL Customer Information                       |\n";
	cout << "|2. Read ALL Customer Information                        |\n";
	cout << "|3. Add Aditional Customer Information                   |\n";
	cout << "|4. Search For a Particular Customer by a Unique ID      |\n";
	cout << "|5. List Total Balance of a Particular Account           |\n";
	cout << "|6. Calculate Total Balance of All Customers             |\n";
	cout << "*--------------------------------------------------------*"<<endl;
}


string getFirstName() {
	string userFirstName;
	cout << "Enter your first name: ";
	cin >> userFirstName;
	return userFirstName;
}
string getLastName() {
	string userLastName;
	cout << "Enter your last name: ";
	cin >> userLastName;
	return userLastName;
}
int getID() {
	int ID;
	cout << "Enter ID: ";
	cin >> ID;
	while (ID < 0) {
		cout << "Please enter a valid ID: ";
		cin >> ID;
	}
	return ID;
}
double getAccountBal() {
	double balance;
	cout << "Enter Account Balance: ";
	cin >> balance;
	while (balance < 0) {
		cout << "Please enter a postive balance: ";
		cin >> balance;
	}
	return balance;
}
double getRate() {
	double rate;
	cout << "Enter Interest Rate: ";
	cin >> rate;
	while (rate < 0) {
		cout << "Please enter a positive rate: ";
		cin >> rate;
	}
	return rate;
}
int getYearSaved() {
	int year;
	cout << "Years Saved: ";
	cin >> year;
	while (year < 0) {
		cout << "Please enter a postive year: ";
		cin >> year;
	}
	return year;
}

//if banker wants to add more customers, this funciton will "increase" the size of dynamic array
void expand(CustomerCls*& arr, int& start, int cap) {
	CustomerCls* tempArr = nullptr;
	tempArr = new CustomerCls[cap];

	
	string fn;
	string ln;
	for (int i = 0; i < start; i++) {
		tempArr[i].setUniqueId(i);
		tempArr[i].setFirstName(arr[i].getFName());
		tempArr[i].setLastName(arr[i].getLName());
		tempArr[i].setAccountBal(arr[i].getAccBalance());
		tempArr[i].setRate(arr[i].getInterestRate());
		tempArr[i].setInterestEarned(arr[i].getInterestEarned());
	}
	delete[] arr; 
	arr = tempArr;
}


int main() {

	//customers
	int num;
	int add;
	int before;
	cout << "Please Enter # of Customers: ";
	cin >> num;

	CustomerCls* customerList;
	customerList = new CustomerCls[num];

	//creating input and output files
	ofstream outFile;
	ifstream inFile;

	//variables for initializing
	string fname;
	string lname;
	double rate;
	double balance;
	double annualInterestEarned;
	double total;
	int years;
	int choice;
	int ID;


	menu();
	cout << "Enter a choice: ";
	cin >> choice;
	while (choice >= 0 && choice <= 6) {
		switch (choice) {
		case 0:
			return 0;
		case 1: //enter customer information

			for (int h = 0; h < num; h++) {
				cout << endl;
				cout << "*--------------------*" << endl;
				cout << "|CUSTOMER #" << h + 1 << " (ID: " << h << ") |" << endl;
				cout << "*--------------------*" << endl;
				customerList[h].setUniqueId(h); //set uniqueID as the same as the element number
				fname = getFirstName();
				customerList[h].setFirstName(fname);
				lname = getLastName();
				customerList[h].setLastName(lname);
				rate = getRate();
				customerList[h].setRate(rate);
				balance = getAccountBal();
				customerList[h].setAccountBal(balance);
				//annual interest earned
				years = getYearSaved();
				annualInterestEarned = customerList[h].calcInterest(years);
				customerList[h].setInterestEarned(annualInterestEarned);
				customerList[h].store(outFile);
			}
			cout << "Data has been written to \"bankaccount.dat\"..." << endl;
			break;
		case 2: //lists customer info from disk

			cout << "List of all customer's info: " << endl;
			for (int j = 0; j < num; j++) {
				//customerList[j].load(inFile);
				customerList[j].display();
			}
			
			break;
		case 3:
			before = num;
			cout << "Enter how many customer you would like to add: ";
			cin >> add;
			num += add;

			expand(customerList, before, num);

			for (int k = before; k < num; k++) {
				cout << "*--------------------*" << endl;
				cout << "|CUSTOMER #" << k + 1 << " (ID: " << k << ") |" << endl;
				cout << "*--------------------*" << endl;
				customerList[k].setUniqueId(k); //set uniqueID as the same as the element number
				fname = getFirstName();
				customerList[k].setFirstName(fname);
				lname = getLastName();
				customerList[k].setLastName(lname);
				rate = getRate();
				customerList[k].setRate(rate);
				balance = getAccountBal();
				customerList[k].setAccountBal(balance);
				//annual interest earned
				years = getYearSaved();
				annualInterestEarned = customerList[k].calcInterest(years);
				customerList[k].setInterestEarned(annualInterestEarned);
				customerList[k].store(outFile);
			}

			break;
		case 4: //displays customer name by a unique ID 
			cout << "Enter Customer Unique ID: ";
			cin >> ID;

			if (ID == customerList[ID].getID()) {
				customerList[ID].displayCustomer();
			}
			break;
		case 5: //displays balance of a particular customer
			cout << "Enter Customer Unique ID: ";
			cin >> ID;

			if (ID == customerList[ID].getID()) {
				customerList[ID].displayBalance();
			}
			break;
		case 6://calculates total balance of all customers and lists it to screen
			total = 0;
			for (int k = 0; k < num; k++) {
				total += customerList[k].getAccBalance();
			}
			cout << "Total Balance of All Customers: $" << fixed << setprecision(2) << total << endl;
			break;
		default:
			cout << "Goodbye";
		}
		menu();
		cout << "Enter a choice: ";
		cin >> choice;
		cout << endl;
	}
	cin.ignore();
	cin.get();

	delete[] customerList;

	system("pause");

	return 0;
}

/*
OUTPUT:
Please Enter # of Customers: 1

*--------------------------------------------------------*
|0. Exit                                                 |
|1. Enter ALL Customer Information                       |
|2. Read ALL Customer Information                        |
|3. Add Aditional Customer Information                   |
|4. Search For a Particular Customer by a Unique ID      |
|5. List Total Balance of a Particular Account           |
|6. Calculate Total Balance of All Customers             |
*--------------------------------------------------------*
Enter a choice: 1

*--------------------*
|CUSTOMER #1 (ID: 0) |
*--------------------*
Enter your first name: allister
Enter your last name: aniceto
Enter Interest Rate: 0.05
Enter Account Balance: 3000
Years Saved: 5
Data has been written to "bankaccount.dat"...

*--------------------------------------------------------*
|0. Exit                                                 |
|1. Enter ALL Customer Information                       |
|2. Read ALL Customer Information                        |
|3. Add Aditional Customer Information                   |
|4. Search For a Particular Customer by a Unique ID      |
|5. List Total Balance of a Particular Account           |
|6. Calculate Total Balance of All Customers             |
*--------------------------------------------------------*
Enter a choice: 2

List of all customer's info:

Unique ID: 0
Name: allister aniceto
Total Balance: $3000.00
Interest Rate: 5.00%
Interest Earned: $828.84

*--------------------------------------------------------*
|0. Exit                                                 |
|1. Enter ALL Customer Information                       |
|2. Read ALL Customer Information                        |
|3. Add Aditional Customer Information                   |
|4. Search For a Particular Customer by a Unique ID      |
|5. List Total Balance of a Particular Account           |
|6. Calculate Total Balance of All Customers             |
*--------------------------------------------------------*
Enter a choice: 3

Enter how many customer you would like to add: 1
*--------------------*
|CUSTOMER #2 (ID: 1) |
*--------------------*
Enter your first name: lisa
Enter your last name: chung
Enter Interest Rate: 0.05
Enter Account Balance: 5000
Years Saved: 7

*--------------------------------------------------------*
|0. Exit                                                 |
|1. Enter ALL Customer Information                       |
|2. Read ALL Customer Information                        |
|3. Add Aditional Customer Information                   |
|4. Search For a Particular Customer by a Unique ID      |
|5. List Total Balance of a Particular Account           |
|6. Calculate Total Balance of All Customers             |
*--------------------------------------------------------*
Enter a choice: 2

List of all customer's info:

Unique ID: 0
Name: allister aniceto
Total Balance: $3000.00
Interest Rate: 5.00%
Interest Earned: $828.84

Unique ID: 1
Name: lisa chung
Total Balance: $5000.00
Interest Rate: 5.00%
Interest Earned: $2035.50

*--------------------------------------------------------*
|0. Exit                                                 |
|1. Enter ALL Customer Information                       |
|2. Read ALL Customer Information                        |
|3. Add Aditional Customer Information                   |
|4. Search For a Particular Customer by a Unique ID      |
|5. List Total Balance of a Particular Account           |
|6. Calculate Total Balance of All Customers             |
*--------------------------------------------------------*
Enter a choice: 4

Enter Customer Unique ID: 0
Customer: allister aniceto

*--------------------------------------------------------*
|0. Exit                                                 |
|1. Enter ALL Customer Information                       |
|2. Read ALL Customer Information                        |
|3. Add Aditional Customer Information                   |
|4. Search For a Particular Customer by a Unique ID      |
|5. List Total Balance of a Particular Account           |
|6. Calculate Total Balance of All Customers             |
*--------------------------------------------------------*
Enter a choice: 5

Enter Customer Unique ID: 1
Total Balance: $5000.00

*--------------------------------------------------------*
|0. Exit                                                 |
|1. Enter ALL Customer Information                       |
|2. Read ALL Customer Information                        |
|3. Add Aditional Customer Information                   |
|4. Search For a Particular Customer by a Unique ID      |
|5. List Total Balance of a Particular Account           |
|6. Calculate Total Balance of All Customers             |
*--------------------------------------------------------*
Enter a choice: 6

Total Balance of All Customers: $8000.00

*--------------------------------------------------------*
|0. Exit                                                 |
|1. Enter ALL Customer Information                       |
|2. Read ALL Customer Information                        |
|3. Add Aditional Customer Information                   |
|4. Search For a Particular Customer by a Unique ID      |
|5. List Total Balance of a Particular Account           |
|6. Calculate Total Balance of All Customers             |
*--------------------------------------------------------*
Enter a choice: 0


C:\Users\allister18\OneDrive - Columbia Basin College\Documents\CS162\FinalProjectFolder\Final Project\FinalProjectProgram\CS162FinalProjectSol\Debug\CS162FinalProject.exe (process 4940) exited with code 0.
To automatically close the console when debugging stops, enable Tools->Options->Debugging->Automatically close the console when debugging stops.
Press any key to close this window . . .

*/
