#include "Enigma.h"

Enigma::Enigma(string discStr, string rotorStr, string ringStr, string keyStr, string plugStr)
{
	initializeStrings();
	string discChars = entryDiscs[charHash(discStr[0])];
	entryDisc = new EntryDisc(discChars);
	plugboard = new Plugboard(plugStr);
	numRotors = rotorStr.length() - 1;
	for (int i = 0; i < numRotors; i++)
	{
		char wheelChar = rotorStr[numRotors - i - 1];
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
		character = rotors[i]->map(character);

	character = reflector->map(character);
	for (int i = numRotors - 1; i >= 0; i--)
		character = rotors[i]->ReverseMap(character);

	character = plugboard->Translate(character);
	character = entryDisc->ReverseMap(character);
	
	return character;
}

void Enigma::Rotate(int rotorNum)
{
	if (rotors[rotorNum]->getNotches().find(rotors[rotorNum]->GetWindowChar()) != -1 && rotorNum < numRotors - 1)
		Rotate(rotorNum + 1);	
	rotors[rotorNum]->rotate();
}

Rotor::Rotor(string rotorChars)
{
	charList = new CircularList(rotorChars);
	ringOffset = 0;
	rotCount = 0;
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
	ringOffset = STARTCHAR - lockedChar;

	for (int i = 0; i < ringOffset + ALPHABETSIZE; i++)
	{
		charList->tail = charList->head;
		charList->head = charList->head->next;
	}
}

void Rotor::setFirstChar(char startChar)
{
	int offset = startChar - STARTCHAR;

	for (int i = 0; i < offset; i++)
	{
		rotate();
	}
}

void Rotor::setNotches(string notches) 
{
	notchedChars = notches;
}

//adds or subtracts rotCount from given char, pass 1 to add, -1 to subtract;
char Rotor::adjust(char inChar, int sign)
{
	char outChar = inChar - STARTCHAR;
	outChar = (outChar + ringOffset * sign) % ALPHABETSIZE;
	if (outChar < 0)
		outChar += ALPHABETSIZE;
	return outChar + STARTCHAR;
}

char Rotor::map(char inChar)
{
	char outChar = charList->getOutput(inChar);
	outChar = adjust(outChar, -1);
	return outChar;
}

char Rotor::ReverseMap(char inChar)
{
	char outChar = adjust(inChar, 1);
	outChar = charList->getReverseOutput(outChar);
	return outChar;
}

void Rotor::rotate()
{
	charList->tail = charList->head;
	charList->head = charList->head->next;
	ringOffset++;
	rotCount++;
}

CircularList::CircularList(string charMap)
{
	count = 0;
	for (int i = 0; i < charMap.length(); i++)
		this->insertNode(charMap[i]);
}

void CircularList::insertNode(char mapChar)
{
	Node *temp = new Node(mapChar);
	if (!head)
	{
		head = temp;
		head->next = head;
		tail = head;
	}
	else
	{
		tail->next = temp;
		tail = temp;
		temp->next = head;
	}
	count++;
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

int Enigma::charHash(char inChar) { return (int)inChar; }

void Enigma::initializeStrings(void)
{
	//real rotors, alphabetSize = 26, startChar = 'A'
	// Wheel Strings
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

	// Reflectors
	reflectors[charHash('A')] = "ABCDEFGHIJKLMNOPQRSTUVWXYZ";  // Enigma A,B did not have reflectors so there was no translation
	reflectors[charHash('B')] = "QYHOGNECVPUZTFDJAXWMKISRBL";  // German Railway Reflector
	reflectors[charHash('C')] = "IMETCGFRAYSQBZXWLHKDVUPOJN";  // Swiss K Reflector
	reflectors[charHash('D')] = "EJMZALYXVBWFCRQUONTSPIKHGD";  // M1,M3,M4 Reflector A
	reflectors[charHash('E')] = "YRUHQSLDPXNGOKMIEBFZCWVJAT";  // M1,M3,M4 Reflector B
	reflectors[charHash('F')] = "FVPJIAOYEDRZXWGCTKUQSBNMHL";  // M1,M3,M4 Reflector C
	reflectors[charHash('G')] = "ENKQAUYWJICOPBLMDXZVFTHRGS";  // M4 R1 (M3 + Thin)
	reflectors[charHash('H')] = "RDOBJNTKVEHMLFCWZAXGYIPSUQ";  // M4 R1 (M3 + Thin)
	
	// Entry Discs
	entryDiscs[charHash('A')] = "ABCDEFGHIJKLMNOPQRSTUVWXYZ";  // standard no translation
	entryDiscs[charHash('B')] = "QWERTZUIOASDFGHJKPYXCVBNML";  // Qwerty based translation
	entryDiscs[charHash('C')] = "KZROUQHYAIGBLWVSTDXFPNMCJE";  // Enigma T-specific entry disc
};	