// Coming soon
#include "provided.h"
#include <iostream>
using namespace std;

void f()
{
	WordList wl;
	if (!wl.loadWordList("wordlist.txt"))
	{
		cout << "Unable to load word list" << endl;
		return;
	}
	if (wl.contains("onomatopoeia"))
		cout << "I found onomatopoeia!" << endl;
	else
		cout << "Onomatopoeia is not in the word list!" << endl;
	string cipher = "xyqbbq";
	string decodedSoFar = "?r????";
	vector<string> v = wl.findCandidates(cipher, decodedSoFar);
	if (v.empty())
		cout << "No matches found" << endl;
	else
	{
		cout << "Found these matches:" << endl;
		for (size_t k = 0; k < v.size(); k++)
			cout << v[k] << endl; // writes grotto and troppo
	}
}
