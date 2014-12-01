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

#ifndef LINKED_HASH_ENTRY
#define LINKED_HASH_ENTRY
#include "Dog.h"

class LinkedHashEntry
{
private:
	string key;				//Dog ID
	Dog* value;				//pointer to a Dog object
	LinkedHashEntry *next;	//pointer to the next entry

public:
	/*
		LinkedHashEntry constructor sets the value with a pointer to a Dog, sets the key with the ID of the pointer to a Dog, and sets next to null
		@param	-	entry is a pointer to a Dog
	*/
	LinkedHashEntry(Dog *entry)
	{
		this->key = entry->getID();
		this->value = entry;
		this->next = nullptr;
	}

	/*
		get the key
		@return key
	*/
	string getKey()
	{
		return key;
	}

	/*
		get the value
		@return value
	*/
	Dog *getValue()
	{
		return value;
	}

	/*
		set the value
		@param	-	value is a pointer to a Dog
	*/
	void setValue(Dog *value)
	{
		this->value = value;
	}

	/*
		get next (the pointer the LinkedHashEntry points to)
		@return	-	next
	*/
	LinkedHashEntry* getNext()
	{
		return next;
	}

	/*
		set next (the pointer the LinkedHashEntry points to)
		@param	-	next is a pointer to a LinkedHashEntry
	*/
	void setNext(LinkedHashEntry *next)
	{
		this->next = next;
	}
};

#endif
