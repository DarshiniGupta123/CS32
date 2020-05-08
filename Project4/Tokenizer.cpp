#include "provided.h"
#include <string>
#include <vector>
using namespace std;

class TokenizerImpl
{
public:
	TokenizerImpl(string separators);

	vector<string> tokenize(const std::string& s) const;

private:
	std::string m_separators;
	std::vector<std::string> tokens;
};

TokenizerImpl::TokenizerImpl(string separators)
{
	m_separators = separators; //check time complexity for the constructor
}

vector<string> TokenizerImpl::tokenize(const std::string& s) const
{
	std::vector<std::string> vec;
	std::string word = "";
	bool add = false;
	for (int i = 0; i != s.size(); i++)
	{
		for (int j = 0; j != m_separators.size(); j++)
		{
			add = false;
			if (s[i] == m_separators[j])
			{
				add = true;
				break;
			}
		}
		if (add) {
			if (word.size() > 0)
				vec.push_back(word);
			word = "";
			add = false;
		}
		else
			word += s[i];
	}
	return vec;

}

//******************** Tokenizer functions ************************************

// These functions simply delegate to TokenizerImpl's functions.
// You probably don't want to change any of this code.

Tokenizer::Tokenizer(string separators)
{
	m_impl = new TokenizerImpl(separators);
}

Tokenizer::~Tokenizer()
{
	delete m_impl;
}

vector<string> Tokenizer::tokenize(const std::string& s) const
{
	return m_impl->tokenize(s);
}
