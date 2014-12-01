using namespace std;

#include<iostream>
#include <string>
#include <sstream>

#include "Dog.h"

int Dog::keyNumGenerator = 1;		//increments when each new instance of Dog is created, unless the Dog uses a recycled ID from nextAvailable
queue<string> Dog::nextAvailable;	//stores deleted Dog IDs to be given to new Dogs

Dog::Dog()
{ 
	ID = generateID();
}

Dog::Dog(string strID, string strN, string strGen, string strAge, string strB, string strD) :
ID(strID), name(strN), gender(strGen), age(strAge), breed(strB), description(strD)
{
	keyNumGenerator++;
}

string Dog::getID() const
{ return ID; }

string Dog::getName() const
{ return name; }

string Dog::getAge() const
{ return age; }

string Dog::getGender() const
{ return gender; }

string Dog::getBreed() const
{ return breed; }

string Dog::getDescription() const
{ return description; }

int Dog::getKeyNumGenerator()
{ return keyNumGenerator; }

string Dog::getNextAvailable()
{ 
	string nextId = nextAvailable.front();
	nextAvailable.pop();
	return nextId; 
}

void Dog::setID(string key)
{ ID = key; }

void Dog::setName(string n)
{ name = n; }

void Dog::setAge(string a)
{ age = a; }

void Dog::setGender(string a)
{gender = a;}

void Dog::setBreed(string type)
{ breed = type; }

void Dog::setDescription(string desc)
{ description = desc; }

void Dog::setKeyNumGenerator(int key)
{ keyNumGenerator = key; }

void Dog::setNextAvailable(string id)
{ nextAvailable.push(id); }


string Dog::generateID()
{
	//no Dog IDs to recycle, make a new one
	if (nextAvailable.empty())
	{
		int tempID = keyNumGenerator++; //next number available for id assignment
		string pad = "";				//a cushion of filler 0s
		stringstream s;
		if (tempID < 10)				//create the filler 0s if needed
			pad = "00";
		else if (tempID < 100)
			pad = "0";
		else if (tempID < 1000)
			pad = "";
		s << "DOG" << pad << tempID;	//concatenate all parts of id
		return s.str();
	}
	//reuse an old Dog ID from the queue
	else
	{
		string newId = nextAvailable.front();
		nextAvailable.pop();
		return newId;
	}	
}

/*Return a string with a Dog's attributes*/
string Dog::toString()
{
	stringstream s;
	s << ID << "\n" << name << "\n" << gender << "\n" << age << "\n" << breed << "\n" << description << "\n";
	return s.str();
}

ostream& operator<<(ostream& os, const Dog& dt)
{
	os << dt.getID() << "," << dt.getName() << "," << dt.getGender() << "," << dt.getAge() << "," << dt.getBreed() << "," << dt.getDescription();
    return os;
}