#include "Enigma.h"
#include <iostream> // only present for debugging

Enigma::Enigma()
{
	rotors[0] = new Rotor("ABCDEFGHIJKLMNOPQRSTUVWXYZ", 'A', 'A');
	rotors[1] = new Rotor("ABCDEFGHIJKLMNOPQRSTUVWXYZ", 'A', 'A');
	rotors[2] = new Rotor("ABCDEFGHIJKLMNOPQRSTUVWXYZ", 'A', 'A');

}

bool Enigma::SetConfig(string input)
{
	size_t start = 0;
	size_t end = input.find(' ', start + 1);
	string token = input.substr(start, end - start);

	// First set of characters specify which rotors in order Left, Middle, Right rotors
	for (int i = 0; i < token.length(); i++)
	{
		SetRotor(token.length() - 1 - i, token[i]);
	}

	// Next 3 characters specify the ring position in same order
	start = end + 1;
	end = input.find(' ', start + 1);
	token = input.substr(start, end - start);
	for (int i = 0; i < token.length(); i++)
	{
		SetRing(token.length() - 1 - i, token[i]);
	}

	// Next 3 characters specify the character first visible
	start = end + 1;
	end = input.find(' ', start + 1);
	token = input.substr(start, end - start);
	for (int i = 0; i < token.length(); i++)
	{
		SetChar(token.length() - 1 - i, token[i]);
	}

	// Next
	start = end + 1;
	end = input.length();
	token = input.substr(start, end - start);

	plugboard.SetPlugs(token);

	return true;
}

void Enigma::SetRotor(int rotorPos, char rotorNum)
{
	rotors[rotorPos]->setRingChars(wheelStrings[rotorNum - '1'][0]);
	rotors[rotorPos]->setNotches(wheelStrings[rotorNum - '1'][1]);
}

void Enigma::SetRing(int rotorPos, char notchedChar)
{
	rotors[rotorPos]->setRing(notchedChar);
}

void Enigma::SetChar(int rotorPos, char visibleChar)
{
	rotors[rotorPos]->setFirstChar(visibleChar);
}

string Enigma::Encrypt(string phrase)
{
	string output = "";
	char tempChar;
	for (int i = 0; i < phrase.length(); i++)
	{
		tempChar = EncryptChar(phrase[i]);
		output.append(1,tempChar);
	}

	return output;
}

char Enigma::EncryptChar(char character)
{
	rotors[0]->rotate();
	if (rotors[0]->getNotches().find(rotors[0]->GetWindowChar()) != -1)
	{
		rotors[1]->rotate();
		if (rotors[1]->getNotches().find(rotors[1]->GetWindowChar()) != -1)
		{
			rotors[2]->rotate();
		}
	}
	character = plugboard.Translate(character);
	character = rotors[0]->map(character);
	character = rotors[1]->map(character);
	character = rotors[2]->map(character);
	character = reflector.map(character);
	character = rotors[2]->ReverseMap(character);
	character = rotors[1]->ReverseMap(character);
	character = rotors[0]->ReverseMap(character);
	character = plugboard.Translate(character);

	
	return character;
}

char Enigma::DebugEncryptChar(char character)
{
	rotors[0]->rotate();
	cout << "Rotate rotor 1" << endl;

	if (rotors[0]->getNotches().find(rotors[0]->GetWindowChar()) != -1)
	{
		rotors[1]->rotate();
		cout << "Rotate rotor 2" << endl;
		if (rotors[1]->getNotches().find(rotors[1]->GetWindowChar()) != -1)
		{
			rotors[2]->rotate();
			cout << "Rotate rotor 3" << endl;
		}
	}
	cout << "Into plugboard: " << character << endl;
	character = plugboard.Translate(character);
	cout << "Into rotor1: " << character << endl;
	character = rotors[0]->map(character);
	cout << "Into rotor2: " << character << endl;
	character = rotors[1]->map(character);
	cout << "Into rotor3: " << character << endl;
	character = rotors[2]->map(character);
	cout << "Into reflector: " << character << endl;
	character = reflector.map(character);
	cout << "Into rotor3: " << character << endl;
	character = rotors[2]->ReverseMap(character);
	cout << "Into rotor2: " << character << endl;
	character = rotors[1]->ReverseMap(character);
	cout << "Into rotor1: " << character << endl;
	character = rotors[0]->ReverseMap(character);
	cout << "Into plugboard: " << character << endl;
	character = plugboard.Translate(character);
	cout << "Out: " << character << endl;


	return character;
}

