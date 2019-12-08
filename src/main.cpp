#include <iostream>
#include <fstream>
#include <string>
#include <sstream>
#include <cstdlib>
#include <cctype>
#include <set>
#include <iomanip> // setw


#include "dictionary.h"

using namespace std;

void lower(string& s);
string stripPunct(const string& s);
void checkSpelling(ifstream& in, Dictionary& dict);

string swap(const string& word, int ai, int bi)
{
    string newStr = word;
    newStr[ai] = word[bi];
    newStr[bi] = word[ai];

    return newStr;
}

string remove(const string& word, int index)
{
    string newStr;
    for (int i = 0; i < word.length(); ++i)
    {
        if (i != index)
            newStr += word[i];
    }

    return newStr;
}

string insertAt(const string& word, char letter, int index)
{
    string newStr;
    for (int i = 0; i < index; ++i)
        newStr += word[i];

    newStr += letter;
    for (int i = index; i < word.length(); ++i)
        newStr += word[i];

    return newStr;
}

// program arguments to run, example: main.exe ../../res/wordlist.txt ../../res/test.txt
int main(int argc, char* argv[]) {
	
	// Output usage message if improper command line args were given.
	if (argc != 3)
    {
		cerr << "Usage: " << argv[0] << " wordlist_filename input_file\n";
		return EXIT_FAILURE;
	}

	ifstream inf(argv[2]);
	if (! inf) 
    {
		cerr << "Could not open " << argv[2] << "\n";
		return EXIT_FAILURE;
	}
	
	// Read dictionary, but let user know what we are working on.
	cout << "Loading dictionary, this may take awhile...\n";

	Dictionary d(argv[1]);
    
	checkSpelling(inf, d);

	inf.close();

	return EXIT_SUCCESS;
}

void checkSpelling(ifstream& in, Dictionary& dict) {

	int line_number = 0;

	while (in) 
    {
		line_number++;
		set<string> totest;
		string line;
		getline(in, line);

  	    stringstream ss (stringstream::in | stringstream::out);
		ss << line;
		
		string word;
		while (ss >> word) 
        {
            // Remove punctuation and make all words lowercase
            word = stripPunct(word);
            lower(word);

            // Searching for suggestions
            if (!dict.search(word))
            {
                cout << "The word '" << word << "' at the line " << line_number << " may be wrong.\nSuggestions:\n";

                set<string> suggestions;

                // Transposing of adjacent letters
                for (int i = 0; i < word.length() - 1; ++i)
                {
                    string newWord = swap(word, i, i + 1);
                    if (dict.search(newWord))
                        suggestions.insert(newWord);
                }

                // Removing each letter
                for (int i = 0; i < word.length(); ++i)
                {
                    string newWord = remove(word, i);
                    if (dict.search(newWord))
                        suggestions.insert(newWord);
                }

                // Replacement of all letters
                for (int i = 0; i < word.length(); ++i)
                    for (char j = 'a'; j <= 'z'; ++j)
                    {
                        string newWord = word;
                        newWord[i] = j;
                        if (dict.search(newWord))
                            suggestions.insert(newWord);
                    }

                // Inserting letters at any position in a word
                for (int i = 0; i <= word.length(); ++i)
                    for (char letter = 'a'; letter <= 'z'; letter++)
                    {
                        string newWord = insertAt(word, letter, i);
                        if (dict.search(newWord))
                            suggestions.insert(newWord);
                    }

                // Printing suggestions
                for (set<string>::iterator it = suggestions.begin(); it != suggestions.end(); ++it)
                    cout << "\t\t" << *it << "\n";
            }
		}
	}
}

void lower(string& s) {

    // Ensures that a word is lowercase
	for (int i = 0; i < s.length(); i++)
    {
		s[i] = tolower(s[i]);
	}
}

string stripPunct(const string& s) {

	// Remove any single trailing
	// punctuation character from a word.  
	if (ispunct(s[s.length() - 1]) ) 
    {
		return s.substr (0, s.length() - 1);
	}
	else 
    {
		return s;
	}
}
