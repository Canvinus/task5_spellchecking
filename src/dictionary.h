#ifndef  _DICTIONARY_H_ 
#define  _DICTIONARY_H_

#include  <iostream>
#include  <vector>
#include  <list>
#include  <algorithm>
#include  <string>
#include  <stdlib.h>
#include <fstream>

#include  "hashset.h"
#include  "hashset.cpp"

using namespace std;

class hash_function
{
public:

    unsigned int operator()( const string& s )  const {

        unsigned int hash = 0;
        for (int i = 0; i < s.length(); ++i)
        {
            hash += s[i];
            hash -= (hash << 13) | (hash >> 19);
        }
        return hash;
        
    }
};

class equality
{
public:
    equality() {}
    bool  operator()( const string& A, const string& B )  const 
    {
        return  (A == B);
    }
};

class Dictionary: public HashSet<string, hash_function, equality> {

public:
	Dictionary(string filename)
	{
        ifstream lst(filename);

        if (!lst.good())
            throw invalid_argument("Error occured while opening " + filename);

        string newWord;
        while (lst.good())
        {
            lst >> newWord;
            this->insert(newWord);
        }

        lst.close();
    };

};
#endif // _DICTIONARY_H_
