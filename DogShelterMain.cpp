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

//Update Dog (Team Choice)
bool updateDog(HashMap* dogHash, avlTree* dogTree);
bool updateDogMenu();
int getUpdateDogMenuChoice();
bool processUpdateDogMenuChoice(int choice, string& id, string& name, string& age, string& gender, string& breed, string& desc);
bool updateName(string& name);
bool updateAge(string& age);
bool updateDesc(string& desc);

//Efficiency Report
bool displayEfficiencyReport(HashMap* dogHash, avlTree* dogTree);

//Dog ID Validation
bool isValidDogId(string dogId);

//Filename
const string FILENAME = "dog.txt";

/*	
	main loads the database into data structures (Hash and Tree) then processes menu choices until the user decides to exit
*/
int main()
{
	int choice = 0;
	
	HashMap* dogHash = new HashMap();
	avlTree* dogTree = new avlTree();
	readDogsFromFile(dogHash, dogTree);

	//display the menu and process choices until the user quits (chooses option 10)
	while (choice != 10)
	{
		//data structures were deleted, must recreate the data structures
		if (choice == 9)
		{
			dogHash = new HashMap();
			dogTree = new avlTree();
		}

		mainMenu();
		choice = getMainMenuChoice();
		processMainMenuChoice(choice, dogHash, dogTree);
	}

	delete dogHash;
	delete dogTree;
	
	system("pause");
	return 0;
}

/*
	mainMenu displays a menu of options
*/
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

/*
	getMainMenuChoice gets a valid menu choice from the user and returns it
	@return	-	choice is a valid menu choice (integer between 1 and 10)
*/
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

/*
	processMainMenuChoice handles each menu option
	@pre	-	choice is an integer between 1 and 10
	@param	-	choice is the menu choice, an integer between 1 and 10
	@param	-	dogHash is a Hash Table of Dog objects
	@param	-	dogTree is an AVL Tree of Dog objects
*/
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
			cout << endl;
			break;
		}
		case 6:
		{
			dogTree->inorder();
			cout << "Scroll up to view all entries." << endl;
			system("pause");
			cout << endl;
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
			displayEfficiencyReport(dogHash, dogTree);
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

/*
	readDogsFromFile reads each line of 'dog.txt', instantiates a Dog object with the line's data, then adds the Dog to the Hash Table and AVL Tree
	@pre	-	FILENAME (global constant string) is a valid file path
	@param	-	dogHash is a Hash Table of Dog objects
	@param	-	dogTree is an AVL Tree of Dog objects
	@post	-	dogHash and dogTree now store the Dogs from 'dog.txt'
*/
bool readDogsFromFile(HashMap* dogHash, avlTree* dogTree)
{
	fstream dogFile;
	string line;

	//open the file
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

		//the function will not create a Dog if it reads an empty line in the file (empty lines usually occur due to a trailing newline)
		if (tempId != "")
		{
			//instantiate a new Dog object
			Dog* dog = new Dog(tempId, tempName, tempAge, tempGender, tempBreed, tempDesc);
		
			//populate the Hash Table
			dogHash->put(dog);

			//populate the AVL Tree
			dogTree->insert(dog);
		}
	}
	
	//close the file
	dogFile.close();
	return true;
}

/*
	updateDogFile updates 'dog.txt' with the contents of the Hash Table
	@pre	-	FILENAME (global constant string) is a valid file path
	@param	-	dogHash is a Hash Table of Dog objects
	@post	-	'dog.txt' is updated with the Dogs in the Hash Table (if yes) or remains the same (if no)
*/
bool updateDogFile(HashMap* dogHash)
{
	ofstream out;
	char choice;

	//user chooses whether to save and exit or exit without saving
	choice = yesNoInput("Would you like to save changes to the file? (Y/N): ");

	if (choice == 'Y')
	{
		//open the file
		out.open(FILENAME.c_str());
		
		//write to the file
		dogHash->writeToFile(out);
		
		if (out.good())
			cout << "File succesfully saved" << endl;
		
		//close the file
		out.close();
	}
	else
		cout << "File not changed." << endl;

	return true;
}

/*
	yesNoInput gets a valid yes/no choice from the user and returns it
	@param	-	prompt is a message prompt
	@return	-	choice is a valid yes/no choice ('Y' or 'N')
*/
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
			cout << "Only 'Y' and 'N' are acceptable.  Please enter again: ";
		}
	} while (choice != 'Y' && choice != 'N');
	return choice;
}

