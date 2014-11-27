#ifndef LINKED_HASH_ENTRY
#define LINKED_HASH_ENTRY
#include "Dog.h"

/*JASON I got the Hash Table working and reading from the file properly.  Play around with options 2 and 7 in the menu because they now work as intended.  
We will continue to work with add/delete and the other Hash/Tree functions once we have the Tree.  

James - I apologize for taking over your part, but since this is our last week, we need to have this working ASAP.  
For now, I want you to focus on understanding the code, and please ask any questions about it.
Once you have it down, let us know, because we'll need your help on the rest of the project, especially the presentation.
*/

class LinkedHashEntry
{
private:
	string key; //this will be a string with the dog id (ex. "DOG002")
	/*JAMES: I left in the above string because I wasn't sure if it was needed. hashVal is the actual assigned hash value
	assigned to each dog node. It's derived from the Dog objects ID number (ie string "DOG002" has a hash value of '02'
	as an int. I don't know if this string belongs, but I'll leave it there until I know for sure.
	JASON: The string is needed because the conversion to digits is only needed to determine the placement of the Dog in the Hash Table.  
	The key and value must retain their original values when they are put in the Table so additional entries and searches can compare against them.*/
	Dog *value; //the Dog object associated with the dog id
	LinkedHashEntry *next;

public:
	LinkedHashEntry(Dog *entry)
	{
		this->key = entry->getID();
		this->value = entry;
		this->next = nullptr;
	}

	string getKey()
	{
		return key;
	}

	Dog *getValue()
	{
		return value;
	}

	void setValue(Dog *value)
	{
		this->value = value;
	}

	LinkedHashEntry *getNext()
	{
		return next;
	}

	void setNext(LinkedHashEntry *next)
	{
		this->next = next;
	}
};

#endif