Rotor::Rotor()
{
	charList = NULL;
	ringOffset = 0;
	rotCount = 0;
	setNotches("");
	//setFirstChar();
	
}
Rotor::Rotor(string charMap, char notchedChar, char startChar)
{
	charList = new CircularList(charMap);
	ringOffset = 0;
	rotCount = 0;
	setNotches("");
	setFirstChar(startChar);
	
}

Rotor::~Rotor()
{
	;
}

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

void Rotor::setNotches(string notchChars)
{
	notchedChars = notchChars;
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

	//Node *lockedNode = charList->findChar(lockedChar);
	//charList->head = lockedNode;
}

void Rotor::setFirstChar(char startChar)
{
	//int offset = startChar - 'A';
	int offset = startChar - 'A';

	for (int i = 0; i < offset; i++)
	{
		charList->head = charList->head->next;
		reverseList->head = reverseList->head->next;
		ringOffset++;
		rotCount++;
	}
}

char Rotor::map(char inChar)
{
	char outChar = charList->getOutput(inChar);
	outChar = 'A' + (outChar  - 'A' +26 - ringOffset%26) % 26;
	return outChar;
}

char Rotor::ReverseMap(char inChar)
{
	char outChar = reverseList->getReverseOutput(inChar);
	outChar = 'A' + (outChar - 'A' + 26 - ringOffset%26) % 26;
	return outChar;
}

void Rotor::rotate()
{
	charList->head = charList->head->next;
	reverseList->head = reverseList->head->next;
	ringOffset++;
	rotCount++;
}

	// not complete
void Rotor::ChangeRotor(int rotorNum)
{
	if (charList != NULL)
	{
		delete [] charList;
	}

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
	int index = inChar - 'A';
	for (int i = 0; i < index; i++)
	{
		curNode = curNode->next;
	}
	return curNode->index;
}

// Need to remove this. Exactly the same as getOutput. Pick a better name.
char CircularList::getReverseOutput(char inChar)
{
	Node *curNode = head;
	int index = inChar - 'A';
	for (int i = 0; i < index; i++)
	{
		curNode = curNode->next;
	}
	return curNode->index;
}

Node::Node(char indexChar)
{
	index = indexChar;
	notched = false;
}

Node::~Node()
{
	;
}

Plugboard::Plugboard()
{
	for (int i = 0; i < 26; i++)
		dictionary[i] = 'A' + i;
}
Plugboard::Plugboard(string letterPairs)
{
	SetPlugs(letterPairs);
}

void Plugboard::SetPlugs(string letterPairs)
{
	for (int i = 0; i < 26; i++)
		dictionary[i] = 'A' + i;
	for (int i = 0; i < letterPairs.length(); i += 2)
	{
		dictionary[letterPairs[i] - 'A'] = letterPairs[i + 1];
		dictionary[letterPairs[i + 1] - 'A'] = letterPairs[i];
	}
}
char Plugboard::Translate(char inChar)
{
	return dictionary[inChar - 'A'];
}

Reflector::Reflector()
{
	//temporary assigned reflector b for testing
	charList = new CircularList(reflectors[1]);
}

Reflector::Reflector(int reflectorNumber)
{
	charList = new CircularList(reflectors[reflectorNumber]);
}

char Reflector::map(char inChar)
{
	return charList->getOutput(inChar);
}