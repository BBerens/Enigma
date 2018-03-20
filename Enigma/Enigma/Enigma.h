#include <string>

using namespace std;

#define ASCII 128
#define ALPHABETSIZE 26
#define STARTCHAR 'A'

class Node
{
public:
	Node * next;
	char index;
	bool notched;
	Node(char);
};

class CircularList
{
public:
	Node * head;
	Node *tail;
	int count;
	CircularList(string);
	char getOutput(char);
	char getReverseOutput(char);
	void insertNode(char);
};

class Plugboard
{
private:
	char dictionary[26];
public:
	Plugboard(string);
	char Translate(char);
};

class Rotor
{
private:
	CircularList * charList;
	CircularList *reverseList;
	int ringOffset;
	int rotCount;
	string notchedChars;
public:
	Rotor(string);
	void setNotches(string);
	string getNotches();
	void setRing(char);
	void setFirstChar(char);
	char adjust(char, int);
	char ReverseMap(char);
	char map(char);
	void rotate();
	char GetWindowChar();
};

class Reflector
{
private:
	CircularList * charList;
public:
	Reflector(string);
	char map(char);
};

class EntryDisc
{
private:
	CircularList * charList;
public:
	EntryDisc(string);
	char map(char);
	char ReverseMap(char);
};

class Enigma
{
private:
	Rotor * rotors[4];
	Plugboard *plugboard;
	Reflector *reflector;
	int numRotors;
	EntryDisc *entryDisc;
public:
	string wheelStrings[ASCII][2];
	string reflectors[ASCII];
	string entryDiscs[ASCII];

	Enigma(string, string, string, string, string);
	string Encrypt(string);
	char EncryptChar(char);
	void Rotate(int);
	int charHash(char);
	void initializeStrings(void);
};