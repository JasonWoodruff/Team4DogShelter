/*
CIS22C Team 4 Project:	Dog Shelter Management Program

Purpose:	Create an easily accessible and editable catalog of dogs in a shelter
			and allow the user to search through it based on dog attributes

@author		
Jason Woodruff	-	Project Coordinator
Stacy Wheeler	-	Data Record
Bryson Li		-	I/O
James Juchau	-	Hashed Table
Kevin Chen		-	Binary Search Tree

@version	1.5
@since		11/25/2014
*/

#include <iostream>
#include <Windows.h> //included by James for 'system("CLS")', a clear screen command for aesthetic purposes
#include <string>
#include <cctype>
#include <fstream>
#include "Dog.h"
#include "HashMap.h"
#include "DogTree.h"
using namespace std;

//File Operations
bool readDogsFromFile(HashMap* dogHash, avlTree* dogTree);
bool updateDogFile(HashMap* dogHash);							
char yesNoInput(string prompt);

//Main Menu and Options
bool mainMenu();														
int getMainMenuChoice();
bool processMainMenuChoice(int choice, HashMap* dogHash, avlTree* dogTree);

//Dog Options (Add/Remove/Display)
bool addDog(HashMap* dogHash, avlTree* dogTree);						
bool removeDog(HashMap* dogHash, avlTree* dogTree);					
bool displayDogInfoByIdSearch(HashMap* dogHash);						

//Update Dog (Team Choice 1)
bool updateDog(HashMap* dogHash, avlTree* dogTree);
bool updateDogMenu();
int getUpdateDogMenuChoice();
bool processUpdateDogMenuChoice(int choice, string& id, string& name, string& age, string& gender, string& breed, string& desc);
bool updateName(string& name);
bool updateAge(string& age);
bool updateDesc(string& desc);

//Efficiency Report
bool displayEfficiencyReport();										

//Dog Id Validation
bool isValidDogId(string dogId);

//Filename
const string FILENAME = "dog.txt";

int main()
{
	int choice = 0;
	
	avlTree* dogTree = new avlTree();
	HashMap* dogHash = new HashMap();
	readDogsFromFile(dogHash, dogTree);

	while (choice != 10)
	{
		//Database was cleared, must recreate the data structures
		if (choice == 9)
		{
			dogTree = new avlTree();
			dogHash = new HashMap();
		}

		mainMenu();
		choice = getMainMenuChoice();
		processMainMenuChoice(choice, dogHash, dogTree);
	}

	delete dogTree;
	delete dogHash;
	
	system("pause");
	return 0;
}

bool mainMenu()
{
	cout << "-----------------Main Menu-----------------" << endl << endl;
	cout << "1  - Add a dog" << endl;
	cout << "2  - Remove a dog" << endl;
	cout << "3  - Display dog attributes" << endl;
	cout << "4  - Update dog attributes" << endl;
	cout << "5  - Display dogs in hash sequence" << endl;
	cout << "6  - Display dogs in key sequence" << endl;
	cout << "7  - Display indented search tree" << endl;
	cout << "8  - Display efficiency report" << endl;
	cout << "9  - Clear the Database" << endl;
	cout << "10 - Update the file / Quit" << endl << endl;
	cout << "Enter your choice here: ";

	return true;
}

int getMainMenuChoice()
{
	string input;
	int choice;

	cin >> input;
	choice = atoi(input.c_str());

	while (choice < 1 || choice > 10)
	{
		cout << "Invalid menu choice, try again: ";
		cin >> input;
		choice = atoi(input.c_str());
	}
	cout << endl;
	return choice;
}

bool processMainMenuChoice(int choice, HashMap* dogHash, avlTree* dogTree)
{
	switch (choice)
	{
		case 1:
		{
			addDog(dogHash, dogTree);
			break;
		}
		case 2:
		{
			removeDog(dogHash, dogTree);
			break;
		}
		case 3:
		{
			displayDogInfoByIdSearch(dogHash);
			break;
		}
		case 4:
		{
			updateDog(dogHash, dogTree);
			break;
		}
		case 5:
		{
			dogHash->display();
			cout << "Scroll up to view all entries." << endl;
			system("pause");
			break;
		}
		case 6:
		{
			dogTree->inorder();
			cout << "Scroll up to view all entries." << endl;
			system("pause");
			break;
		}
		case 7:
		{
			dogTree->display(1);
			cout << endl << endl;
			break;
		}
		case 8:
		{
			displayEfficiencyReport();
			break;
		}
		case 9:
		{
			delete dogHash;
			delete dogTree;
			break;
		}
		case 10:
		{
			updateDogFile(dogHash);
			cout << "Thank you, now exiting..." << endl << endl;
			break;
		}
	}
	return true;
}

