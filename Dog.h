#ifndef DOG_H
#define DOG_H
#include <string>
#include <iostream>
#include <sstream>
#include <queue>

using namespace std;

class Dog
{
private:   
	string ID;			//format DOG###
	string name;
	string age;
	string gender;
	string breed;
	string description;
	
	static int keyNumGenerator;			//counts number of keys generated
	static queue<string> nextAvailable;	//stores recycled IDs from deleted Dogs
	
protected:
	/*
		generate a unique Dog ID of the format DOG### if nextAvailable is empty, or get the front of nextAvailable then pop it if it isn't empty
		@post	-	keyNumGenerator is incremented or nextAvailable is popped
		@return	-	the Dog ID
	*/
	string generateID();
	
public:
	/*
		Dog default constructor creates a new Dog and calls generateID()
	*/
	Dog();
	
	/*
		Dog constructor creates a new Dog, sets all fields and increments keyNumGenerator
		@param	-	strID is the Dog ID
		@param	-	strN is the Dog name
		@param	-	strGen is the Dog gender
		@param	-	strAge is the Dog age
		@param	-	strB is the Dog breed
		@param	-	strD is the Dog description
	*/
	Dog(string strID, string strN, string strGen, string strAge, string strB, string strD);
	
	/**Accessor methods*/
	string getID() const;
	string getName() const;
	string getAge() const;
	string getGender() const;
	string getBreed() const;
	string getDescription() const;

	static int getKeyNumGenerator();

	/*
		get the front of the queue then pop it
		@pre	-	nextAvailable is not empty
		@post	-	nextAvailable is popped
		@return	-	front of nextAvailable
	*/
	static string getNextAvailable();
	
	/**Mutator methods*/
	void setID(string key);
	void setName(string n);
	void setAge(string a);
	void setGender(string a);
	void setBreed(string type);
	void setDescription(string desc);

	static void setKeyNumGenerator(int key);

	/*
		push an ID onto the queue
		@pre	-	id is a correctly formatted Dog ID (DOG###)
		@param	-	id is the string to push onto the queue containing the Dog ID
		@post	-	id is pushed onto the queue
	*/
	static void setNextAvailable(string id);

	/*
		create a string out of the Dog's attributes
		@return	-	a string with Dog attributes
	*/
	string toString();
	
	/* Override ostream - for when saving to file*/
	friend ostream& operator<<(ostream& os, const Dog& dt);
};

#endif
