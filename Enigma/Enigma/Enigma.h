#include <string>

using namespace std;

const string wheelStrings[27][2] =
{
	//real rotors, alphabetSize = 26, startChar = 'A'	
	{ "DMTWSILRUYQNKFEJCAZBPGXOHV", "" },	// Enigma A,B IC
	{ "HQZGPJTMOBLNCIFDYAWVEUSRKX", "" },	// Enigma A,B IIC
	{ "UQNTLSZFMREHDPXKIBVYGJCWOA", "" },	// Enigma A,B IIIC

	{ "JGDQOXUSCAMIFRVTPNEWKBLZYH", "N" },	// German Railway I
	{ "NTZPSFBOKMWRCJDIVLAEYUXHGQ", "E" },	// German Railway II
	{ "JVIUBHTCDYAKEQZPOSGXNRMWFL", "Y" },	// German Railway III

	{ "LPGSZMHAEOQKVXRFYBUTNICJDW", "Y" },	// Enigma D,K I
	{ "SLVGBTFXJQOHEWIRZYAMKPCNDU", "E" },	// Enigma D,K II
	{ "CJGDPSHKTURAWZXFMYNQOBVLIE", "N" },	// Enigma D,K III

	{ "KPTYUELOCVGRFQDANJMBSWHZXI", "WZEKQ" },	// Enigma T I
	{ "UPHZLWEQMTDJXCAKSOIGVBYFNR", "WZFLR" },	// Enigma T II
	{ "QUDLYRFEKONVZAXWHMGPJBSICT", "WZEKQ" },	// Enigma T III
	{ "CIWTBKXNRESPFLYDAGVHQUOJZM", "WZFLR" },	// Enigma T IV
	{ "UAXGISNJBVERDYLFZWTPCKOHMQ", "YCFKR" },	// Enigma T V
	{ "XFUZGALVHCNYSEWQTDMRBKPIOJ", "XEIMQ" },	// Enigma T VI
	{ "BJVFTXPLNAYOZIKWGDQERUCHSM", "YCFKR" },	// Enigma T VII
	{ "YMTPNZHWKODAJXELUQVGCBISFR", "XEIMQ" },	// Enigma T VIII

	{ "EKMFLGDQVZNTOWYHXUSPAIBRCJ", "Q" },	// Enigma M1,M3,M4 I
    { "AJDKSIRUXBLHWTMCQGZNPYFVOE", "E" },	// Enigma M1,M3,M4 II
	{ "BDFHJLCPRTXVZNYEIWGAKMUSQO", "V" },	// Enigma M1,M3,M4 III
	{ "ESOVPZJAYQUIRHXLNFTGKDCMWB", "J" },	// Enigma M1,M3,M4 IV
	{ "VZBRGITYUPSDNHLXAWMJQOFECK", "Z" },	// Enigma M1,M3,M4 V
	{ "JPGVOUMFYQBENHZRDKASXLICTW", "ZM" },	// Enigma M1,M3,M4 VI 
	{ "NZJHGRCXMYSWBOUFAIVLPEKQDT", "ZM" },	// Enigma M1,M3,M4 VII
	{ "FKQHTLXOCBJSPDZRAMEWNIUYGV", "ZM" },	// Enigma M1,M3,M4 VIII
	{ "LEYJVCNIXWPBQMDRTAKZGFUHOS", "" },	// M4 Greek Rotor "b" (beta)
	{ "FSOKANUERHMBTIYCWLQPZXVGJD", "" },	// M4 Greek Rotor "g" (gama)
};

const string reflectors[10] =
{
	"ABCDEFGHIJKLMNOPQRSTUVWXYZ",  // Enigma A,B did not have reflectors so there was translation
	"QYHOGNECVPUZTFDJAXWMKISRBL",  // German Railway Reflector
	"IMETCGFRAYSQBZXWLHKDVUPOJN",  // Swiss K Reflector
	"LEYJVCNIXWPBQMDRTAKZGFUHOS",  // M4 Beta Rotor
	"FSOKANUERHMBTIYCWLQPZXVGJD",  // M4 Gamma Rotor
	"EJMZALYXVBWFCRQUONTSPIKHGD",  // M1,M3,M4 Reflector A
	"YRUHQSLDPXNGOKMIEBFZCWVJAT",  // M1,M3,M4 Reflector B
	"FVPJIAOYEDRZXWGCTKUQSBNMHL",  // M1,M3,M4 Reflector C
	"ENKQAUYWJICOPBLMDXZVFTHRGS",  // M4 R1 (M3 + Thin)
	"RDOBJNTKVEHMLFCWZAXGYIPSUQ",  // M4 R1 (M3 + Thin)
};

const string entryDiscs[3] =
{
	"ABCDEFGHIJKLMNOPQRSTUVWXYZ",  // standard no translation
	"QWERTZUIOASDFGHJKPYXCVBNML",  // Qwerty based translation
	"KZROUQHYAIGBLWVSTDXFPNMCJE"   // Enigma T-specific entry disc
};

const int alphabetSize = 26;

const char startChar = 'A';

class Node
{
public:
	Node *next;
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
	Node *tail;
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
	Rotor(string);
	~Rotor();
	void setRingChars(string);
	void setNotches(string);
	bool getNotches();
	void setRing(char);
	void setFirstChar(char);
	char adjust(char, int);
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
};

class EntryDisc
{
private:
	CircularList * charList;
public:
	EntryDisc(char *);
	char map(char);
	char ReverseMap(char);
};

class Plugboard
{
private:
	char dictionary[26];
public:
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
	EntryDisc *entryDisc;

public:
	Enigma();
	Enigma(char *, char *, char *, char *, char *);
	bool SetConfig(string);
	void SetRotor(int, char);
	void SetRing(int, char);
	void SetChar(int, char);
	void Rotate(int);
	string Encrypt(string);
	char EncryptChar(char);
	//char DebugEncryptChar(char);

};
