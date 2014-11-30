#ifndef HASH_MAP
#define HASH_MAP
#include "LinkedHashEntry.h"

#include <iostream>
#include <string>
#include <cstdlib>
#include <fstream>

using namespace std;

/*
	HashMap code adapted from the following source:
	http://www.algolist.net/Data_structures/Hash_table/Chaining
*/
class HashMap
{
private:
	const int TABLE_SIZE = 31;	//TABLE_SIZE is a prime number
	LinkedHashEntry **table;	//table is an array of pointers

public:
	/*
		HashMap constructor initializes table as a new array of LinkedHashEntry of size TABLE_SIZE, then assigns each index to null
	*/
	HashMap();

	/*
		~HashMap destructor resets the keyNumGenerator to 1 then calls clear() to destroy the Hash Table
	*/
	~HashMap();

	/*
		get searches the Hash Table for a Dog with the same id as key, then returns a pointer to that Dog or null if not found
		@pre	-	a properly formatted key string (DOG###)
		@param	-	key is the ID of the Dog to search for
		@return	-	a pointer to the Dog or null if not found
	*/
	Dog* get(string key);

	/*
		put adds a Dog pointer to the Hash Table
		@param	-	dog is the Dog pointer to be added
		@post	-	dog is placed in its proper position in the Hash Table
	*/
	void put(Dog* dog);

	/*
		remove searches the Hash Table for a Dog with the same id as key, then removes that Dog's pointer from the Hash Table if found
		@pre	-	a properly formatted key string (DOG###)
		@param	-	key is the ID of the Dog to remove
		@post	-	the Dog pointer is removed from the Hash Table
	*/
	void remove(string key);

	/*
		display displays the attributes of each Dog in the Hash Table by iterating through each table element and each element's linked chain
	*/
	void display();
	
	/*
		writeToFile writes the dogs in the hash table to the file.
		@pre    -	a file has been succesfully opened using the ofstream
		@param  -	out, an ofstream reference variable referring to a file that has been opened
		@post   -	Using the ofstream, the file has been written to.
		@return -	boolean indicating that the file has been succesfully written to
	*/
	bool HashMap::writeToFile(ofstream& out);
	
	/*
		clear deletes all the entries in the Hash Table and deletes table
		@post	-	the Hash Table is deleted
	*/
	void clear();

	/*
		getEfficiency calculates the Load Factor, Longest Linked List, and Average Number of Nodes in Linked List
		@param	-	longestList is a reference to an int that stores the longest linked list in the Hash Table
		@param	-	linksPerElement is a reference to a double that stores the average number of links per occupied element in the Hash Table
		@return	-	loadFactor is the load factor of the Hash Table
	*/
	double getEfficiency(int& longestList, double& linksPerElement);

	/*
		getLast3Digits gets the last 3 digits of a dogId for hashing calculations
		@pre	-	dogId is a valid Dog ID (DOG###)
		@return	-	last3Num is the last 3 digits of dogId
	*/
	int getLast3Digits(string dogId);
};

#endif
