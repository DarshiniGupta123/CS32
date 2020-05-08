// Map.h

#ifndef MAP_INCLUDED
#define MAP_INCLUDED

template <typename KeyType, typename ValueType>

class Map
{
public:
	Map()
		: m_size(0)
	{
		// create dummy node
		m_head = new Node;
		m_head->m_next = m_head;
		m_head->m_prev = m_head;
	}// Create an empty map (i.e., one with no key/value pairs).

	bool empty() const;  // Return true if the map is empty, otherwise false.

	int size() const;    // Return the number of key/value pairs in the map.

	bool insert(const KeyType& key, const ValueType& value);
	
	bool update(const KeyType& key, const ValueType& value);
	
	bool insertOrUpdate(const KeyType& key, const ValueType& value);

	bool contains(const KeyType& key) const;

	bool erase(const KeyType& key)
	{
		Node* p = find(key);

		if (p == m_head)  // not found
			return false;

		// unlink the node from the list and destroy it

		p->m_prev->m_next = p->m_next;
		p->m_next->m_prev = p->m_prev;
		delete p;

		m_size--;
		return true;
	}

	//bool contains(const KeyType& key) const;

	bool get(const KeyType& key, ValueType& value) const
	{
		Node* p = find(key);
		if (p == m_head)  // not found
			return false;
		value = p->m_value;
		return true;
	}

	bool get(int i, KeyType& key, ValueType& value) const
	{
		if (i < 0 || i >= m_size)
			return false;

		Node* p;
		if (i < m_size / 2)  // closer to head
		{
			p = m_head->m_next;
			for (int k = 0; k != i; k++)
				p = p->m_next;
		}
		else  // closer to tail
		{
			p = m_head->m_prev;
			for (int k = m_size - 1; k != i; k--)
				p = p->m_prev;
		}

		key = p->m_key;
		value = p->m_value;
		return true;
	}

	void swap(Map& other)
	{
		// swap head pointers
		Node* tempHead = m_head;
		m_head = other.m_head;
		other.m_head = tempHead;

		// swap sizes
		int t = m_size;
		m_size = other.m_size;
		other.m_size = t;
	}

	~Map()
	{
		// Delete the m_size non-dummy nodes plus the dummy node

		for (Node* p = m_head->m_prev; m_size >= 0; m_size--)
		{
			Node* toBeDeleted = p;
			p = p->m_prev;
			delete toBeDeleted;
		}
	}

	Map(const Map& other)
		: m_size(other.m_size)
	{
		m_head = new Node;
		Node* prev = m_head;

		for (Node* p = other.m_head->m_next; p != other.m_head; p = p->m_next)
		{
			// Create a copy of the node p points to
			Node* pnew = new Node;
			pnew->m_key = p->m_key;
			pnew->m_value = p->m_value;

			// Connect the new node to the previous one
			pnew->m_prev = prev;
			prev->m_next = pnew;

			// Reset prev to last node created
			prev = pnew;
		}

		// Connect last node created to m_head 
		m_head->m_prev = prev;
		prev->m_next = m_head;
	}

	Map& operator=(const Map& rhs)
	{
		if (this != &rhs)
		{
			Map temp(rhs);
			swap(temp);
		}
		return *this;
	}

private:
	// Representation:
	//   a circular doubly-linked list with a dummy node.
	//   m_head points to the dummy node.
	//   m_head->m_prev->m_next == m_head and m_head->m_next->m_prev == m_head
	//   m_size == 0  iff  m_head->m_next == m_head->m_prev == m_head
	//   If m_size > 0
	//       m_head->next points to the node at position 0.
	//       m_head->prev points to the node at position m_size-1.
	//   Nodes are in no particular order.

	struct Node
	{
		KeyType   m_key;
		ValueType m_value;
		Node*     m_next;
		Node*     m_prev;
	};

	Node* m_head;
	int   m_size;

	Node* find(const KeyType& key) const
	{
		Node* p;
		for (p = m_head->m_next; p != m_head && p->m_key != key; p = p->m_next)
			;
		return p;
	}

