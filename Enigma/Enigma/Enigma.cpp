#include "Enigma.h"
#include <iostream> // only present for debugging

Enigma::Enigma(char *discStr, char *rotorStr, char *ringStr, char *keyStr, char *plugStr)
{
	entryDisc = new EntryDisc(discStr);
	numRotors = strlen(rotorStr) - 1; // one of the chars is for the reflector
	for (int i = 0; i < numRotors; i++)
	{
		int rotorNumber = rotorStr[numRotors - i - 1] - '0';
		rotors[i] = new Rotor(wheelStrings[rotorNumber][0]);
		rotors[i]->setRing(ringStr[numRotors - i - 1]);
		rotors[i]->setNotches(wheelStrings[rotorNumber][1]);
		rotors[i]->setFirstChar(keyStr[numRotors - i - 1]);

	}
	reflector = new Reflector(rotorStr[strlen(rotorStr)-1] - '0');

}

string Enigma::Encrypt(string phrase)
{
	string output = "";
	char tempChar;
	for (int i = 0; i < phrase.length(); i++)
	{
		if (startChar <= phrase[i]) //&& phrase[i] < (alphabetSize + startChar))
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
	if (rotors[rotorNum]->getNotches() && rotorNum < numRotors - 1)
		Rotate(rotorNum + 1);	
	rotors[rotorNum]->rotate();
}

Rotor::Rotor(string charMap)
{
	charList = new CircularList(charMap);
	rotCount = 0;
}

Rotor::~Rotor()
{
	;
}

/* seemed redundant
void Rotor::setRingChars(string ringChars)
{
	// memory leak here. Need to remove charList
	/*if (charList != NULL)
	{
		delete[] charList;
	}
	rotCount = 0;

}
*/

bool Rotor::getNotches()
{
	return charList->head->notched;
}

void Rotor::setRing(char lockedChar)
{
	rotCount = lockedChar - startChar;	
}

void Rotor::setFirstChar(char startChar)
{

	int offset = adjust(startChar, -1) - startChar;

	for (int i = 0; i < offset; i++)
	{
		rotate();
	}

	rotCount = offset;
}

void Rotor::setNotches(string notches) 
{
	for (int i = 0; i < notches.length(); i++)
	{
		Node *index = charList->head;
		char position = adjust(notches.at(i), -1) - startChar;
		for (int j = 0; j < position; j++)
			index = index->next;
		index->notched = true;
	}
}

//adds or subtracts rotCount from given char, pass 1 to add, -1 to subtract;
char Rotor::adjust(char inChar, int sign)
{
	char outChar = inChar - startChar;
	outChar = (outChar + rotCount * sign) % alphabetSize;
	if (outChar < 0)
		outChar += alphabetSize;
	return outChar + startChar;
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
	int index = inChar - startChar;
	for (int i = 0; i < index; i++)
	{
		curNode = curNode->next;
	}
	return curNode->index;
}

char CircularList::getReverseOutput(char inChar)
{
	Node *curNode = head;
	char outChar = startChar;
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

Plugboard::Plugboard(char *letterPairs)
{
	SetPlugs(letterPairs);
}

void Plugboard::SetPlugs(char *letterPairs)
{
	for (int i = 0; i < alphabetSize; i++)
		dictionary[i] = startChar + i;
	for (int i = 0; i < strlen(letterPairs); i += 2)
	{
		dictionary[letterPairs[i] - startChar] = letterPairs[i + 1];
		dictionary[letterPairs[i + 1] - startChar] = letterPairs[i];
	}
}

char Plugboard::Translate(char inChar)
{
	return dictionary[inChar - startChar];
}

Reflector::Reflector(int reflectorNumber)
 {
	charList = new CircularList(reflectors[reflectorNumber]);
}

char Reflector::map(char inChar)
{
	return charList->getOutput(inChar);
}

EntryDisc::EntryDisc(char *discStr)
{
	int discNum = discStr[0] - 'A';
	charList = new CircularList(entryDiscs[discNum]);
}

char EntryDisc::map(char inChar)
{
	return charList->getOutput(inChar);
}

char EntryDisc::ReverseMap(char inChar)
{
	return charList->getReverseOutput(inChar);
}