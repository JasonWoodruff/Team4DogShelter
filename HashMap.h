#ifndef HASH_MAP
#define HASH_MAP
#include "LinkedHashEntry.h"

#include <iostream>
#include <string>
#include <cstdlib>

using namespace std;

//this header file doesn't compile, don't include quite yet

const int TABLE_SIZE = 31; //modified
int getLast3Digits(string dogId); /*JASON This function converts the last 3 characters of a string into an integer*/

class HashMap
{
private:
	LinkedHashEntry **table;

public:
	HashMap()
	{
		table = new LinkedHashEntry*[TABLE_SIZE];
		for (int i = 0; i < TABLE_SIZE; i++)
			table[i] = nullptr;
	}

	~HashMap()
	{
		clear();
	}
	
	/*JASON The user would type something like "DOG003" which would get passed to get*/
	Dog *get(string key)
	{
		int hash = (getLast3Digits(key) % TABLE_SIZE); /*JASON note how the conversion happens here.  We don't want to change the key itself because it's needed for comparisons with other Dogs in the table later.  Only int hash changes.*/
		if (table[hash] == nullptr)
			return nullptr;
		else
		{
			LinkedHashEntry *entry = table[hash];
			while (entry != nullptr && entry->getKey() != key) /*JASON This is one of the comparisons I mentioned.*/
				entry = entry->getNext();
			if (entry == nullptr)
				return nullptr;
			else
				return entry->getValue();
		}
	}

	/*JASON We will pass a dog to this function.  This happens repeatedly when we read in the file.*/
	void put(Dog *dog)
	{
		int hash = (getLast3Digits(dog->getID()) % TABLE_SIZE); /*JASON another conversion to get int hash*/
		if (table[hash] == nullptr)	/*JASON if this element is empty, put the Dog here*/
			table[hash] = new LinkedHashEntry(dog);	
		else
		{
			LinkedHashEntry *entry = table[hash];
			while (entry->getNext() != nullptr) /*JASON if the element was occupied, walk the linked list until you find get to null, then put the Dog there*/
				entry = entry->getNext();
			if (entry->getKey() == dog->getID()) /*If the same key already exists in the hash table, overwrite its value with the value of the Dog we're passing in.  Note:  Keys must be unique, but as long as we do the ID's correctly this should never occur.*/
				entry->setValue(dog);
			else
				entry->setNext(new LinkedHashEntry(dog)); /*JASON got to null, put the Dog here*/
		}
	}

	void remove(string key)
	{
		int hash = (getLast3Digits(key) % TABLE_SIZE); /*JASON another conversion*/
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

	//Used by displayInHashSequence() function
	void display()
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

	//Modification of Bryson's code
	bool writeToFile(ofstream& out)
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

	void clear()
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
};

int getLast3Digits(string dogId)
{
	string last3Str = dogId.substr(dogId.length() - 3, 3);
	int last3Num = atoi(last3Str.c_str());
	return last3Num;
}

#endif