#include "Enigma.h"
#include <iostream> // only present for debugging

Enigma::Enigma(string discStr, string rotorStr, string ringStr, string keyStr, string plugStr)
{
	initializeStrings();
	string discChars = entryDiscs[charHash(discStr[0])];
	entryDisc = new EntryDisc(discChars);
	plugboard = new Plugboard(plugStr);
	numRotors = rotorStr.length() - 1; // one of the chars is for the reflector
	for (int i = 0; i < numRotors; i++)
	{
		char wheelChar = rotorStr[numRotors - i - 1];
		char deleteMe = charHash(wheelChar);
		rotors[i] = new Rotor(wheelStrings[charHash(wheelChar)][0]);
		rotors[i]->setNotches(wheelStrings[charHash(wheelChar)][1]);
		rotors[i]->setRing(ringStr[numRotors - i - 1]);
		rotors[i]->setFirstChar(keyStr[numRotors - i - 1]);
	}
	string reflectorChars = reflectors[charHash(rotorStr[rotorStr.length() - 1])];
	reflector = new Reflector(reflectorChars);

}

string Enigma::Encrypt(string phrase)
{
	string output = "";
	char tempChar;
	for (int i = 0; i < phrase.length(); i++)
	{
		if (STARTCHAR <= phrase[i] && phrase[i] < (ALPHABETSIZE + STARTCHAR))
			tempChar = EncryptChar(phrase[i]);
		else
			tempChar = phrase[i];
		output.append(1,tempChar);
	}

	return output;
}

char Enigma::EncryptChar(char character)
{
	Rotate(0);
	character = entryDisc->map(character);
	character = plugboard->Translate(character);
	for (int i = 0; i < numRotors; i++)
	{
		character = rotors[i]->map(character);
	}

	character = reflector->map(character);
	for (int i = numRotors - 1; i >= 0; i--)
	{
		character = rotors[i]->ReverseMap(character);
	}

	character = plugboard->Translate(character);
	character = entryDisc->ReverseMap(character);
	
	return character;
}

void Enigma::Rotate(int rotorNum)
{	
	if (rotors[rotorNum]->getNotches().find(rotors[rotorNum]->GetWindowChar()) != -1)
		Rotate(rotorNum + 1);
	rotors[rotorNum]->rotate();
}

Rotor::Rotor(string rotorChars)
{
	setRingChars(rotorChars);
	ringOffset = 0;
	rotCount = 0;
}

Rotor::~Rotor()
{
	;
}

// seemed redundant

void Rotor::setRingChars(string ringChars)
{
// memory leak here. Need to remove charList
/*if (charList != NULL)
{
delete[] charList;
}*/
ringOffset = 0;
rotCount = 0;
charList = new CircularList(ringChars);
char reverseString[27];
for (int i = 0; i < ringChars.length(); i++)
{
	reverseString[ringChars[i] - 'A'] = i + 'A';
}
reverseString[26] = '\0';
reverseList = new CircularList(reverseString);
}


string Rotor::getNotches()
{
	return notchedChars;
}

char Rotor::GetWindowChar()
{
	return 'A' + (rotCount) % 26;
}

void Rotor::setRing(char lockedChar)
{
	ringOffset = 'A' - lockedChar;

	for (int i = 0; i < -ringOffset; i++)
	{
		charList->head = charList->head->prev;
		reverseList->head = reverseList->head->prev;
	}
}

void Rotor::setFirstChar(char startChar)
{
	int offset = startChar - 'A';

	for (int i = 0; i < offset; i++)
	{
		charList->head = charList->head->next;
		reverseList->head = reverseList->head->next;
		ringOffset++;
		rotCount++;
	}
}

void Rotor::setNotches(string notches) 
{
	notchedChars = notches;
}

char Rotor::map(char inChar)
{
	char outChar = charList->getOutput(inChar);
	outChar = 'A' + (outChar - 'A' + 26 - ringOffset % 26) % 26;
	return outChar;
}

char Rotor::ReverseMap(char inChar)
{
	char outChar = reverseList->getOutput(inChar);
	outChar = 'A' + (outChar - 'A' + 26 - ringOffset % 26) % 26;
	return outChar;
}

void Rotor::rotate()
{
	charList->head = charList->head->next;
	reverseList->head = reverseList->head->next;
	ringOffset++;
	rotCount++;
}

CircularList::CircularList(string charMap)
{
	count = 0;

	for (int i = 0; i < charMap.length(); i++)
	{
		this->insertNode(charMap[i]);		
	}
}

CircularList::~CircularList()
{
	// need to deal with this
	;
}

void CircularList::insertNode(char mapChar)
{
	Node *temp = new Node(mapChar);
	if (!head)
	{
		head = temp;
		head->next = head;
		head->prev = head;
		
	}
	else
	{
		temp->next = head;
		temp->prev = head->prev;
		head->prev->next = temp;
		head->prev = temp;		
	}
	count++;
}

