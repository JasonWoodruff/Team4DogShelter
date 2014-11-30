#include "HashMap.h"

HashMap::HashMap()
{
	table = new LinkedHashEntry*[TABLE_SIZE];
	for (int i = 0; i < TABLE_SIZE; i++)
		table[i] = nullptr;
}

HashMap::~HashMap()
{
	Dog::setKeyNumGenerator(1);
	clear();
}

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

double HashMap::getLoadFactor()
{
	double entries = 0;

	for (int i = 0; i < TABLE_SIZE; i++)
	{
		if (table[i] != nullptr)
		{
			LinkedHashEntry *entry = table[i];

			while (entry != nullptr)
			{
				entries++;
				entry = entry->getNext();
			}
		}
	}
	double loadFactor = entries / TABLE_SIZE;
	return loadFactor;
}

int HashMap::getLast3Digits(string dogId)
{
	string last3Str = dogId.substr(dogId.length() - 3, 3);
	int last3Num = atoi(last3Str.c_str());
	return last3Num;
}