bool readDogsFromFile(HashMap* dogHash, avlTree* dogTree)
{
	fstream dogFile;
	string line;

	dogFile.open(FILENAME, fstream::in);
	while (!dogFile.eof())
	{
		string tempId = "";
		string tempName = "";
		string tempAge = "";
		string tempGender = "";
		string tempBreed = "";
		string tempDesc = "";

		getline(dogFile, tempId, ',');
		getline(dogFile, tempName, ',');
		getline(dogFile, tempAge, ',');
		getline(dogFile, tempGender, ',');
		getline(dogFile, tempBreed, ',');
		getline(dogFile, tempDesc, '\n');

		//JASON I added this check so the program won't recognize an empty line in the file
		if (tempId != "")
		{
			Dog* dog = new Dog(tempId, tempName, tempAge, tempGender, tempBreed, tempDesc);
		
			//Populate the Hash Table
			dogHash->put(dog);

			//Populate the AVL Tree
			dogTree->insert(dog);
		}
	}
	dogFile.close();
	return true;
}

bool updateDogFile(HashMap* dogHash)
{
	ofstream out;
	char choice;

	// will update dogs here
	choice = yesNoInput("Would you like to save changes to the file? (Y/N) -> ");

	if (choice == 'Y')
	{
		out.open(FILENAME.c_str());
		
		dogHash->writeToFile(out);
		
		if (out.good())
			cout << "File succesfully saved" << endl;
		out.close();
	}
	else
		cout << "File not changed." << endl;

	return true;
}

char yesNoInput(string prompt)
{
	char choice;
	cout << prompt;
	do
	{
		cin >> choice;
		choice = toupper(choice);
		if (choice != 'Y' && choice != 'N')
		{
			cout << "Only 'Y' and 'N' are acceptable.  Please enter again-> ";
		}
	} while (choice != 'Y' && choice != 'N');
	return choice;
}

bool addDog(HashMap* dogHash, avlTree* dogTree)
{	
	Dog* newDog = new Dog;

	string newName, newGender, newAge, newBreed, newDescription;
	char yn='x'; //for finalizing 
	system("CLS");
	cout << "Please enter the dog's name." << endl;
	cin.ignore();
	getline(cin, newName); newName[0] = toupper(newName[0]);          //capitalizes first letter to homogenize data
	
	while (newGender != "Male" && newGender != "Female")
	{
		cout << "\nEnter the dog's gender ('Male' or 'Female')." << endl;
		cin >> newGender; newGender[0] = toupper(newGender[0]);        //capitalizes the first character to simplify while loop
		if (newGender != "Male" && newGender != "Female"){ cout << "Only the inputs 'Male' and 'Female' are acceptable." << endl; system("pause"); }
	}

	while (newAge != "Young" && newAge != "Puppy" && newAge != "Adult" && newAge != "Older" && newAge != "Senior")
	{
		cout << "\nEnter a qualitative estimate of the dog's age." << endl;
		cout << "The only acceptable inputs are 'Puppy', 'Young', 'Adult', 'Older' or 'Senior'." << endl;
		cin >> newAge; newAge[0] = toupper(newAge[0]);
	}


	cout << "\nEnter the dog's breed (ie 'Beagle' or 'Scottish Terrier')." << endl;
	cin.ignore();
	getline(cin, newBreed);	newBreed[0] = toupper(newBreed[0]);


	cout << "\nEnter a brief description of this dog's personality." << endl;
	getline(cin, newDescription); newDescription[0] = toupper(newDescription[0]);

	system("CLS"); //clears screen for aesthetics

	cout << "Name: " << newName << endl;
	cout << "Age: " << newAge << endl;
	cout << "Gender: " << newGender << endl;
	cout << "Breed: " << newBreed << endl;
	cout << "Description: " << newDescription << endl;
	
	while (yn != 'Y' && yn != 'N')
	{
		cout << "Finalize this new dog? (Y/N)" << endl;
		cin >> yn;
		yn = toupper(yn);
		if (yn != 'Y' && yn != 'N'){ cout << "Only the characters 'Y' and 'N' are acceptable." << endl; system("pause");}
	}

	if (yn == 'N'){ cout << "Dog discarded. Returning to main menu..." << endl; system("pause"); system("CLS"); }
	if (yn == 'Y')
	{ 
		newDog->setName(newName); 
		newDog->setBreed(newBreed);
		newDog->setGender(newGender);
		newDog->setAge(newAge);
		newDog->setDescription(newDescription);
		/*Obviously this ultimately does nothing. But we can assign this dog pointer to anything once I know where addDog()
		actually assigns a dog to*/
		dogHash->put(newDog);
		dogTree->insert(newDog);
		cout << "Dog finalized. Returning to main menu..." << endl;
		system("pause"); system("CLS");
	}
	

	return true;
}
 //Be sure to enqueue the deleted Dog's ID into Dog::nextAvailable 
bool removeDog(HashMap* dogHash, avlTree* dogTree)
{
	string dogId;
	cout << "Enter the ID of the dog to remove.  Use the format \"DOG###\"\n";
	cout << "Enter the ID here: ";
	cin >> dogId;

	while (!isValidDogId(dogId))
	{
		cout << "Invalid input.  Use the format \"DOG###\"\n";
		cout << "Enter the ID here: ";
		cin >> dogId;
	}

	cout << endl;

	dogHash->remove(dogId);
	dogTree->deleteNode(dogId);

	return true;
}

