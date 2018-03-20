#include <iostream>
#include <string>
#include "Enigma.h"


using namespace std;

int main(int argc, char *argv[])
{
	/*
	argv[1] = rotors 4 or 5 digits indicating the rotor and order. The last digit indicates the reflector being used.
	argv[2] = ring setting 3 or 4 letters (same number as rotors - 1) 
	argv[3] = message key 3 or 4 letters (same number as rotors - 1)
	argv[4] = plugboard data pairs of letters (no repeats)
	argv[5] = message string to be decrypted (must be surrounded in quotes)
	*/
	;
	if (argc != 7)
	{
		return 1; // need to print usage		
	}

	Enigma *enigma1 = new Enigma(argv[1], argv[2], argv[3], argv[4], argv[5]);
	cout <<  enigma1->Encrypt(argv[6]);
	return 0;
	
}
