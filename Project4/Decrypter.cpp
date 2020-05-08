#include "provided.h"
#include <string>
#include <vector>
using namespace std;

class DecrypterImpl
{
public:
	bool load(string filename);
	vector<string> crack(const string& ciphertext);
private:
	WordList wl;
	Translator t1;
	vector<string> output;
	
	vector<bool> chosen;
	int numChosen = 0;

	string getCipherWord(vector<string> w)
	{
		if (numChosen == 0)
		{
			string max = w[0];
			for (int i = 0; i != w.size(); i++)
			{
				if (w[i].size() > max.size())
					max = w[i];
			}
			return max;
		}
		for (int i = 0; i != w.size(); i++)
		{
			int count = 0;
			int MaxCount = 0;
			string rightWord;
			string word = t1.getTranslation(w[i]);
			for (int j = 0; j != word.size(); j++)
			{
				if (word[j] == '?')
					count++;
			}
			if (count > MaxCount && chosen[i] == false)
				rightWord = w[i];
		}


	}
};

bool DecrypterImpl::load(string filename)
{
	if (!wl.loadWordList("filename.txt"))
		return false;
	return true;
}

vector<string> DecrypterImpl::crack(const string& ciphertext)
{
	Tokenizer token(" ,;:.!()[] {}-\"#$%^&0123456789");

	vector<string> words = token.tokenize(ciphertext);

	string cipherWord = getCipherWord(words);

	string currentTranslation = t1.getTranslation(cipherWord);

	vector<string> possible = wl.findCandidates(cipherWord, currentTranslation);

	if (possible.empty())
	{
		t1.popMapping();
		return;
	}
	for (int i = 0; i != possible.size(); i++)
	{
		if (t1.pushMapping(cipherWord, possible[i]))
		{
			string partialMessage = t1.getTranslation(ciphertext); //can i do this instead of making a temporary mapping table
			//TO DO : check fully translated words
			int count = 0;
			for (int i = 0; i != partialMessage.size(); i++)
			{
				if (partialMessage[i] == '?')
					count++;
			}
			if (count == 0)
				output.push_back(partialMessage);
			else
				crack(ciphertext);
		}
	}
	return output;
}

//******************** Decrypter functions ************************************

// These functions simply delegate to DecrypterImpl's functions.
// You probably don't want to change any of this code.

Decrypter::Decrypter()
{
	m_impl = new DecrypterImpl;
}

Decrypter::~Decrypter()
{
	delete m_impl;
}

bool Decrypter::load(string filename)
{
	return m_impl->load(filename);
}

vector<string> Decrypter::crack(const string& ciphertext)
{
	return m_impl->crack(ciphertext);
}
