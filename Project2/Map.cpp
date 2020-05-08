#include "Map.h"
#include <string>
using namespace std;

Map::Map()
{
	head = nullptr; //initializes the linked list 
	num = 0;
}

bool Map::empty() const
{
	if (num == 0) //if the counter size equals 0 then returns true
		return true;
	return false;
}

int Map::size() const
{
	return num; //returns the size of the list
}

bool Map::insert(const KeyType& key, const ValueType& value)
{
	Pair* curr;
	if (head == nullptr) //if the list is empty 
	{
		Pair* node = new Pair; //new node points to pair structure
		node->m_key = key; // the key node points to is set to the key parameter
		node->m_value = value; //the value node points to is set to the value parameter
		node->m_next = head; // m_next is now null ptr
		head = node; // head is set to the node 
		num++; //list has now one more node
		return true;
	}
	else
	{
		for (curr = head; curr->m_next != NULL && curr->m_key != key; curr = curr->m_next); //loops through until the end of the list or the key is already in the list
		if (curr->m_next == NULL) // if the key is not in the list
		{
			Pair* node = new Pair;
			node->m_key = key;
			node->m_value = value;
			node->m_next = curr->m_next; //node is appended to the end of the list and m-next is current's m_next
			node->m_prev = curr; //curr is the previous node for the new node
			curr->m_next = node; //curr's m_next is the next node
			num++; //list has one more node
			return true;
		}
	}
	return false;
}

bool Map::update(const KeyType& key, const ValueType& value)
{
	Pair* p;
	for (p = head; p != NULL && p->m_key != key; p = p->m_next)//loops through until the end of the list or the p is pointing to the key in the list
		;
	if (p != NULL) // if the key is in the list
	{
		p->m_value = value; // the value in the list is updated to the value in the parameter
		return true;
	}
	return false;
}

bool Map::insertOrUpdate(const KeyType& key, const ValueType& value)
{
	Pair* curr;

	if (head == nullptr) //if the list is empty
	{
		Pair* node = new Pair; // 
		node->m_key = key;//the key node points to is set to the value parameter
		node->m_value = value; //the value node points to is set to the value parameter
		node->m_next = head;
		head = node;// head is set to the node 
		num++;
		return true;
	}
	else
	{
		for (curr = head; curr->m_next != NULL && curr->m_key != key; curr = curr->m_next) // finds where the key is and leaves the loop or when the list is at the end
			;
		if (curr != NULL) // if the key is in the list
			curr->m_value = value; //the value parameter is copied into the value in the node

		else if (curr->m_next == NULL) //if the key is not in the list
		{
			Pair* node = new Pair; //key is inserted into the end of the function like the insert function
			node->m_key = key;
			node->m_value = value;
			node->m_next = curr->m_next;//curr is the previous node for the new node
			node->m_prev = curr;//curr's m_next is the next node
			curr->m_next = node;
			num++;
		}
	}
	return true;
}

bool Map::erase(const KeyType& key)
{
	Pair* p;
	for (p = head; p != NULL && p->m_key != key; p = p->m_next) //finds where the key is and leaves the loop or when the list is at the end
		;
	if (p != NULL)//if the key is in the list 
	{
		if (p->m_prev == NULL && p->m_next == NULL) //if the list is empty
			head = NULL;
		else if (p->m_prev == NULL) //if this was the first node in the list
		{
			head = p->m_next; //head now points to the m_next
			head->m_prev = NULL;
		}
			
		else if (p->m_next == NULL) //if this is the last node in the list
			p->m_prev->m_next = NULL;
		else//if this is in the middle of the list
		{
			p->m_next->m_prev = p->m_prev;
			p->m_prev->m_next = p->m_next;
		}
		delete p;
		num--;
		return true;
	}
	return false;

}

bool Map::contains(const KeyType& key) const
{
	Pair* p;
	for (p = head; p != NULL && p->m_key != key; p = p->m_next)//loops through until the end of the list or the p is pointing to the key in the list
		;
	if (p == NULL) //if the loop reaches the end of the list than the key is not in the list
		return false; //returns false
	return true;
}

bool Map::get(const KeyType& key, ValueType& value) const
{
	Pair* p;
	for (p = head; p != NULL && p->m_key != key; p = p->m_next) //loops through until the pointer is pointing to the key that is the parameter or the key is not in the list
		;
	if (p != NULL) //if the key is in the list
	{
		value = p->m_value; //the value is copied into the value parameter
		return true;
	}
	return false;
}

bool Map::get(int i, KeyType& key, ValueType& value) const
{
	Pair* p = head;
	if (i >= 0 && i < size()) //if i is a valid size and position for the list
	{
		if (empty()) //if the list is empty it automatically returns false
			return false;
		
		for (int j = 0; j != i; j++) //until j reaches the position i
			p = p->m_next; // p keeps pointing to te next node
		key = p->m_key; //the value is copied into the key parameter
		value = p->m_value;//the value is copied into the value parameter
		return true;
	}
	return false;//if the key is not in the list or i is not a valid position
}

