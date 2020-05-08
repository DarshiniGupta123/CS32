#include "provided.h" 
#include "MyHash.h"
#include <string>
using namespace std;

class TranslatorImpl
{
public:
	TranslatorImpl();
	bool pushMapping(string ciphertext, string plaintext);
	bool popMapping();
	string getTranslation(const string& ciphertext) const;
private:
	MyHash<char, char> translating;
	string alphabet = "ABCDEFGHIJKLMNOPQRSTUVWXYZ";
	vector<MyHash<char,char>> stack;
};

TranslatorImpl::TranslatorImpl()
{
	for (int i = 0; i != alphabet.size(); i++)
		translating.associate(alphabet[i], '?');
}

bool TranslatorImpl::pushMapping(string ciphertext, string plaintext)
{
	if (ciphertext.size() != plaintext.size())
		return false;
	for (int i = 0; i != plaintext.size(); i++)
	{
		if (!isalpha(plaintext[i]))
			return false;
		if (translating.find(ciphertext[i]) != nullptr && (plaintext[i] != *translating.find(ciphertext[i]))) //THIS IS WRONG TO DO!!!!!!!
			return false;
	}

	//save current mapping table on stack
	stack.push_back(translating);

	for (int j = 0; j != plaintext.size(); j++)
		translating.associate(ciphertext[j], plaintext[j]);

	return true;
}

bool TranslatorImpl::popMapping()
{
	if (stack.empty())
		return false;  // This compiles, but may not be correct
	//translating = stack[stack.size() - 1];
	stack.erase(stack.begin() + (stack.size() - 1));
	return true;
}

string TranslatorImpl::getTranslation(const string& ciphertext) const
{
	string translation;
	for (int i = 0; i != ciphertext.size(); i++)
	{
		if (isalpha(ciphertext[i]))
		{
			if (translating.find(ciphertext[i]) == nullptr)
				translation[i] = '?';
			else if (isupper(ciphertext[i]))
				translation[i] = toupper(*translating.find(ciphertext[i]));
			else
				translation[i] = tolower(*translating.find(ciphertext[i]));
		}
		else
			translation[i] = ciphertext[i];
	}
	return translation;
}

//******************** Translator functions ************************************

// These functions simply delegate to TranslatorImpl's functions.
// You probably don't want to change any of this code.

Translator::Translator()
{
	m_impl = new TranslatorImpl;
}

Translator::~Translator()
{
	delete m_impl;
}

bool Translator::pushMapping(string ciphertext, string plaintext)
{
	return m_impl->pushMapping(ciphertext, plaintext);
}

bool Translator::popMapping()
{
	return m_impl->popMapping();
}

string Translator::getTranslation(const string& ciphertext) const
{
	return m_impl->getTranslation(ciphertext);
}