bool displayDogInfoByIdSearch(HashMap* dogHash)
{
	string dogId;
	cout << "Enter the ID of the Dog to search for.  Use the format \"DOG###\"\n";
	cout << "Enter the ID here: ";
	cin >> dogId;
	cout << endl;

	//JASON - Get the correct syntax, we do not need a validation function
	while (!isValidDogId(dogId))
	{
		cout << "Invalid input.  Use the format \"DOG###\"\n";
		cout << "Enter the ID here: ";
		cin >> dogId;
		cout << endl;
	}
	
	Dog *dog = dogHash->get(dogId);

	if (dog == nullptr)
		cout << "Dog not found" << endl << endl;
	else
		cout << dogHash->get(dogId)->toString() << endl;

	return true;
}

/*
Load Factor
Longest Linked List
Average number of nodes in linked lists 8
*/
bool displayEfficiencyReport()
{
	return true;
}

//Team Choice 1
bool updateDog(HashMap *dogHash, avlTree *dogTree)
{
	string dogId;
	cout << "Enter the ID of the Dog to update.  Use the format \"DOG###\"\n";
	cout << "Enter the ID here: ";
	cin >> dogId;
	cout << endl;

	while (!isValidDogId(dogId))
	{
		cout << "Invalid input.  Use the format \"DOG###\"\n";
		cout << "Enter the ID here: ";
		cin >> dogId;
		cout << endl;
	}

	Dog *dog = dogHash->get(dogId);

	if (dog == nullptr)
		cout << "Dog not found" << endl << endl;
	else
	{
		cout << dog->toString() << endl;
		string id = dog->getID();
		string name = dog->getName();
		string age = dog->getAge();
		string gender = dog->getGender();
		string breed = dog->getBreed();
		string desc = dog->getDescription();

		dogHash->remove(dogId);
		dogTree->deleteNode(dogId);
		
		int choice = 0;
		while (choice != 4)
		{
			updateDogMenu();
			choice = getUpdateDogMenuChoice();
			processUpdateDogMenuChoice(choice, id, name, age, gender, breed, desc);
		}

		Dog* updatedDog = new Dog(id, name, age, gender, breed, desc);
		Dog::setKeyNumGenerator(Dog::getKeyNumGenerator() - 1);

		dogHash->put(updatedDog);
		dogTree->insert(updatedDog);
	}

	return true;
}

bool updateDogMenu()
{
	cout << "-----------------Update Options-----------------" << endl << endl;
	cout << "1  - Update Name" << endl;
	cout << "2  - Update Age" << endl;
	cout << "3  - Update Description" << endl;
	cout << "4  - Return to Main Menu" << endl << endl;
	cout << "Enter your choice here: ";

	return true;
}

int getUpdateDogMenuChoice()
{
	string input;
	int choice;

	cin >> input;
	choice = atoi(input.c_str());

	while (choice < 1 || choice > 4)
	{
		cout << "Invalid menu choice, try again: ";
		cin >> input;
		choice = atoi(input.c_str());
	}
	cout << endl;
	return choice;
}

bool processUpdateDogMenuChoice(int choice, string& id, string& name, string& age, string& gender, string& breed, string& desc)
{
	switch (choice)
	{
		case 1:
		{
			updateName(name);
			break;
		}
		case 2:
		{
			updateAge(age);
			break;
		}
		case 3:
		{
			updateDesc(desc);
			break;
		}
		case 4:
{
			cout << "Returning to Main Menu..." << endl << endl;
			break;
		}
	}
	return true;
}

bool updateName(string& name)
{
	string dogName;
	cout << "Enter a new name: ";
	cin >> dogName;
	cout << endl;
	name = dogName;
	return true;
}

bool updateAge(string& age)
{
	string dogAge;
	cout << "Enter a new age (must be \"Puppy\", \"Young\", \"Adult\", or \"Senior\": ";
	cin >> dogAge;

	while (dogAge != "Puppy" && dogAge != "Young" && dogAge != "Adult" && dogAge != "Senior")
{
		cout << "Invalid input.  Age must be \"Puppy\", \"Young\", \"Adult\", or \"Senior\"" << endl;
		cout << "Enter a new age: ";
		cin >> dogAge;
	}
	cout << endl;
	age = dogAge;
	return true;
}

bool updateDesc(string& desc)
{
	string dogDesc;
	cout << "Enter a new description: ";
	cin.ignore();
	getline(cin, dogDesc);
	cout << endl;
	desc = dogDesc;
	return true;
}

bool isValidDogId(string dogId)
{
	bool isValid = false;

	if (dogId.length() == 6)
	{
		if (dogId[0] == 'D' && dogId[1] == 'O' && dogId[2] == 'G' && isdigit(dogId[3]) && isdigit(dogId[4]) && isdigit(dogId[5]))
			isValid = true;
	}

	return isValid;
}

/*Team Choice 2	-	Note:	This section may be tricky because we'll have to create many different trees.
I suggest we start with Team Choice 1 and do this if we have time.*/
bool SearchDogMenu()
{
	return true;
}

bool searchDogByName()
{
	return true;
}

bool searchDogByGender()
{
	return true;
}

bool searchDogByAge()
{
	return true;
}

bool searchDogByBreed()
{
	return true;
}


