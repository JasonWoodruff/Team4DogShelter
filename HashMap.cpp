/*
CIS22C Team 4 Project:	Dog Shelter Management Program

Purpose:	The function of our software is to store, organize and
manage data about dogs in a shelter.

@author
Jason Woodruff	-	Project Coordinator
Stacy Wheeler	-	Data Record
Bryson Li		-	I/O
James Juchau	-	Hashed Table
Kevin Chen		-	Binary Search Tree

@version	1.5
@since		11/30/2014
*/

#include "HashMap.h"

/*
Initialize the Hash Table
*/
HashMap::HashMap()
{
	table = new LinkedHashEntry*[TABLE_SIZE];
	for (int i = 0; i < TABLE_SIZE; i++)
		table[i] = nullptr;
}

/*
Reset the KeyNumGenerator and Delete the Hash Table
*/
HashMap::~HashMap()
{
	Dog::setKeyNumGenerator(1);
	clear();
}

/*
Get a Dog from the Hash Table
*/
Dog* HashMap::get(string key)
{
	int hash = (getLast3Digits(key) % TABLE_SIZE);
	if (table[hash] == nullptr)
		return nullptr;
	else
	{
		LinkedHashEntry *entry = table[hash];
		while (entry != nullptr && entry->getKey() != key)
			entry = entry->getNext();
		if (entry == nullptr)
			return nullptr;
		else
			return entry->getValue();
	}
}

/*
Add a Dog to the Hash Table
*/
void HashMap::put(Dog *dog)
{
	int hash = (getLast3Digits(dog->getID()) % TABLE_SIZE);
	if (table[hash] == nullptr)
		table[hash] = new LinkedHashEntry(dog);
	else
	{
		LinkedHashEntry *entry = table[hash];
		while (entry->getNext() != nullptr)
			entry = entry->getNext();
		if (entry->getKey() == dog->getID())
			entry->setValue(dog);
		else
			entry->setNext(new LinkedHashEntry(dog));
	}
}

/*
Remove a Dog from the Hash Table
*/
void HashMap::remove(string key)
{
	int hash = (getLast3Digits(key) % TABLE_SIZE);
	if (table[hash] != nullptr)
	{
		LinkedHashEntry *prevEntry = nullptr;
		LinkedHashEntry *entry = table[hash];
		while (entry->getNext() != nullptr && entry->getKey() != key)
		{
			prevEntry = entry;
			entry = entry->getNext();
		}
		if (entry->getKey() == key)
		{
			if (prevEntry == nullptr)
			{
				LinkedHashEntry *nextEntry = entry->getNext();
				delete entry;
				table[hash] = nextEntry;
			}
			else
			{
				LinkedHashEntry *next = entry->getNext();
				delete entry;
				prevEntry->setNext(next);
			}
		}
	}
}

/*
Display all the Dogs in the Hash Table
*/
void HashMap::display()
{
	for (int i = 0; i < TABLE_SIZE; i++)
	{
		if (table[i] != nullptr)
		{
			LinkedHashEntry *entry = table[i];
			cout << entry->getValue()->toString() << endl;;
			while (entry->getNext() != nullptr)
			{
				entry = entry->getNext();
				cout << entry->getValue()->toString() << endl;
			}
		}
	}
}

/*
Write the contents of the Hash Table to 'dog.txt'
*/
bool HashMap::writeToFile(ofstream& out)
{
	for (int i = 0; i < TABLE_SIZE; i++)
	{
		if (table[i] != nullptr)
		{
			LinkedHashEntry *entry = table[i];
			out << *entry->getValue() << "\n";

			while (entry->getNext() != nullptr)
			{
				entry = entry->getNext();
				out << *entry->getValue() << "\n";
			}
		}
	}
	return true;
}

/*
Delete the contents of the Hash Table
*/
void HashMap::clear()
{
	for (int i = 0; i < TABLE_SIZE; i++)
	{
		if (table[i] != nullptr)
		{
			LinkedHashEntry *prevEntry = nullptr;
			LinkedHashEntry *entry = table[i];
			while (entry != nullptr)
			{
				prevEntry = entry;
				entry = entry->getNext();
				delete prevEntry;
			}
		}
	}
	delete[] table;
}

/*
Get the efficiency of the Hash Table
*/
double HashMap::getEfficiency(int& longestList, double& linksPerElement)
{
	//number of entries
	double entries = 0;

	//stores the number of links in an occupied element
	int mostLinks = 0;

	//stores the total number of occupied elements in the Hash Table
	int elements = 0;

	//stores the longest list in the Hash Table (most collisions)
	longestList = 0;

	//stores the average number of links per occupied element
	linksPerElement = 0;

	for (int i = 0; i < TABLE_SIZE; i++)
	{
		mostLinks = 0;
		
		if (table[i] != nullptr)
		{
			elements++;
			LinkedHashEntry *entry = table[i];

			while (entry != nullptr)
			{
				mostLinks++;
				entries++;

				if (mostLinks > longestList)
					longestList = mostLinks;

				entry = entry->getNext();
			}
		}
	}
	
	if (elements > 0)
		linksPerElement = entries / elements;
	
	double loadFactor = entries / TABLE_SIZE;
	return loadFactor;
}

/*
Get the Last 3 Digits of a Dog ID
*/
int HashMap::getLast3Digits(string dogId)
{
	string last3Str = dogId.substr(dogId.length() - 3, 3);
	int last3Num = atoi(last3Str.c_str());
	return last3Num;
}