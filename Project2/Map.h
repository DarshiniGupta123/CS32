#ifndef MAP_INCLUDED
#define MAP_INCLUDED

#include <string>

using KeyType = std::string;
using ValueType = double;

const int DEFAULT_MAX_ITEMS = 250;
class Map
{
public:
	Map();
	Map(const Map& other);
	Map& operator = (const Map& other);
	~Map();
	bool empty() const;
	int size() const;
	bool insert(const KeyType& key, const ValueType& value);
	bool update(const KeyType& key, const ValueType& value);
	bool insertOrUpdate(const KeyType& key, const ValueType& value);
	bool erase(const KeyType& key);
	bool contains(const KeyType& key) const;
	bool get(const KeyType& key, ValueType& value) const;
	bool get(int i, KeyType& key, ValueType& value) const;
	void swap(Map& other);
private:

	// Since this structure is used only by the implementation of the
	// Map class, we'll make it private to Map.

	struct Pair
	{
		KeyType   m_key;
		ValueType m_value;
		Pair* m_next = NULL;
		Pair* m_prev = NULL;
	};

	Pair* head;
	int num;
};

bool combine(const Map& m1, const Map& m2, Map& result);
void subtract(const Map& m1, const Map& m2, Map& result);
#endif //MAP_INCLUDED
