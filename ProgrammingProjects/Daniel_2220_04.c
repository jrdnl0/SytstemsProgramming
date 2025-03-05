/*

Name: Jacob Daniel 

CUID: jrdnl 

Course: ECE2220

Semester: Spring 2025 

Assignment Number: Project #4 


Purpose:

A very famous cipher / method of encryption is the vigenere cipher, a form of code-forming where a message is shifted by some value 
determined by a repeated codephrase. Although not used in modern cryptography, it reamains a very common teaching tool for cryptography, and is
recorded being used as early as the late 13th century.

This program allows the user to input a command that performs the encryption or decryption process of the cipher between two phrases -- the first
phrase being the either encrypted or decrypted message, and the second phrase being the encoding, or "key" phrase of the message.


Assumptions:

We assume that the command-line argument entered by the user will have three main specifiers: 

1) An indicator whether the program shall encrypt or decrypt the program. 
2) A string encapsulated in double quotes that represents the message phrase. 
3) A string encapsulated in double quotes that represents the key/encoding phrase. 

ex 1) >> encode "Beat the Shamecocks!" "Go tigers!" --> HSTBZLVKNOFMISTCY
ex 2) >> decode "HSTBZLVKNOFMISTCY" "Go tigers!" --> BEATTHESHAMECOCKS

Note that the command(s) above are NOT case specific and are not sensitive to whitespace, but ARE order-specific
(i.e. entering >> "Beat the shamecocks" encode "Go tigers!" is NOT valid).



Known Bugs: 

There are no known bugs, besides that the message(s) will simply be truncated / shortened to 32 characters either of the inputs are greater than
that maximum length. 


*/

#include <stdio.h>
#include <stdlib.h>
#include <string.h>

#define TRUE  1
#define FALSE 0
#define MAX_SIZE 32


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


char* normalizeCode(char *userString, int desiredLength)
/*

PURPOSE
This function "normalizes" the encoding string to be the same length of the message string (if need be).
Looking at the project description and the example of the message "Beat the Shamecocks!" the encoding phrase
"Go, Tigers!", it seems that (if the coding phrase is shorter) is "normalized" to be the same length as the
message phrase, (e.g. "Go, tigers!" becomes "GOTIGERSGOTIGERSG". This is essentially what this function
performs, and what I mean what I say "normalized", too.

Additionally, I also designed the function so that if the encoding phrase is longer than the message phrase,
the encoding phrase will be shortened to perserve the nature of the cipher. (This applies to the case
where the encoding and message strings are the same length).


ARGUMENTS
char *userString -> This is the original message string (input as an argument of the function)
int desiredLength -> This is the length of the string that it is to be extended to.

RETURNS
char* to the "updated" version of the user's encoding phrase (recitified to be the same length as the message) --
this way, we can match each character of the message (both encoded and decoded) to the coding phrase by index
and adjust them to their correct corresponding position.

*/
{



    int messageIndex   = 0;
    int messageLength  = 0;
    int originalLength = strlen(userString);


    char* newString = (char*)(malloc(sizeof(char) * MAX_SIZE));

    if(desiredLength > MAX_SIZE)
    {
        desiredLength = MAX_SIZE;
    }



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

        messageIndex++;
    }

    return newString;
}


char encodeCharacter(char messageCharacter, char codingCharacter)
/*

PURPOSE
This is the fundamental fucntion for encoding the user message based on the coding phraee
specified by the user. It shifts the character based on the lexicographical "difference" between
the two respective characters. (I won't explain any further -- I'll assume you know the rest!)


ARGUMENTS
messageCharacter -> The indexed character from the message phrase.

codingCharcter   -> The indexed character from the coding phrase -- determines the lexicographical shift of the
messageCharacter variable.


RETURNS
char -> The shifted message character by order of the coding phrase's respective character as described
above.


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
This function converts two strings: one representing the original message specified by the user, and the
second representing the coding phrase, into the encoded version of the message string.

ARGUMENTS
char* userMessage   -> "String" representing the original message specified by the user to be encoded

char* codingMessage -> "String" representing the coding phrase that will lexicologically shift the user's message
accorindly (This is explained plenty above throughout the program so I won't tire it).


RETURNS
char* -> "String" representing the encoded version of the original message string -- by order/specification
by the (equally user-specified) encoding string.


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
This function serves as the basis for the decoding process for the strings. We simply "unshift' characters
and check for "wrap-arounds" (e.g. if an encoded A gets deshifted by 1 character it becomes a "Z").
These characters are then put into a string that is put into a s

ARGUMENTS
char encodedCharacter -> The corresponding character of the encoded message to be "unshifted" into the original code.

char codingCharacter  -> The corresponding character of the coding phrase that determines the value that the
encded character is shifted by lexicologically.



RETURNS
Returns the char value that represents the "unshifted" value of encodedCharacter by order of codingCharacter

*/
{

	char decodedChar = (encodedCharacter - codingCharacter) + 0x41;

	if(decodedChar < 0x41)
	{
		decodedChar += 0x1A;

	}

	return decodedChar;
}


char *decodeMessage(char *userEncoded, char *codingMessage)
/*

PURPOSE
This function takes two strings -- one of them representing an encoded message, and the other
representing the "encoding phrase" that corresponds to the lexicological shifting of the respective
characters to create the encoded phrase.


ARGUMENTS
char *userEncoded 	-> The encoded message string that is to be decoded by this function

char *codingMessgae -> The coding phrase/string that is to be used in order to determine the lexicological
order to "unshift" each character in the *userEncoded string


RETURNS
This function returns a char* to the decoded string based on the two arguments of the function
described above.

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

    if(sizeString > MAX_SIZE)
    {
        sizeString = MAX_SIZE;
    }


    int charCounter = 0;

    char *newString = (char*)(malloc(sizeof(char) * MAX_SIZE));


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


    }


    return newString;

}







int main(int argc, char **argv)
{




    char* argumentOne = strippedCapitalized(argv[1]);

    // Validating that the user's input is valid before running the rest of the program

    int checkOne = ((strcmp(argumentOne, "ENCODE") == 0) || (strcmp(argumentOne, "DECODE") == 0)) ? 1 : 0 ;
    int checkTwo = (argc == 4) ? 1 : 0 ;


    if(checkOne && checkTwo)
    {

        
        char* argumentTwo = strippedCapitalized(argv[2]);
        char* argumentThree = normalizeCode(argv[3], (int)(strlen(argumentTwo)));
        char* output;


        if(strcmp(argumentOne, "ENCODE") == 0)
        {
            
            output = encodeMessage(argumentTwo, argumentThree);
            printf("The encoded message is: %s\n", output);
        }
        else
        {
            output = decodeMessage(argumentTwo, argumentThree);
            printf("The decoded message is: %s\n", output);
        }

        return EXIT_SUCCESS;
    }
    else
    {

        printf("INVALID COMMAND! Please enter a command such as: \n");
        printf(">> encode \"Beat the Shamecocks!\" \"Go Tigers\"\n");
        return EXIT_FAILURE;
    }


}	