	bool doInsertOrUpdate(const KeyType& key, const ValueType& value,
		bool mayInsert, bool mayUpdate)
	{
		Node* p = find(key);

		if (p != m_head)  // found
		{
			if (mayUpdate)
				p->m_value = value;
			return mayUpdate;
		}
		if (!mayInsert)  // not found, and not allowed to insert
			return false;

		// Create a new node
		p = new Node;
		p->m_key = key;
		p->m_value = value;

		// Insert new item at tail of list (arbitrary choice of position)
		//     Connect it to tail
		p->m_prev = m_head->m_prev;
		p->m_prev->m_next = p;

		//     Connect it to dummy node
		p->m_next = m_head;
		m_head->m_prev = p;

		m_size++;
		return true;
	}
};


// Declarations of non-member functions

template <typename KeyType, typename ValueType>
bool combine(const Map<KeyType, ValueType>& m1, const Map<KeyType, ValueType>& m2, Map<KeyType, ValueType>& result)
{
	const Map<KeyType, ValueType>* bigger;
	const Map<KeyType, ValueType>* smaller;
	if (m1.size() >= m2.size())
	{
		bigger = &m1;
		smaller = &m2;
	}
	else
	{
		bigger = &m2;
		smaller = &m1;
	}
	bool status = true;
	Map<KeyType, ValueType> res(*bigger);               // res starts as a copy of the bigger map
	for (int n = 0; n < smaller->size(); n++)  // for each pair in smaller
	{
		KeyType k;
		ValueType vsmall;
		smaller->get(n, k, vsmall);
		ValueType vbig;
		if (!res.get(k, vbig))      // key in smaller doesn't appear in bigger
			res.insert(k, vsmall);  //     so add it to res
		else if (vbig != vsmall)    // same key, different value
		{                           //     so pair shouldn't be in res
			res.erase(k);
			status = false;
		}
	}
	result.swap(res);
	return status;
}

template <typename KeyType, typename ValueType>
void subtract(const Map<KeyType, ValueType>& m1, const Map<KeyType, ValueType>& m2, Map<KeyType, ValueType>& result)
{
	if (m1.size() <= m2.size())
	{
		Map<KeyType, ValueType> res;
		for (int n = 0; n < m1.size(); n++)
		{
			KeyType k;
			ValueType v;
			m1.get(n, k, v);
			if (!m2.contains(k))
				res.insert(k, v);
		}
		result.swap(res);
	}
	else
	{
		Map<KeyType, ValueType> res(m1);
		for (int n = 0; n < m2.size(); n++)
		{
			KeyType k;
			ValueType v;
			m2.get(n, k, v);
			res.erase(k);
		}
		result.swap(res);
	}
}
// Inline implementations

template <typename KeyType, typename ValueType>

inline
int Map<KeyType, ValueType>::size() const
{
	return m_size;
}

template <typename KeyType, typename ValueType>
inline
bool Map<KeyType, ValueType>::empty() const
{
	return size() == 0;
}

template <typename KeyType, typename ValueType>
inline
bool Map<KeyType, ValueType>::contains(const KeyType& key) const
{
	return find(key) != m_head;
}

template <typename KeyType, typename ValueType>
inline
bool Map<KeyType, ValueType>::insert(const KeyType& key, const ValueType& value)
{
	return doInsertOrUpdate(key, value, true /* insert */, false /* no update */);
}

template <typename KeyType, typename ValueType>
inline
bool Map<KeyType, ValueType>::update(const KeyType& key, const ValueType& value)
{
	return doInsertOrUpdate(key, value, false /* no insert */, true /* update */);
}

template <typename KeyType, typename ValueType>
inline
bool Map<KeyType, ValueType>::insertOrUpdate(const KeyType& key, const ValueType& value)
{
	return doInsertOrUpdate(key, value, true /* insert */, true /* update */);
}

#endif // MAP_INCLUDED