/*
	addDog creates a new Dog and adds it to the Hash Table and AVL Tree or discards it, depending on user input
	@param	-	dogHash is a Hash Table of Dog objects
	@param	-	dogTree is an AVL Tree of Dog objects
	@post	-	the new Dog is discarded or added to the Hash Table and AVL Tree
*/
bool addDog(HashMap* dogHash, avlTree* dogTree)
{	
	Dog* newDog = new Dog;

	string newName, newGender, newAge, newBreed, newDescription;
	char yn='x';	//holds yes/no choice for finalizing
	system("CLS");
	
	//get the name
	cout << "Please enter the dog's name." << endl;
	cin.ignore();
	getline(cin, newName); newName[0] = toupper(newName[0]);          //capitalizes first letter to homogenize data
	
	//get the gender
	while (newGender != "Male" && newGender != "Female")
	{
		cout << "\nEnter the dog's gender ('Male' or 'Female')." << endl;
		cin >> newGender; newGender[0] = toupper(newGender[0]);        //capitalizes the first character to simplify while loop
		if (newGender != "Male" && newGender != "Female"){ cout << "Only the inputs 'Male' or 'Female' are acceptable." << endl; system("pause"); }
	}

	//get the age
	while (newAge != "Puppy" && newAge != "Young" && newAge != "Adult" && newAge != "Senior")
	{
		cout << "\nEnter a qualitative estimate of the dog's age." << endl;
		cout << "The only acceptable inputs are 'Puppy', 'Young', 'Adult', or 'Senior'." << endl;
		cin >> newAge; newAge[0] = toupper(newAge[0]);
	}

	//get the breed
	cout << "\nEnter the dog's breed (ie 'Beagle' or 'Scottish Terrier')." << endl;
	cin.ignore();
	getline(cin, newBreed);	newBreed[0] = toupper(newBreed[0]);

	//get the description
	cout << "\nEnter a brief description of this dog's personality." << endl;
	getline(cin, newDescription); newDescription[0] = toupper(newDescription[0]);

	//clear screen for aesthetics
	system("CLS"); 

	//display the new Dog's attributes
	cout << "Name: " << newName << endl;
	cout << "Age: " << newAge << endl;
	cout << "Gender: " << newGender << endl;
	cout << "Breed: " << newBreed << endl;
	cout << "Description: " << newDescription << endl;
	
	//get the yes/no input
	yn = yesNoInput("Finalize this new dog? (Y/N): ");
	
	//no was chosen, discard the dog
	if (yn == 'N'){ cout << "Dog discarded. Returning to main menu..." << endl; system("pause"); system("CLS"); }
	
	//yes was chosen, set the new Dog's attributes and add it to the data structures
	if (yn == 'Y')
	{ 
		newDog->setName(newName); 
		newDog->setBreed(newBreed);
		newDog->setGender(newGender);
		newDog->setAge(newAge);
		newDog->setDescription(newDescription);
		
		//add the Dog to the Hash Table
		dogHash->put(newDog);
		
		//add the Dog to the AVL Tree
		dogTree->insert(newDog);
		
		cout << "Dog finalized. Returning to main menu..." << endl;
		system("pause"); system("CLS");
	}

	return true;
}

/*
	removeDog removes a Dog from the data structures if found by ID search
	@param	-	dogHash is a Hash Table of Dog objects
	@param	-	dogTree is an AVL Tree of Dog objects
	@post	-	the Dog is removed from the Hash Table and AVL Tree if found
*/
bool removeDog(HashMap* dogHash, avlTree* dogTree)
{
	string dogId;
	
	//get the Dog ID
	cout << "Enter the ID of the dog to remove.  Use the format \"DOG###\"\n";
	cout << "Enter the ID here: ";
	cin >> dogId;
	cout << endl;

	//repeat getting the Dog ID until valid
	while (!isValidDogId(dogId))
	{
		cout << "Invalid input.  Use the format \"DOG###\"\n";
		cout << "Enter the ID here: ";
		cin >> dogId;
		cout << endl;
	}

	//remove the Dog from the Hash Table
	dogHash->remove(dogId);

	//remove the Dog from the AVL Tree
	dogTree->deleteNode(dogId);

	return true;
}

/*
	displayDogInfoByIdSearch performs a Hash Table search to find a Dog by ID and displays its attributes if found
	@param	-	dogHash is a Hash Table of Dog objects
*/
bool displayDogInfoByIdSearch(HashMap* dogHash)
{
	string dogId;
	
	//get the Dog ID
	cout << "Enter the ID of the Dog to search for.  Use the format \"DOG###\"\n";
	cout << "Enter the ID here: ";
	cin >> dogId;
	cout << endl;

	//repeat getting the Dog ID until valid
	while (!isValidDogId(dogId))
	{
		cout << "Invalid input.  Use the format \"DOG###\"\n";
		cout << "Enter the ID here: ";
		cin >> dogId;
		cout << endl;
	}
	
	//get the Dog from the Hash Table
	Dog *dog = dogHash->get(dogId);

	//if the Dog was not found, display a 'not found' message
	if (dog == nullptr)
		cout << "Dog not found" << endl << endl;
	//if the Dog was found, display its attributes
	else
		cout << dogHash->get(dogId)->toString() << endl;

	return true;
}