Node * CircularList::findChar(char searchChar)
{
	Node * curNode = head;
	for (int i = 0; i < count; i++)
	{
		if (curNode->index == searchChar)
			return curNode;
		else
			curNode = curNode->next;
	}
	return NULL;
}

char CircularList::getOutput(char inChar)
{
	Node *curNode = head;
	int index = inChar - STARTCHAR;
	for (int i = 0; i < index; i++)
	{
		curNode = curNode->next;
	}
	return curNode->index;
}

char CircularList::getReverseOutput(char inChar)
{
	Node *curNode = head;
	char outChar = STARTCHAR;
	while (curNode->index != inChar)
	{
		curNode = curNode->next;
		outChar++;
	}
	return outChar;
}

Node::Node(char indexChar)
{
	index = indexChar;
	notched = false;
}

Plugboard::Plugboard(string letterPairs)
{
	SetPlugs(letterPairs);
}

void Plugboard::SetPlugs(string letterPairs)
{
	for (int i = 0; i < ALPHABETSIZE; i++)
		dictionary[i] = STARTCHAR + i;
	for (int i = 0; i < letterPairs.length(); i += 2)
	{
		dictionary[letterPairs[i] - STARTCHAR] = letterPairs[i + 1];
		dictionary[letterPairs[i + 1] - STARTCHAR] = letterPairs[i];
	}
}

char Plugboard::Translate(char inChar)
{
	return dictionary[inChar - STARTCHAR];
}

Reflector::Reflector(string reflectorChars)
 {
	charList = new CircularList(reflectorChars);
}

char Reflector::map(char inChar)
{
	return charList->getOutput(inChar);
}

EntryDisc::EntryDisc(string discChars)
{
	charList = new CircularList(discChars);
}

char EntryDisc::map(char inChar)
{
	return charList->getOutput(inChar);
}

char EntryDisc::ReverseMap(char inChar)
{
	return charList->getReverseOutput(inChar);
}


////////////////////////////////////////////////////

int Enigma::charHash(char inChar) { return (int)inChar; }

