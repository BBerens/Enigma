#include <string>

using namespace std;

const string wheelStrings[10][2] =
{
	{"EKMFLGDQVZNTOWYHXUSPAIBRCJ", "R"},	// Rotor I
	{"AJDKSIRUXBLHWTMCQGZNPYFVOE", "F"},	// Rotor II
	{"BDFHJLCPRTXVZNYEIWGAKMUSQO", "W"},	// Rotor III
	{"ESOVPZJAYQUIRHXLNFTGKDCMWB", "K"},	// Rotor IV
	{"VZBRGITYUPSDNHLXAWMJQOFECK", "A"},	// Rotor V
	{"JPGVOUMFYQBENHZRDKASXLICTW", "AN"},	// Rotor VI 2 notched points therefore turns over twice as often
	{"NZJHGRCXMYSWBOUFAIVLPEKQDT", "AN" },	// Rotor VII 2 notched points therefore turns over twice as often
	{"FKQHTLXOCBJSPDZRAMEWNIUYGV", "AN" },	// Rotor VIII 2 notched points therefore turns over twice as often
	{"LEYJVCNIXWPBQMDRTAKZGFUHOS", ""},		// M4 Greek Rotor "b" (beta) M4 rotors do not auto rotate. They can only be moved manually.
	{"FSOKANUERHMBTIYCWLQPZXVGJD", ""},		// M4 Greek Rotor "g" (gama) M4 rotors do not auto rotate. They can only be moved manually.
};

const string reflectors[5] =
{
	"EJMZALYXVBWFCRQUONTSPIKHGD",	// Reflector A
	"YRUHQSLDPXNGOKMIEBFZCWVJAT",	// Reflector B
	"FVPJIAOYEDRZXWGCTKUQSBNMHL",	// Reflector C
	"ENKQAUYWJICOPBLMDXZVFTHRGS",	// Thin B Reflector M4 only
	"RDOBJNTKVEHMLFCWZAXGYIPSUQ"	// Thin C Reflector M4 only
};

class Node
{
public:
	Node *next;
	Node *prev;
	char index;
	bool notched;
	Node(char);
	~Node();
	void setNotched();
};

class CircularList
{
public:
	Node *head;
	int count;
	CircularList(string);
	~CircularList();
	char getOutput(char);
	char getReverseOutput(char);
	void insertNode(char);
	Node * findChar(char);
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
	Rotor();
	Rotor(string, string);
	~Rotor();
	void setRingChars(string);
	void setNotches(string);
	string getNotches();
	void setRing(char);
	void setFirstChar(char);
	char ReverseMap(char);
	char map(char);
	void rotate();
	void ChangeRotor(int);
	char GetWindowChar();
};

class Reflector
{
private:
	CircularList *charList;
public:
	Reflector();
	Reflector(int);
	char map(char);
	void ChangeReflector(int);
};


class Plugboard
{
private:
	char dictionary[26];
public:
	Plugboard();
	Plugboard(char *);
	void SetPlugs(char *);
	char Translate(char);


};

class Enigma
{
private:
	Rotor * rotors[4];
	Plugboard *plugboard;
	Reflector *reflector;
	int numRotors;

public:
	Enigma();
	Enigma(char *, char *, char *, char *);
	bool SetConfig(string);
	void SetRotor(int, char);
	void SetRing(int, char);
	void SetChar(int, char);
	void Rotate(int);
	string Encrypt(string);
	char EncryptChar(char);
	char DebugEncryptChar(char);

};
