#include <stdio.h>
#include <stdlib.h>
#include <string.h>

#define TRUE  1
#define FALSE 0
#define MAX_SIZE 32

typedef char* commandArg;


int isAlphabetic(char checkingChar)
/*


PURPOSE

This function returns an integer that represents whether or not a character is an letter / alphabetic, and also whether
the character is uppercase or lowercase



ARGUMENTS

char checkingChar -> The character being checked




RETURNS

The integer returned has three possible variations with these corresponding meanings:

0 -> The character is non-alphabetic
1 -> The character is an uppercase letter
2 -> The character is a lowercase letter


*/
{


    int numericCheck = (int)(checkingChar);

    if((numericCheck >= 0x41) && (numericCheck <= 0x5A))
    {
        return 0x01;
    }

    else if((numericCheck >= 0x61) && (numericCheck <= 0x7A))
    {

        return 0x02;
    }


    return 0x00;
}


int countAlphabetic(char *userString, int sizeString)
/*

PURPOSE
This function counts the number of alphabetic characters (i.e. letters of the alphabet) in a specified string

ARGUMENTS

char *userString -> Pointer to the user string to be analyzed by the function
int  sizeString  -> Integer that represents the size of the string so we can safely iterate


RETURNS
Returns an integer that represents the number of alphabetic characters (uppercase and lowercase) found within the string



*/
{

    int counterVar = 0;
    int ii;


    for(ii=0;ii<sizeString;ii++)
    {
        int checkNumeric = isAlphabetic(userString[ii]);

        if((checkNumeric == 0x01) || (checkNumeric == 0x02))
        {
            counterVar ++ ;
        }

    }


    return counterVar;
}


char *strippedCapitalized(char *userString)
/*

PURPOSE
This function returns a whitespace / nonalphabetic and fully capitalized verison of a string input by the user. When I wrote this I was thinking of just replicating
doing str.strip().upper() in Python, as that's what the example code showed (when it comes to "rectifiying" the original inputs to be encoded).

ARGUMENTS
*userString -> A pointer to the original string to be stripped of whitespace and capitalized by the user.


RETURNS
*newString -> A pointer to the stripped, capitalized version of the argument/parameter string.




*/
{

    int ii;
    int sizeString = strlen(userString) + 1;

    int charCounter = 0;
    int memoryCounter = 1;

    char *newString = (char*)(malloc(sizeof(char) * memoryCounter));


    for(ii=0;ii<sizeString;ii++)
    {

        int checkAlphabet = isAlphabetic(userString[ii]);

        if(checkAlphabet == 0x01)
        {

            newString[charCounter] = userString[ii];
            charCounter++;

        }

        else if(checkAlphabet == 0x02)
        {

            newString[charCounter] = (userString[ii] - 0x20);
            charCounter++;
        }

        if(charCounter >= memoryCounter)
        {

            if(realloc(newString, 1));
            // Technically if you don't operate on / "recognize" the void pointer return in some form it throws a warning -- #dealwithit

            memoryCounter ++;

        }

    }


    return newString;


}


char *normalizeCode(char *userString, int desiredLength)
/*

PURPOSE

This function "normalizes" a code to the 


ARGUMENTS


RETURNS



*/

{



    int messageIndex   = 0;
    int messageLength  = 0;
    int originalLength = strlen(userString);

    int memoryCounter = 1;
    char* newString = (char*)(malloc(sizeof(char) * memoryCounter));


    while(messageLength < desiredLength)
    {


        messageIndex = (messageIndex > originalLength) ? 0 : messageIndex;

        int alphabetTest = isAlphabetic(userString[messageIndex]);

        if(alphabetTest == 0x01)
        {
            newString[messageLength] = userString[messageIndex];
            messageLength++ ;
        }
        else if(alphabetTest == 0x02)
        {

            newString[messageLength] = userString[messageIndex] - 0x20;
            messageLength++;
        }


        if(messageLength >= memoryCounter)
        {

            if(realloc(newString, 1));
            memoryCounter++;


        }

        messageIndex++;
    }


    newString[memoryCounter] = 0x00;

    return newString;
}


char encodeCharacter(char messageCharacter, char codingCharacter)
/*

PURPOSE
This function simply encodes the character with the correct cipher protocol, i.e. shifts the 

ARGUMENTS


RETURNS



*/
{

	char encodedChar = (messageCharacter + codingCharacter) - 0x41;
	if(encodedChar >= 0x5B)
	{
		encodedChar = (messageCharacter + codingCharacter) - 0x5B;
	}

	return encodedChar;

}


char *encodeMessage(char* userMessage, char* codingMessage)
/*

PURPOSE

ARGUMENTS

RETURNS


*/
{

    int stringSize = strlen(userMessage);
    char *encodedMessage = (char*)(malloc(sizeof(char) * stringSize));

    for(int ii=0; ii<stringSize; ii++)
    {

	encodedMessage[ii] = encodeCharacter(userMessage[ii], codingMessage[ii]);

    }


    return encodedMessage;

}


char decodeCharacter(char encodedCharacter, char codingCharacter)
/*

PURPOSE

ARGUMENTS

RETURNS


*/
{

	char decodedChar = (encodedCharacter - codingCharacter) + 0x41;

	if(decodedChar < 0x41)
	{
		decodedChar += 0x1A ;

	}

	return decodedChar;
}


char *decodeMessage(char *userEncoded, char *codingMessage)
/*

PURPOSE

ARGUMENTS

RETURNS


*/
{

	int stringSize = strlen(userEncoded);
	char *decodedMessage = (char*)(malloc(sizeof(char) * stringSize));

	for(int ii=0; ii<stringSize; ii++)
	{
		decodedMessage[ii] = decodeCharacter(userEncoded[ii], codingMessage[ii]);
	}


	return decodedMessage;

}


char *fetchSubstring(const char *wholeMessage, int startIndex, int endIndex)
/*

PURPOSE


ARGUMENTS



RETURNS


*/
{

    int stringLength = (endIndex - startIndex) ;

    char *newString = (char*)(malloc(sizeof(char) * (stringLength + 1)));

    for(int ii=startIndex; ii<(endIndex+1); ii++)
    {
        int jj = (ii - startIndex);
        newString[jj] = wholeMessage[ii];
    }



    newString[stringLength+1] = '\0';

    return newString;

}



commandArg fetchArgument(char* argv_i)
{

	commandArg argumentFound = (commandArg)(malloc(sizeof(char) * MAX_SIZE));
	int arg_idx = 0;

	while(argv_i[arg_idx] != 0x00)
	{

		if(arg_idx == 31)
		{
			return argumentFound;
		}
		argumentFound[arg_idx] = argv_i[arg_idx];
		arg_idx++;
	}


	return argumentFound;

}

int main(int agrc, char **argv)
{


	commandArg argumentOne = strippedCapitalized(fetchArgument(argv[1]));
	commandArg argumentMessage = strippedCapitalized((argv[2]));
	commandArg argumentCoding  = normalizeCode(strippedCapitalized(fetchArgument(argv[3])), (int)(strlen(argumentMessage)));

	int encodingCheck = strcmp(argumentOne, "ENCODE");


	if(encodingCheck == 0)
	{

		commandArg output = encodeMessage(argumentMessage, argumentCoding);
		printf("%s \n", output);

	}
	else
	{

		commandArg output = decodeMessage(argumentMessage, argumentCoding);
		printf("%s \n", output);


	}

	
	return EXIT_SUCCESS;
}	
