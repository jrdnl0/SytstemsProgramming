#include <stdio.h>
#include <stdlib.h>
#include <string.h>

#define TRUE  1 
#define FALSE 0 
#define testMessage "BEATTHESHAMECOCKS"
#define codeMessage "GOTIGERSGOTIGERSG"
#define testEncoded "HSTBZLVKNOFMISTCY"





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




char *strippedCapitalized(char *userString, int sizeString)
{

    int ii;
    int stringCounter = 0;
    int alphabetCount = countAlphabetic(userString, sizeString);
    char *newString = (char*)(malloc(sizeof(char) * alphabetCount));


    for(ii=0;ii<sizeString;ii++)
    {

        if(isAlphabetic(userString[ii]) == 0x02)
        {

            newString[stringCounter] = (userString[ii] - 0x20);
            stringCounter++;
        }

        else if(isAlphabetic(userString[ii]) == 0x01)
        {
            newString[stringCounter] = userString[ii];
            stringCounter++;
        }


    }


    return newString;
    

}



char* normalizeCode(char *userString, int sizeString, int desiredLength)
{



    char* newString = (char*)(malloc(sizeof(char) * desiredLength));
    int messageIndex  = 0;
    int messageLength = 0;



    while(messageLength < (desiredLength))
    {

        if(messageIndex > sizeString)
        {
            messageIndex = 0;
        }

        char currentChar = userString[messageIndex];        

        if((currentChar >= 0x61) && (currentChar <= 0x7A))
        {
            newString[messageLength] = (currentChar - 0x20);
            messageLength ++;
        }

        else if((currentChar >= 0x41) && (currentChar <= 0x5A))
        {
            newString[messageLength] = currentChar;
            messageLength ++;
        }

        
        messageIndex++;


    }


    return newString;

}


char *createMessage(void)
{
    char* userEntry = (char*)(malloc(sizeof(char) * 32)) ;

    printf("Enter message here:\n");
    fgets(userEntry, 32, stdin);


    char *newEntry = strippedCapitalized(userEntry, 32);
    free(userEntry);


    return newEntry;
}


char* createStandardCode(int messageLength)
{

    char* userEntry = (char*)(malloc(sizeof(char) * messageLength)) ;
    printf("Enter code word here:\n");

    fgets(userEntry, messageLength, stdin);

    char *newEntry = strippedCapitalized(userEntry, messageLength);

    free(userEntry);

    char *finishedCode = normalizeCode(newEntry, 32, (messageLength-1));

    free(newEntry);

    return finishedCode;



}


int shiftCharacter(char messageCharacter, char codingCharacter)
{

	int message_integer = (int)(messageCharacter);
	int coding_integer  = (int)(codingCharacter);


	int shiftedIndex = (message_integer + coding_integer) - 0x41;
	if(shiftedIndex >= 0x5B)
	{
		shiftedIndex = (shiftedIndex - 0x5B) + 0x41;
	}


	return shiftedIndex;
}


int unshiftCharacter(char encodedChar, char codingChar)
{


    int encoded_integer = (int)(encodedChar);
    int coding_integer = (int)(codingChar);

    int shiftedIndex = (encoded_integer - coding_integer) + 0x41;


    if(shiftedIndex < 0x41)
    {
        shiftedIndex = (shiftedIndex + 0x5B) - 0x41;
    }


    return shiftedIndex;


}




void shift_whole_message(char *message, char *coding, int size)
{
	int ii;
	for(ii=0;ii<size;ii++)
	{
		char cMess = (char)(message[ii]);
		char cCode = (char)(coding[ii]) ;
		int shift  = shiftCharacter(cMess, cCode);

		printf("%c",shift);

	}

	printf("\n");


	return;
}



void deshift_whole_message(char *encodedMessage, char *codingMessage, int size)
{
    int ii;
    for(ii=0;ii<size;ii++)
    {

        char cEncoded = (char)(encodedMessage[ii]);
        char cCoding  = (char)(codingMessage[ii]);
        int unshifted = unshiftCharacter(cEncoded, cCoding);
        printf("%c", unshifted);
    }



    printf("\n");


    return;


}