void Enigma::initializeStrings(void)
{
	//real rotors, alphabetSize = 26, startChar = 'A'

	wheelStrings[charHash('A')][0] = "DMTWSILRUYQNKFEJCAZBPGXOHV";		// Enigma A,B IC
	wheelStrings[charHash('A')][1] = "";
	wheelStrings[charHash('B')][0] = "HQZGPJTMOBLNCIFDYAWVEUSRKX";		// Enigma A,B IIC
	wheelStrings[charHash('B')][1] = "";
	wheelStrings[charHash('C')][0] = "UQNTLSZFMREHDPXKIBVYGJCWOA";		// Enigma A,B IIIC
	wheelStrings[charHash('C')][1] = "";

	wheelStrings[charHash('D')][0] = "JGDQOXUSCAMIFRVTPNEWKBLZYH";		// German Railway I
	wheelStrings[charHash('D')][1] = "N";
	wheelStrings[charHash('E')][0] = "NTZPSFBOKMWRCJDIVLAEYUXHGQ";		// German Railway II
	wheelStrings[charHash('E')][1] = "E";
	wheelStrings[charHash('F')][0] = "JVIUBHTCDYAKEQZPOSGXNRMWFL";		// German Railway III
	wheelStrings[charHash('F')][1] = "Y";

	wheelStrings[charHash('G')][0] = "LPGSZMHAEOQKVXRFYBUTNICJDW";		// Enigma D,K I
	wheelStrings[charHash('G')][1] = "Y";
	wheelStrings[charHash('H')][0] = "SLVGBTFXJQOHEWIRZYAMKPCNDU";		// Enigma D,K II
	wheelStrings[charHash('H')][1] = "E";
	wheelStrings[charHash('I')][0] = "CJGDPSHKTURAWZXFMYNQOBVLIE";		// Enigma D,K III
	wheelStrings[charHash('I')][1] = "N";

	wheelStrings[charHash('J')][0] = "KPTYUELOCVGRFQDANJMBSWHZXI";		// Enigma T I
	wheelStrings[charHash('J')][1] = "WZEKQ";
	wheelStrings[charHash('K')][0] = "UPHZLWEQMTDJXCAKSOIGVBYFNR";		// Enigma T II
	wheelStrings[charHash('K')][1] = "WZFLR";
	wheelStrings[charHash('L')][0] = "QUDLYRFEKONVZAXWHMGPJBSICT";		// Enigma T III
	wheelStrings[charHash('L')][1] = "WZEKQ";
	wheelStrings[charHash('M')][0] = "CIWTBKXNRESPFLYDAGVHQUOJZM";		// Enigma T IV
	wheelStrings[charHash('M')][1] = "WZFLR";
	wheelStrings[charHash('N')][0] = "UAXGISNJBVERDYLFZWTPCKOHMQ";		// Enigma T V
	wheelStrings[charHash('N')][1] = "YCFKR";
	wheelStrings[charHash('O')][0] = "XFUZGALVHCNYSEWQTDMRBKPIOJ";		// Enigma T VI
	wheelStrings[charHash('O')][1] = "XEIMQ";
	wheelStrings[charHash('P')][0] = "BJVFTXPLNAYOZIKWGDQERUCHSM";		// Enigma T VII
	wheelStrings[charHash('P')][1] = "YCFKR";
	wheelStrings[charHash('Q')][0] = "YMTPNZHWKODAJXELUQVGCBISFR";		// Enigma T VIII
	wheelStrings[charHash('Q')][1] = "XEIMQ";

	wheelStrings[charHash('R')][0] = "EKMFLGDQVZNTOWYHXUSPAIBRCJ";		// Enigma M1,M3,M4 I
	wheelStrings[charHash('R')][1] = "Q";
	wheelStrings[charHash('S')][0] = "AJDKSIRUXBLHWTMCQGZNPYFVOE";		// Enigma M1,M3,M4 II
	wheelStrings[charHash('S')][1] = "E";
	wheelStrings[charHash('T')][0] = "BDFHJLCPRTXVZNYEIWGAKMUSQO";		// Enigma M1,M3,M4 III
	wheelStrings[charHash('T')][1] = "V";
	wheelStrings[charHash('U')][0] = "ESOVPZJAYQUIRHXLNFTGKDCMWB";		// Enigma M1,M3,M4 IV
	wheelStrings[charHash('U')][1] = "J";
	wheelStrings[charHash('V')][0] = "VZBRGITYUPSDNHLXAWMJQOFECK";		// Enigma M1,M3,M4 V
	wheelStrings[charHash('V')][1] = "Z";
	wheelStrings[charHash('W')][0] = "JPGVOUMFYQBENHZRDKASXLICTW";		// Enigma M1,M3,M4 VI 
	wheelStrings[charHash('W')][1] = "ZM";
	wheelStrings[charHash('X')][0] = "NZJHGRCXMYSWBOUFAIVLPEKQDT";		// Enigma M1,M3,M4 VII
	wheelStrings[charHash('X')][1] = "ZM";
	wheelStrings[charHash('Y')][0] = "FKQHTLXOCBJSPDZRAMEWNIUYGV";		// Enigma M1,M3,M4 VIII
	wheelStrings[charHash('Y')][1] = "ZM";
	wheelStrings[charHash('Z')][0] = "LEYJVCNIXWPBQMDRTAKZGFUHOS";		// M4 Greek Rotor "b" (beta)
	wheelStrings[charHash('Z')][1] = "";
	wheelStrings[charHash('1')][0] = "FSOKANUERHMBTIYCWLQPZXVGJD";		// M4 Greek Rotor "g" (gamma)
	wheelStrings[charHash('1')][1] = "";


	reflectors[charHash('A')] = "ABCDEFGHIJKLMNOPQRSTUVWXYZ";  // Enigma A,B did not have reflectors so there was translation
	reflectors[charHash('B')] = "QYHOGNECVPUZTFDJAXWMKISRBL";  // German Railway Reflector
	reflectors[charHash('C')] = "IMETCGFRAYSQBZXWLHKDVUPOJN";  // Swiss K Reflector
	reflectors[charHash('D')] = "LEYJVCNIXWPBQMDRTAKZGFUHOS";  // M4 Beta Rotor
	reflectors[charHash('E')] = "FSOKANUERHMBTIYCWLQPZXVGJD";  // M4 Gamma Rotor
	reflectors[charHash('F')] = "EJMZALYXVBWFCRQUONTSPIKHGD";  // M1,M3,M4 Reflector A
	reflectors[charHash('G')] = "YRUHQSLDPXNGOKMIEBFZCWVJAT";  // M1,M3,M4 Reflector B
	reflectors[charHash('H')] = "FVPJIAOYEDRZXWGCTKUQSBNMHL";  // M1,M3,M4 Reflector C
	reflectors[charHash('I')] = "ENKQAUYWJICOPBLMDXZVFTHRGS";  // M4 R1 (M3 + Thin)
	reflectors[charHash('J')] = "RDOBJNTKVEHMLFCWZAXGYIPSUQ";  // M4 R1 (M3 + Thin)
	

	
	entryDiscs[charHash('A')] = "ABCDEFGHIJKLMNOPQRSTUVWXYZ";  // standard no translation
	entryDiscs[charHash('B')] = "QWERTZUIOASDFGHJKPYXCVBNML";  // Qwerty based translation
	entryDiscs[charHash('C')] = "KZROUQHYAIGBLWVSTDXFPNMCJE";  // Enigma T-specific entry disc

};	