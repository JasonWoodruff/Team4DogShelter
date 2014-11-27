#ifndef DOG_H
#define DOG_H
#include <string>
#include <iostream>
#include <sstream>
#include <queue>

using namespace std;

class Dog
{
public: 
	string ID;    //format DOG001, DOG002
private:                         
	string name;
	string age;
	string gender;
	string breed;
	string description;
	
protected:
	//generates a Dog ID based on a static variable which counts the instances of Dogs
	string generateID();
	
public:
	static int keyNumGenerator; //counts number of keys generated
	static queue<string> nextAvailable;

	/*Default constructor creates a dogID only*/
	Dog();
	
	/**Constructor assigns all fields*/
	Dog(string strID, string strN, string strGen, string strAge, string strB, string strD);
	
	/**Accessor methods*/
	string getID() const;
	string getName() const;
	string getAge() const;
	string getGender() const;
	string getBreed() const;
	string getDescription() const;
	
	/**Mutator methods*/
	void setID(string key);
	void setName(string n);
	void setAge(string a);
	void setGender(string a);
	void setBreed(string type);
	void setDescription(string desc);

	/**Print out the Dog's info*/
	string toString();
	
	/* Override ostream - for when saving to file*/
	friend ostream& operator<<(ostream& os, const Dog& dt);
};

#endif
