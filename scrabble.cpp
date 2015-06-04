#include <map>
#include <set>
#include <string>
#include <iostream>
#include <iomanip>
#include <fstream>
#include <algorithm>

using namespace std;

typedef map<char, short> LetterValue;

void subStrings (const string &sref, set<string> &subs)
{
    string str = sref;
    sort ( str.begin(), str.end() );
    
    do{
        for (unsigned i = 0; i<str.length(); i++)
        {
            subs.insert (str.substr(0, i+1));
        } // for all the letters in the string
        
    }while (next_permutation(str.begin(), str.end()));
    
} // void subStrings

class Word
{
    int value;
    string word;

    friend class Scrabble;
    
public:
    Word(string w = string(""), int v = 0) : value(v), word(w)
    {
        
    } // 
    
    bool operator< (const Word &rhs) const{
        
        if (word.compare(rhs.word) < 0)
            return true;
        return false;
        
    }
    
    bool operator == (string & rhs) const{
    
        if (word.compare(rhs) == 0)
            return true;
        return false;
    
    }
    
};

class Scrabble
{
    
    LetterValue letterValues;
    set<Word> wordBank;

public:

    Scrabble();
    void readBank(const char *file);
    bool validWord(const string &word) const;
    int scoreWord(const string &word);
    int maxScore(const set<Word> &bank);
    void unscramble(string jumble);

};


Scrabble::Scrabble()
{
    short values [ ] = { 1, 3, 3, 2, 1, 4, 2, 4, 2, 8, 5, 1, 3, 1, 1, 3, 10, 1, 1, 1, 1, 4, 4, 8, 4, 10 };
    
    for (int i = 0; i < 26; i++)
        letterValues.insert( LetterValue::value_type((char)i + 'a', values[i]) );
}

void Scrabble::readBank (const char *file)
{
    ifstream inf(file);
    string word;
    while (getline(inf, word)){
        if (validWord (word))
            wordBank.insert(Word(word, scoreWord(word)));
    }
    
    inf.close();
}

bool Scrabble::validWord(const string &word) const
{
    string::const_iterator i;
    int j = 0;
    
    for (i = word.begin(); word.length() <= 7; i++) //
    {
        j++;
        
        if ((*i > 'z') || (*i < 'a'))
            break;
        
        if (j == (int)word.length())
            return true;
    } // for all the letters in the word
    return false;
} // validWord

int Scrabble::scoreWord (const string &word)
{
    int score = 0;
    for (string::const_iterator iter = word.begin(); iter<word.end(); iter++)
        score += letterValues.find(*iter)->second;
    return score;
}

int Scrabble::maxScore(const set<Word> &bank)
{
    int max = 0;
    for (set<Word>::const_iterator iter = bank.begin(); iter != bank.end(); iter++)
        if (iter->value > max)
            max = iter->value;
    return max;
}

void Scrabble::unscramble (string jumble)
{
    set<Word> matches;
    set<Word>::iterator ref;
    Word word;
    
    set<string> searchSpace;
    subStrings(jumble, searchSpace);
    
    for(set<string>::const_iterator iter = searchSpace.begin(); iter != searchSpace.end(); iter++)
    {
        word = Word(*iter, scoreWord(*iter));
        ref = wordBank.find(word);
        if (ref != wordBank.end())
            matches.insert(*ref);
    }
    
    int max = (int)maxScore (matches);
    cout << setw(2) << max << ' ' << jumble << ':';
    if ((max == 0) || (jumble.length() > 7)) cout << "No words found.\n";
    else
    {
        for (set<Word>::const_iterator iter = matches.begin(); iter != matches.end(); iter++)
        {
            if (iter->value == max)
                    cout << ' ' << iter->word;
        }
        
        cout << endl;
    }
    
}

int main(int argc, char **argv)
{
    Scrabble game;
    game.readBank("words.txt");
    ifstream test (argv[1]);
    string word;
    
    while(getline(test, word))
        game.unscramble(word);
    test.close();
    
    return 0;
}
