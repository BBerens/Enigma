#include <string>

using namespace std;

#define ASCII 128
#define ALPHABETSIZE 26
#define STARTCHAR 'A'

class Node
{
public:
	Node *next;
	char index;
	bool notched;
	Node(char);
	//~Node();
	//void setNotched();
};

class CircularList
{
public:
	Node *head;
	Node *tail;
	int count;
	CircularList(string);
	~CircularList();
	char getOutput(char);
	char getReverseOutput(char);
	void insertNode(char);
	//Node * findChar(char);
};

class Rotor
{
private:
	CircularList *charList;
	CircularList *reverseList;
	int ringOffset;
	int rotCount;
	string notchedChars;
public:
	//Rotor();
	Rotor(string);
	~Rotor();
	//void setRingChars(string);
	void setNotches(string);
	bool getNotches();
	void setRing(char);
	void setFirstChar(char);
	char adjust(char, int);
	char ReverseMap(char);
	char map(char);
	void rotate();
	//void ChangeRotor(int);
	//char GetWindowChar();
};

class Reflector
{
private:
	CircularList *charList;
public:
	//Reflector();
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

class Plugboard
{
private:
	char dictionary[26];
public:
	Plugboard(string);
	void SetPlugs(string);
	char Translate(char);
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

	//Enigma();
	Enigma(string, string, string, string, string);
	void initializeStrings(void);
	//bool SetConfig(string);
	//void SetRotor(int, char);
	//void SetRing(int, char);
	//void SetChar(int, char);
	void Rotate(int);
	string Encrypt(string);
	char EncryptChar(char);
	//char DebugEncryptChar(char);
	int charHash(char);

};