void Map::swap(Map& other)
{
	Pair* temp = head; //stores the address of the head in temp variable
	head = other.head; //this head equals the other list's head pointer, not connecting this head to the complete list
	other.head = temp; //the other head equals this head which was stored in the temp

	int t = num; //swaps the counter for size for both lists
	num = other.num;
	other.num = t;
}

Map::Map(const Map& other)
{
	num = 0; //num is initialized to 0
	head = nullptr; //head is initalized to null
	Pair* sum = other.head;
	while (sum != NULL) //loops through the entire other list
	{
		insert(sum->m_key, sum->m_value); //inserts each value from the other list to this list
		sum = sum->m_next; //increments sum to point to the next node
	}
}

Map& Map::operator = (const Map& other)
{
	if (this != &other) //if the two objects are different
	{
		Pair* cur = head;
		while (cur != NULL) //while the curr is not null
		{
			Pair* temp = cur->m_next;
			delete cur; // the node is deleted
			cur = temp;
		} //to prevent memory leaks

		num = 0; //num is set to the other object's num
		head = nullptr;
		Pair* sum = other.head;
		while (sum != NULL) //loops through the list to copy each node into the new map list
		{
			insert(sum->m_key, sum->m_value);
			sum = sum->m_next;
		}
	}
	return *this;
}

Map::~Map() // is a Map destructor
{

	Pair* cur = head; 
	while (cur != NULL)//loops through the entire list
	{
		Pair* temp = cur->m_next;
		delete cur; //deletes each node
		cur = temp;
	}
}

bool combine(const Map& m1, const Map& m2, Map& result)
{

	KeyType z; //initialize a string and a double 
	ValueType z1;

	bool answer = true; // initialize a boolean variable to true


	if (&result != &m1 && &result != &m2) //if result does not point to the same list
	{
		while (!result.empty()) //while result is not empty
		{
			result.get(0, z, z1); //erase each node
			result.erase(z);
		}
	}


	for (int i = 0; i < m1.size(); i++) {
	    KeyType x;
		ValueType y1, y2;
		if (m1.get(i, x, y1)) //gets the value and key pair at that index
		{
			if (m2.get(x, y2) == false || y1 == y2)//if m2 does not have that value and the get function returns false or both of them have the same pair
				result.insert(x, y1); //the pair is inserted into the result list
			else
				answer = false; //if the same key is in both list with different values, the function returns false and nothing is inserted
		}

    }
	for (int i = 0; i < m2.size(); i++)
	{
		KeyType x;
		ValueType y1, y2;
		if (m2.get(i, x, y1))//gets the value and key pair at that index
		{
			if (m1.get(x, y2) == false || y1 == y2) //if m1 does not have that value and the get function returns false or both of them have the same pair
				result.insert(x, y1); //the pair is inserted into the result list
			else
				answer = false; //if the same key is in both list with different values, the function returns false and nothing is inserted
		}


	}
	return answer;
}

void subtract(const Map& m1, const Map& m2, Map& result)
{
	KeyType z;
	ValueType z1;


	if (&m1 == &result) //if result and m1 are aliases
	{
		Map tempMap(m1); //create a temp map using copy constructor
		result = tempMap; //set the address of the temp map to the result so you can delete things without affecting m1
		while(!result.empty()) //while result is not empty
		{
			
			result.get(0, z, z1);
			result.erase(z); //erases each node from result
		}

	}

	if (&m2 == &result) // if result and m2 are aliases
	{
		Map tempMap(m2);//create a temp map using copy constructor
		result = tempMap;//set the address of the temp map to the result so you can delete things without affecting m2
		while (!result.empty())//while result is not empty
		{

			result.get(0, z, z1);
			result.erase(z);//erases each node from result
		}

	}

	if (&m1 == &m2) //if both m1 and m2 point to the same thing
	{
		while (!result.empty())
		{

			result.get(0, z, z1); //result should empty everything and remain empty because all the elements are the same
			result.erase(z);
		}
	}

	else 
	{
		while (!result.empty()) //otherwise erase everything from result
		{

			result.get(0, z, z1);
			result.erase(z);
		}

		for (int i = 0; i < m1.size(); i++)  //loops through all of the 1st list
		{
			KeyType x;
			ValueType y1, y2;
			if (m1.get(i, x, y1)) //gets the value and key pair at that index
			{
				if (!m2.get(x, y2)) //if m2 does not have that value and the get function returns false
					result.insert(x, y1); //that key and value pair are inserted into result
			}

		}
		
	}

}