/*	
	display data structure efficiences including Load Factor, Longest Linked List, and Average Number of Nodes in Linked List
	@param	-	dogHash is a Hash Table of Dog objects
	@param	-	dogTree is an AVL Tree of Dog objects
*/
bool displayEfficiencyReport(HashMap* dogHash, avlTree* dogTree)
{
	cout << "Load Factor:\t\t" << dogHash->getLoadFactor() << endl;
	cout << "Height of AVL Tree:\t" << dogTree->getHeight() << endl;
	cout << endl;
	
	return true;
}

/*
	TEAM CHOICE

	updateDog searches the Hash Table for a Dog and allows the user to update its attributes if found
	
	If found:
	The original Dog's attributes are saved in strings before the Dog is removed from the Hash Table and AVL Tree
	The user is given a menu to update the Dog's name, age, and description (by changing the strings)
	A new Dog is created with the saved attributes (and any changes made to name, age, or description) of the original Dog
	The new Dog is added to the Hash Table and AVL Tree

	If not found:
	A 'not found' message is displayed

	@param	-	dogHash is a Hash Table of Dog objects
	@param	-	dogTree is an AVL Tree of Dog objects
	@post	-	if found, the Dog is deleted and a new Dog is added to take its place
*/
bool updateDog(HashMap *dogHash, avlTree *dogTree)
{
	string dogId;

	//get the Dog ID
	cout << "Enter the ID of the Dog to update.  Use the format \"DOG###\"\n";
	cout << "Enter the ID here: ";
	cin >> dogId;
	cout << endl;

	//repeat getting the Dog ID until valid
	while (!isValidDogId(dogId))
	{
		cout << "Invalid input.  Use the format \"DOG###\"\n";
		cout << "Enter the ID here: ";
		cin >> dogId;
		cout << endl;
	}

	//get the Dog from the Hash Table
	Dog *dog = dogHash->get(dogId);

	//if the Dog was not found, display a 'not found' message
	if (dog == nullptr)
		cout << "Dog not found" << endl << endl;
	//if the Dog was found, display  and save its attributes
	else
	{
		cout << dog->toString() << endl;
		string id = dog->getID();
		string name = dog->getName();
		string age = dog->getAge();
		string gender = dog->getGender();
		string breed = dog->getBreed();
		string desc = dog->getDescription();

		//remove the Dog from the Hash Table
		dogHash->remove(dogId);

		//remove the Dog from the AVL Tree
		dogTree->deleteNode(dogId);
		
		//display the menu and process choices until the user quits (chooses option 4)
		int choice = 0;
		while (choice != 4)
		{
			updateDogMenu();
			choice = getUpdateDogMenuChoice();
			processUpdateDogMenuChoice(choice, id, name, age, gender, breed, desc);
		}

		//create a new Dog with the saved attributes (and any changes made to name, age, or description)
		Dog* updatedDog = new Dog(id, name, age, gender, breed, desc);

		//add the Dog to the Hash Table
		dogHash->put(updatedDog);

		//add the Dog to the AVL Tree
		dogTree->insert(updatedDog);
	}

	return true;
}

/*
	TEAM CHOICE

	updateDogMenu displays a menu of options
*/
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

/*
	TEAM CHOICE

	getUpdateDogMenuChoice() gets a valid menu choice from the user and returns it
	@return	-	choice is a valid menu choice (integer between 1 and 4)
*/
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

/*
	TEAM CHOICE

	processUpdateDogMenuChoice handles each menu option
	@pre	-	choice is an integer between 1 and 4
	@param	-	id is a reference to the string holding the Dog id
	@param	-	name is a reference to the string holding the Dog name
	@param	-	age is a reference to the string holding the Dog age
	@param	-	gender is a reference to the string holding the Dog gender
	@param	-	breed is a reference to the string holding the Dog breed
	@param	-	desc is a reference to the string holding the Dog description
*/
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

/*
	TEAM CHOICE

	updateName gets a new name from the user and stores it in name
	@param	-	name is a reference to the string holding the Dog name
*/
bool updateName(string& name)
{
	string dogName;
	cout << "Enter a new name: ";
	cin.ignore();
	getline(cin, dogName);
	cout << endl;
	name = dogName;
	return true;
}

/*
	TEAM CHOICE

	updateAge gets a new age from the user and stores it in age
	@param	-	age is a reference to the string holding the Dog age
*/
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

/*
	TEAM CHOICE

	updateDesc gets a new description from the user and stores it in desc
	@param	-	desc is a reference to the string holding the Dog description
*/
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

/*
	isValidDogId return true or false depending on whether a given Dog ID is formatted correctly
	@param	-	dogId is the Dog ID to be evaluated
	@return -	isValid is true if the dogId is correctly formatted or false if dogId is incorrectly formatted
*/
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


