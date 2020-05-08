// MyHash.h

// Skeleton for the MyHash class template.  You must implement the first seven
// member functions; we have implemented the eighth.

template<typename KeyType, typename ValueType>
class MyHash
{
public:
	MyHash(double maxLoadFactor = 0.5)
	{
		numBuckets = 100;
		numAssociations = 0;
		myArray = new Association*[numBuckets];
		for (int i = 0; i < numBuckets; i++)
			myArray[i] = nullptr;
	}

	~MyHash()
	{
		clear();
	}
	void reset()
	{
		clear();
		numBuckets = 100;
		numAssociations = 0;
		myArray = new Association[numBuckets];
		for (int i = 0; i < numBuckets; i++)
			myArray[i] = nullptr;
	}

	void associate(const KeyType& key, const ValueType& value)
	{
		unsigned int hash(const KeyType& k);
		int bucketNum = hash(key) % numBuckets;
		if (myArray[bucketNum] == nullptr)
		{
			resize();
			Association* node = new Association; //new node points to association structure
			node->m_key = key; // the key node points to is set to the key parameter
			node->m_value = value; //the value node points to is set to the value parameter
			node->m_next = myArray[bucketNum]; // m_next is now null ptr
			myArray[bucketNum] = node; // head is set to the node 
			numAssociations++; //list has now one more node
		}
		else
		{
			Association* curr;
			for (curr = myArray[bucketNum]; curr->m_next != NULL && curr->m_key != key; curr = curr->m_next); //loops through until the end of the list or the key is already in the list
			if (curr->m_next == nullptr) // if the key is not in the list
			{
				resize();
				Association* node = new Association;
				node->m_key = key;
				node->m_value = value;
				node->m_next = curr->m_next; //node is appended to the end of the list and m-next is current's m_next
				curr->m_next = node; //curr's m_next is the next node
				numAssociations++; //list has one more association

			}
			else //if the key is already in the list
				curr->m_value = value; //change the value to the value in the parameter
		}
	}
	int getNumItems() const
	{
		return numAssociations;
	}

	double getLoadFactor() const
	{
		return ((numAssociations*1.0) / numBuckets); //i dont think this is right
	}

	// for a map that can't be modified, return a pointer to const ValueType
	const ValueType* find(const KeyType& key) const
	{
		Association* curr;
		int bucketNum = hash(key) % numBuckets; //will this return the same thing it was stored as
		for (curr = myArray[bucketNum]; curr->m_next != NULL && curr->m_key != key; curr = curr->m_next);
		if (curr->m_next == nullptr)
			return nullptr;
		else
			return curr;// ->m_value; //IS THIS RIGHT
	}

	// for a modifiable map, return a pointer to modifiable ValueType
	ValueType* find(const KeyType& key)
	{
		return const_cast<ValueType*>(const_cast<const MyHash*>(this)->find(key));
	}

	// C++11 syntax for preventing copying and assignment
	MyHash(const MyHash&) = delete;
	MyHash& operator=(const MyHash&) = delete;

private:
	struct Association
	{
		KeyType m_key;
		ValueType m_value;
		Association* m_next = nullptr;
	};

	void clear()
	{
		for (int i = 0; i < numBuckets; i++)
		{
			Association* cur = myArray[i];
			while (cur != nullptr)//loops through the entire list
			{
				Association* temp = cur->m_next;
				delete cur; //deletes each node
				cur = temp;
			}
		}
		delete[] myArray; //do I need this
	}

	double getNewLoad()
	{
		return ((numAssociations*1.0) + 1) / numAssociations;
	}

	void resize()
	{
		if (getNewLoad() <= maxLoadFactor)
			return;

		Association** newArray = new Association*[numBuckets * 2];

		for (int i = 0; i < numBuckets * 2; i++)
			newArray[i] = nullptr;
		numBuckets = numBuckets * 2;

		Association** tempArray = myArray;
		myArray = newArray;
		numAssociations = 0;

		for (int i = 0; i < numBuckets / 2; i++)
		{
			Association* head = tempArray[i];
			while (head != nullptr)
			{
				associate(head->m_key, head->m_value);
				Association* curr = head;
				head = head->m_next;
				delete curr;
			}
		}
		delete[] tempArray;
	}

	Association** myArray;
	double maxLoadFactor;
	int numBuckets;
	int numAssociations;
};
