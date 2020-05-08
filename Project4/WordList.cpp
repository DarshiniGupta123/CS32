#include "provided.h"
#include "MyHash.h"
#include <fstream>
#include <string>
#include <vector>
using namespace std;

class WordListImpl
{
public:
	bool loadWordList(string filename);
	bool contains(string word) const;
	vector<string> findCandidates(string cipherWord, string currTranslation) const;

private:
	MyHash<string, vector<string>> wordList;

	string wordPattern(string s) const
	{
		MyHash<char, int> m1;
		int counter = 0;
		string key;
		for (int i = 0; i != s.size(); i++)
		{
			if (m1.find(s[i]) == nullptr)
			{
				m1.associate(s[i], counter);
				counter++;
			}
		}
		for (int i = 0; i != s.size(); i++)
			key += *m1.find(s[i]); //can I make value a string like that

		return key;
	}
};


bool WordListImpl::loadWordList(string filename)
{
	
	wordList.reset();

	ifstream infile("filename.txt");
	if (!infile)
		return false;
	else
	{
		std::string s;
		while (getline(infile, s))
		{
			bool answer = true;
			for (int i = 0; i != s.size(); i++)
			{
				if (!isalpha(s[i]) && s[i] != '\'')
					answer = false;
			}
			if(answer)
			{
				string value = wordPattern(s);

				if (wordList.find(value) != nullptr)
					(wordList.find(value))->push_back(s);
				else
				{
					vector<string> tempWordList;
					tempWordList.push_back(s);
					wordList.associate(value, tempWordList);
				}
			}
		}
		return true;
	}
}

bool WordListImpl::contains(string word) const
{
	/*if (wordList.find(word) == nullptr)
		return false; // This compiles, but may not be correct how do i make this not case sensititve
	else
		return true;*/
	vector<string> y;
	string key = wordPattern(word);
	y = *wordList.find(key);

	for (int i = 0; i != word.size(); i++)
		tolower(word[i]);

	for (int i = 0; i != y.size(); i++)
	{
		string compare = y[i];
		for (int j = 0; i != compare.size(); j++)
		{
			tolower(compare[j]);
		}
		if (compare == word)
			return true;
	}

	return false;

}

vector<string> WordListImpl::findCandidates(string cipherWord, string currTranslation) const
{
	vector<string> x; //empty vector throughout the function
	vector<string> y; //vector that contains the possible words that match the letter pattern
	vector<string> z; // final vector that adds the words that work with the current translation 


	string key = wordPattern(cipherWord);
	 y = *wordList.find(key);

	 for (int i = 0; i != y.size(); i++)
	 {
		 bool add = true;
		 string compare = y[i];
		 for (int j = 0; j != currTranslation.size(); j++)
		 {
			 if (isalpha(currTranslation[j]))
			 {
				 char c = tolower(currTranslation[j]);
				 char d = tolower(cipherWord[j]);
				 char e = tolower(compare[j]);
				 if (c != d)
					 return x;
				 else if (c != e)
					 add = false;
			 }
			 if (currTranslation[j] == '?')
			 {
				 if (!isalpha(cipherWord[j]))
					 return x;
				 else if (!isalpha(compare[j]))
					 add = false;
			 }
			 if (currTranslation[j] == '\'')
			 {
				 if (cipherWord[j] != '\'')
					 return x;
				 else if (compare[j] != '\'')
					 add = false;
			 }
		 }
		 if (add)
			 z.push_back(compare);
	 }
	 return z;
}

//***** hash functions for string, int, and char *****

unsigned int hash(const std::string& s)
{
	return std::hash<std::string>()(s);
}

unsigned int hash(const int& i)
{
	return std::hash<int>()(i);
}

unsigned int hash(const char& c)
{
	return std::hash<char>()(c);
}

//******************** WordList functions ************************************

// These functions simply delegate to WordListImpl's functions.
// You probably don't want to change any of this code.

WordList::WordList()
{
	m_impl = new WordListImpl;
}

WordList::~WordList()
{
	delete m_impl;
}

bool WordList::loadWordList(string filename)
{
	return m_impl->loadWordList(filename);
}

bool WordList::contains(string word) const
{
	return m_impl->contains(word);
}

vector<string> WordList::findCandidates(string cipherWord, string currTranslation) const
{
	return m_impl->findCandidates(cipherWord, currTranslation);
}
