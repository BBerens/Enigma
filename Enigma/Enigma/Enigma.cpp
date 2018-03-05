#include "Enigma.h"
#include <iostream> // only present for debugging

Enigma::Enigma(char *rotorStr, char *ringStr, char *keyStr, char *plugStr)
{
	numRotors = strlen(rotorStr) - 1; // one of the chars is for the reflector
	for (int i = 0; i < numRotors; i++)
	{
		int rotorNumber = rotorStr[numRotors - i - 1] - '1';
		rotors[i] = new Rotor(wheelStrings[rotorNumber][0], wheelStrings[rotorNumber][1]);
		rotors[i]->setRing(ringStr[numRotors - i - 1]);
 		rotors[i]->setFirstChar(keyStr[numRotors - i - 1]);		
	}
	reflector = new Reflector(rotorStr[strlen(rotorStr)-1] - '1');

	plugboard = new Plugboard(plugStr);

}

string Enigma::Encrypt(string phrase)
{
	string output = "";
	char tempChar;
	for (int i = 0; i < phrase.length(); i++)
	{
		if (isalpha(phrase[i]))
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

	
	return character;
}

void Enigma::Rotate(int rotorNum)
{
	rotors[rotorNum]->rotate();
	if (rotors[rotorNum]->getNotches().find(rotors[rotorNum]->GetWindowChar()) != -1)
		Rotate(rotorNum + 1);
}

Rotor::Rotor(string charMap, string notches)
{
	setRingChars(charMap);
	setNotches(notches);
	ringOffset = 0;
	rotCount = 0;
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

char Rotor::map(char inChar)
{
	char outChar = charList->getOutput(inChar);
	outChar = 'A' + (outChar  - 'A' +26 - ringOffset%26) % 26;
	return outChar;
}

char Rotor::ReverseMap(char inChar)
{
	char outChar = reverseList->getOutput(inChar);
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

	// not complete probably dont need it
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

Node::Node(char indexChar)
{
	index = indexChar;
	notched = false;
}

Node::~Node()
{
	;
}

Plugboard::Plugboard(char *letterPairs)
{
	SetPlugs(letterPairs);
}

void Plugboard::SetPlugs(char *letterPairs)
{
	for (int i = 0; i < 26; i++)
		dictionary[i] = 'A' + i;
	for (int i = 0; i < strlen(letterPairs); i += 2)
	{
		dictionary[letterPairs[i] - 'A'] = letterPairs[i + 1];
		dictionary[letterPairs[i + 1] - 'A'] = letterPairs[i];
	}
}

char Plugboard::Translate(char inChar)
{
	return dictionary[inChar - 'A'];
}

Reflector::Reflector(int reflectorNumber)
 {
	charList = new CircularList(reflectors[reflectorNumber]);
}

char Reflector::map(char inChar)
{
	return charList->getOutput(inChar);
}