char *encodeMessage(char *userMessage, char *userCoding, int messageSize)
{

    int ii;
    char *decodeString = (char*)(malloc(sizeof(char) * messageSize));

    for(ii=0;ii<messageSize;ii++)
    {

        char shiftedChar = shiftCharacter(userMessage[ii], userCoding[ii]);
        decodeString[ii] = shiftedChar;
    }


    return decodeString;
}

char *decodeMessage(char *encodedMessage, char *codingMessage, int messageSize)
{

    int ii;
    char *decodeString = (char*)(malloc(sizeof(char) * (messageSize+1)));

    for(ii=0;ii<messageSize;ii++)
    {
        char unshiftedChar = unshiftCharacter(encodedMessage[ii], codingMessage[ii]);
        decodeString[ii] = unshiftedChar;
    }


    decodeString[messageSize] = '\0';



    return decodeString;


}



int* findMessageIndices(char *wholeCommand, int sizeCommand)
{


    int startOne, endOne, startTwo, endTwo, ii, jj;


    startOne = -1;
    startTwo = -1;
    endOne = -1;
    endTwo = -1;


    

    int *indicesList = (int*)(malloc(sizeof(int) * 4));

    for(ii=0;ii<(sizeCommand-1);ii++)
    {

        jj = ii + 1;


        if((wholeCommand[ii] == 0x20) && (wholeCommand[jj] == 0x27))
        {

            if(startOne == -1)
            {
                startOne = (jj+1);
            }
            else
            {
                startTwo = (jj+1);
            }


        }


        if((wholeCommand[ii] == 0x27) && ((wholeCommand[jj] == 0x20) || (wholeCommand[jj] == 0x00) || (wholeCommand[jj] == 0x0A)))
        {

            if(endOne == -1)
            {
                endOne = (ii-1);
            }
            else
            {
                endTwo = (ii-1);
            }
        }


    }



    indicesList[0] = startOne;
    indicesList[1] = startTwo;
    indicesList[2] = endOne;
    indicesList[3] = endTwo;



    return indicesList ;


}




char *fetchSubstring(const char *wholeMessage, int startIndex, int endIndex)
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





void PRINT_DEBUG_STRING(char *debugString, int lenDebug)
{


    for(int ii=0;ii<lenDebug;ii++)
    {
        printf("%i 0x%X\n", ii, debugString[ii]);
    }


    return;
}






void fetchCommand(void)
{


    // encode 'beat the shamecocks' 'go tigers'
    // decode 'HSTB ZLV KNOFMISTCY' 'go tigers'
    char *userEntry = (char*)(malloc(sizeof(char) * 128));


    printf("Enter command here:\n");
    fgets(userEntry, 128, stdin);



    int entryLength = strlen(userEntry);
    char *encodeTest = strstr(userEntry, "encode");
    int *messageIndices = findMessageIndices(userEntry, entryLength);

    char *subOne = fetchSubstring(userEntry, messageIndices[0], messageIndices[2]);
    char *subTwo = fetchSubstring(userEntry, messageIndices[1], messageIndices[3]);

    int sizeOne = (messageIndices[2] - messageIndices[0]) + 1;
    int sizeTwo = (messageIndices[3] - messageIndices[1]) + 1;

    char *subOneFixed = strippedCapitalized(subOne, sizeOne);
    printf("New substring one: %s\n", subOneFixed);


    sizeOne = strlen(subOneFixed);

    char *subTwoFixed = normalizeCode(subTwo, sizeTwo, sizeOne);
    printf("New substring two: %s\n", subTwoFixed);



    if(encodeTest != NULL)
    {
        char *encodedVersion = encodeMessage(subOneFixed, subTwoFixed, sizeOne);
        printf("%s\n", encodedVersion);
        return;
    }
    

    char *decodedVersion = decodeMessage(subOneFixed, subTwoFixed, sizeOne);
    printf("%s\n", decodedVersion);

    
    return;


}

int main(int argc, char **argv)
{


    fetchCommand();




    return EXIT_SUCCESS;
}