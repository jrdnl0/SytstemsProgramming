

/*

Name: Jacob Daniel 

CUID: jrdnl 

Course: ECE2220

Semester: Spring 2025 

Assignment Number: Project #2 


Purpose: 

The purpose of this program is to allow for the user to intialize and store data that represents the multiple attributes of a college football player. In 
addition, this must all be done through a terminal-based graphical display which is intuitive and easy to navigate. In addition, the code must also
enforce checks that prevent the user from entering data that is considered 'invalid' for that catgeory (based on a variety of parameters). 


Assumptions:  

There are not many assumptions in the code besides the fact that the user will understand the specified rules of the program
(which I believe are quitely explicitly described by the program during runtime), and just follow the rules. I've tried to make this
program 'bulletproof' in the sense that the user attempting to do something 'wrong' by the definition of the program, they will simply
be unable to affect the actual data of the script, as well as only being re-prompted to enter (as well as having the option to exit that screen / entry menu). 



Known Bugs: 

1) I don't know if this would be considered a 'bug,' but if the user were to enter something like a first name something that is over 32 characters, 
they WOULD be re-prompted to enter again, but if they entered 'QUIT,' the program will simply store the shortened version of that string in the data. 

ex: If the user tried to enter the first name as: "THISSTRINGISLONGERTHAN32CHARACTERSITRIED," they would be prompted to enter again, however, 
if they quit rather than trying to enter a valid string, my code will store the string as: "THISSTRINGISLONGERTHAN32CHARACTE" 

I'm not sure if this is considered a 'bug' as it doesn't technically break any rules and I see this happen all the time with actual software, so I just kept it 
as it seemed like a pretty 'safe' way to ensure the lengths of the various data entries aren't violated

*/



/*INCLUDE STATEMENTS*/
#include <stdio.h>
#include <stdlib.h>
#include <string.h>

/*DEFINING CONSTANTS*/
#define TRUE 1 
#define FALSE 0
#define QUIT "QUIT\n"
#define ERR_MESSAGE "INVALID SELECTION! Please enter an option corresponding to the numbers in the brackets!"
#define UNSET -1 // Every numeric value and their real-world counterparts technically can't be negative, so this was an easy choice for me
#define UNSET_STR "N/A"

#define test "THISSTRINGISLONGERTHAN32CHARACTERSITRIED"


typedef struct Player
/*

For this project, I opted to utilize a C struct to represent the data attributed to the 'player' being edited in the code. It seemed somewhat as an 
obvious choice from a design perspective as it allowed for easy manipulation of variables with the use of an access pointer to the struct being the most
common argument / means of manipulating crucial data whether than worrying about pointers and their behaviors between arrays and strings too much. 

Maybe I'm just weird, but I find structs much easier to deal with in a programming setting that keeping tabs on a bunch of variables and their own 'special'
functions. I know you mentioned saying my code design and writing style you find commendable (I'm very flattered, by the way) and this additionally 
made the code a lot cleaner in my eyes (albeit a bit lengthy, I'll try and dial it back, perhaps this was overly-modular). 

The main thing I'd like to make clear regarding the way I designed the implementation of this struct is the fact that the string variables are char * and not 
char[int] upon declaration -- this is a VERY purposeful design as not only does this make it much more intuitive to use the struct Player* pointers 
in my functions to edit the names themselves, but also leverage the malloc() function as well with more ease. Additionally, as you'll see, the size of these 
different strings is enforced through the functions that handle their respective user inputs and value assingments, as well, so declaring this inside the 
struct is unnecessary. 

*/
{
    
    char *first_name; 
    char *last_name; 
    char *school; 
    char *standing; 
    char *position; 
    char *twitter_handle; 

    char middle_name; 


    int age; 
    int weight; 
    int height_feet; 
    int height_in;
    int star_rating; 


    double NIL_money;


    long int phone_number;

}Player;



void initializePlayer(struct Player *P) 
/*

Arguments:
A pointer to the player struct 

Purpose: 
This is simply a function that creates the "blank slate" version of the player struct before the user actually
edits any of the information.


Returns:
None (is Void)


*/
{
    
    P->first_name = UNSET_STR; 
    P->last_name = UNSET_STR; 
    P->school = UNSET_STR; 
    P->standing = UNSET_STR;
    P->position = UNSET_STR; 
    P->twitter_handle = UNSET_STR; 
    P->middle_name = UNSET;
    P->age = UNSET; 
    P->weight = UNSET; 
    P->height_feet = UNSET;
    P->height_in = UNSET; 
    P->star_rating = UNSET; 
    P->NIL_money = UNSET;
    P->phone_number = UNSET; 
}




char *createAllocatedString(int max_size, char prompt[], char *currentSetting)
/*

Arguments:

max_size -> An integer that represents the max number of characters (including the terminate line) the user can input.

prompt -> A string that allows for the user to create a custom prompting for string entry

currentSetting -> A string that represents the current setting of the player-struct's category during specific function calls; it's used to
ensure the same value is held in the struct if the player decides to exit this 'screen' during runtime

Purpose: 
This function is pretty integral to this entire code, it allows me to prompt and validate different string variables throughout 
the duration, as well as allowing for customization which makes it easy to call it over and over again in different scenarios while 
still allowing for navigability


Returns:
An allocated string/array of characters through the malloc command -- i.e. an address that can be assigned to one of the many string
variables for the player, as well as edited over and over again.

*/
{

    char *S = (char*)(malloc(sizeof(char) * max_size));
    printf("Enter %s here (max %i characters). Or enter 'QUIT' to exit: \n", prompt, max_size-1);
    fgets(S, max_size, stdin);
    if(strcmp(S, QUIT) == 0)
    {
        return currentSetting;
    }

    S[strcspn(S, "\n")] = 0;

    if(strlen(S) < 2)
    {
        return currentSetting; 
        /*
        The smallest a string variable could POSSIBLY be according to the instructions of this project is 2 characters, so we simply treat the input 
        of anything less than 2 characters as the user 'misfiring' hitting submit and keep their precious data unaffected. 
        */
    }

    
    return S;

}



int quickStringFind(char **L, char *target, int sizeL)
/*

Arguments:

L -> An array of strings that represent the set/pool of strins to search through
target -> The string that we're comparing with each member of 'L' to check for any matches
sizeL -> The size of / number of strings within L -- serves as a stopping point for iterating through the list

Purpose: 
This function is called to validate the user entries for the Player's academic standing/class as well the position. When the user inputs
one of these two categories, this function is used to ensure that the entry provided is one of the avaliable options

ex: If the user put in 'Doctor' as the player's position, this function returns 0 to tell the rest of the program that this
entry was NOT valid given the context


Returns:
An integer 1 or 0 to represent if the target string was found in the set L or if it was not


*/
{
    int ii;
    for(ii=0;ii<sizeL;ii++)
    {
        int iiComp = strcmp(L[ii], target);
        if(iiComp == 0)
        {
            return 1;
        }
    }

    return 0;
}



char *returnApprovedString(int max_size, char prompt[], char **L, int sizeL, char *currentSetting)
/*

Arguments:

L -> An array of strings that represent the set/pool of strins to search through

sizeL -> The size of / number of strings within L -- serves as a stopping point for iterating through the list

max_size -> An integer that represents the max number of characters (including the terminate line) the user can input.

prompt -> A string that allows for the user to create a custom prompting for string entry

currentSetting -> A string that represents the current setting of the player-struct's category during specific function calls; it's used to
ensure the same value is held in the struct if the player decides to exit this 'screen' during runtime

Purpose: 

This function is similar to the createAllocString function, but adds in a validation principle with the inclusion of 
the quickStringFind function. This is basically createAllocString but checking to make sure the user's input exists in a 
pre-determined set of options 


Returns:
An allocated string/array of characters through the malloc command -- the same idea as the createAllocString function above


*/
{
    int localError = 0;

    while(TRUE)
    {
        system("clear");
        if(localError == 1)
        {
            printf("Invalid entry! Please enter one of the options below (or enter [1] to QUIT)\n");
            for(int ii=0; ii<sizeL; ii++)
            {
                printf("%s \n", L[ii]);
            }
        }
        localError = 0;
        char *S = (char*)(malloc(sizeof(char) * max_size));
        printf("Enter %s here (max %i characters), or enter [1] to QUIT: \n", prompt, max_size-1);
        fgets(S, max_size, stdin);
        if(strcmp(S, "1\n") == 0)
        {
            return currentSetting;
        }
        S[strcspn(S, "\n")] = 0;
        int checkS = quickStringFind(L, S, sizeL);
        if(checkS == 1)
        {
            return S;
        }
        else
        {
            localError = 1;
        }
    }
}



double doubleWithBounds(double min, double max, char prompt[])
/*

Arguments:
min -> the smallest value the user can input 

max -> the largest value the user can input 

prompt -> A custom messaging prompt for the screen while 'fetching' the user's input

currentSetting -> Represents the current value of the player's category (whichever one that may be) and is the default return 
if the user decides to quit 


Purpose: 
This is a pretty basic function to validate the user input of a floating point / double type in the program. I pretty much
exclusively use this for the entry of the player's NIL money


Returns:
A custom-valued double within a specific range



*/
{
    char A[32];
    double C;
    int err_flag = FALSE;

    while(TRUE)
    {
        if(err_flag == FALSE)
        {
            printf("Please enter a %s between or equal to %.0f and %e! Type 'QUIT' to exit\n", prompt, min, max);
        }
        else
        {
            C = 0.0;
            printf("Entry for %s must be between or equal to %.0f and %e! Please try again or type 'QUIT' to exit\n", prompt, min, max);
        }
        fgets(A, 32, stdin);
        if(strcmp(A, QUIT) == 0)
        {
            return 0;
        }
        else
        {
            sscanf(A, "%lf", &C);
            if((C >= min) && (C <= max))
            {
                return C;
            }
            else
            {
                if(err_flag == FALSE)
                {
                    err_flag = TRUE;
                    continue;
                }
                else
                {
                    continue;
                }
            }
        }
    }

    return 0;
}



int intWithBounds(int min, int max, char prompt[], int currentSetting)
/*

This function is the EXACT same as doubleWithBounds, except it deals with integers, so I won't go into detail with this specific function

*/
{
    char A[32];
    int C;
    int err_flag = FALSE;

    while(TRUE)
    {
        if(err_flag == FALSE)
        {
            printf("Please enter a %s between or equal to %i and %i! Type 'QUIT' to exit\n", prompt, min, max);
        }
        else
        {
            C = 0.0;
            printf("Entry for %s must be between or equal to %i and %i! Please try again or type 'QUIT' to exit\n", prompt, min, max);
        }
        fgets(A, 32, stdin);
        if(strcmp(A, QUIT) == 0)
        {
            return currentSetting;
        }
        else
        {
            sscanf(A, "%i", &C);
            if((C >= min) && (C <= max))
            {
                return C;
            }
            else
            {
                if(err_flag == FALSE)
                {
                    err_flag = TRUE;
                    continue;
                }
                else
                {
                    continue;
                }
            }
        }
    }

    return 0;
}



void optionOne(struct Player *P)
/*

Arguments:
A pointer to the player struct

Purpose: 
This screen is for the first option in the menu. This allows for the player to assign the first name, last name, 
and middle initial to the player. 


Returns:
None (is Void)


*/
{
    int errFlag = 0;
    while(TRUE)
    {
        system("clear");

        if(errFlag == 1)
        {
            printf("%s\n", ERR_MESSAGE);
        }
        
        int userChoice;
        errFlag = 0;

        char *X[] = {"Edit player first name", "Edit player last name", "Edit player middle initial"};
        int lenX = 3;

        printf("EDIT OPTIONS:\n");

        for(int ii = 0; ii < lenX; ii++)
        {
            printf("[%i] %s\n", (ii+1), X[ii]);
        }

        printf("Exit screen [%i]\n", lenX + 1);

        scanf(" %i", &userChoice);
        while(getchar() != '\n');

        if(userChoice == 1)
        {   
            char *currentSetting = P->first_name;
            P->first_name = createAllocatedString(33, "first name", currentSetting);
        }
        else if(userChoice == 2)
        {
            char *currentSetting = P->last_name;
            P->last_name = createAllocatedString(33, "last name", currentSetting);
        }
        else if(userChoice == 3)
        {
            char buff;
            printf("Enter your middle initial below (max 1 character):\n");
            scanf(" %c", &buff);
            P->middle_name = buff;
        }
        else if(userChoice == 4)
        {
            return;
        }
        else
        {
            errFlag = 1;
        }
        
     
    }
}



void optionTwo(struct Player *P)
/*

Arguments:
A pointer to the player struct

Purpose: 
This function represents the second option on the menu, and allows the user to change the name of the school
the player attends and the class/academic standing of the player


Returns:
None (is Void)


*/
{
    int errFlag = 0;
    while(TRUE)
    {
        system("clear");
        int userChoice;
        if(errFlag == 1)
        {
            printf("%s\n", ERR_MESSAGE);
        }

        errFlag = 0;
        printf("EDIT OPTIONS:\n");
        printf("[1] Edit name of school attending\n");
        printf("[2] Edit class standing\n");
        printf("[3] Exit screen\n");

        scanf(" %i", &userChoice);
        while (getchar() != '\n');

        if(userChoice == 1)
        {   
            char *currentSetting = P->school;
            P->school = createAllocatedString(65, "Name of School Attending", currentSetting);
        }
        else if(userChoice == 2)
        {
            char *ALL_VALID_STANDINGS[] = {"RFR", "RSO", "FJR", "RSR", "FR", "SO", "JR", "SR"};
            char *currentSetting = P->standing;
            P->standing = returnApprovedString(4, "academic standing", ALL_VALID_STANDINGS, 8, currentSetting);
        }
        else if(userChoice == 3)
        {
            return;
        }
        else
        {
            errFlag = 1;
        }
    }
}



void optionThree(struct Player *P)
/*

Arguments:
A pointer to the player struct

Purpose: 
This is the function for the third option on the menu. Allows for the user to edit the age, height, and weight of the player. 


Returns:
None (is Void)


*/
{
    int errFlag = 0;
    while(TRUE)
    {
        system("clear");
        int userChoice;
        if(errFlag == 1)
        {
            printf("%s\n", ERR_MESSAGE);
        }
        errFlag = 0;
        char *X[] = {"Edit player age", "Edit player height", "Edit player weight"};
        int lenX = 3;
        printf("EDIT OPTIONS:\n");
        for(int ii=0; ii<lenX; ii++)
        {
            printf("[%i] %s\n", (ii+1), X[ii]);
        }
        printf("Exit screen [%i]\n", lenX+1);

        scanf(" %i", &userChoice);
        while (getchar() != '\n');

        if(userChoice == 1)
        {
            int currentSetting = P->age;
            P->age = intWithBounds(21, 99, "age", currentSetting);
        }
        else if(userChoice == 2)
        {
            int currentSetting = P->height_in;
            int height_inches = intWithBounds(48, 84, "height (inches)", currentSetting);
            if(height_inches != UNSET)
            {
                P->height_feet = (height_inches / 12);
                P->height_in = (height_inches % 12);
            }
        }
        else if(userChoice == 3)
        {
            int currentSetting = P->weight;
            P->weight = intWithBounds(100, 500, "weight (lbs)", currentSetting);
        }
        else if(userChoice == 4)
        {
            return;
        }
        else
        {
            errFlag = 1;
        }
    }
}



void optionFour(struct Player *P)
/*

Arguments:
A pointer to the player struct

Purpose: 
This is the function for the fourth option on the menu. Allows for the user to change the position of the player, 
their star rating, as well as their NIL contract/earnings


Returns:
None (is Void)


*/
{
    char *ALL_POS[] = {"QB", "RB", "WR", "TE", "OL", "DL", "LB", "CB", "S", "K", "P"};
    int errFlag = 0;
    while(TRUE)
    {
        system("clear");
        int userChoice;
        if(errFlag == 1)
        {
            printf("%s\n", ERR_MESSAGE);
        }

        errFlag = 0;
        char *X[] = {"Edit player position", "Edit player star rating", "Edit player NIL money"};
        int lenX = 3;
        printf("EDIT OPTIONS:\n");
        for(int ii=0; ii<lenX; ii++)
        {
            printf("[%i] %s\n", (ii+1), X[ii]);
        }
        printf("Exit screen [%i]\n", lenX+1);
        
        scanf(" %i", &userChoice);
        while(getchar() != '\n');

        if(userChoice == 1)
        {
            char *currentSetting = P->position;
            P->position = returnApprovedString(3, "position", ALL_POS, 11, currentSetting);
        }
        else if(userChoice == 2)
        {
            int currentSetting = P->star_rating;
            P->star_rating = intWithBounds(0, 5, "star rating", currentSetting);
        }
        else if(userChoice == 3)
        {
            P->NIL_money = doubleWithBounds(0, 1e+50, "NIL money");
        }
        else if(userChoice == 4)
        {
            return;
        }
        else
        {
            errFlag = 1;
        }
    }
}



void optionFive(struct Player *P)
/*

Arguments:
A pointer to the player struct

Purpose: 
The function for the fifth option in the menu. Allows for the user to set the contact information of the player: their twitter handle
as well as their phone number


Returns:
None (is Void)


*/
{
    int errFlag = 0;
    while(TRUE)
    {
        system("clear");
        if(errFlag == 1)
        {
            printf("%s \n", ERR_MESSAGE);
        }

        errFlag = 0;
        int userChoice = 0;

        printf("EDIT OPTIONS:\n");
        printf("[1] Enter player Twitter handle\n");
        printf("[2] Enter player phone number\n");
        printf("[3] Exit screen\n");


        scanf(" %i", &userChoice);

        if(userChoice == 1)
        {
            int localErrorFlag = -1;
            while(TRUE)
            {
                system("clear");
                if(localErrorFlag == 1)
                {
                    printf("Invalid entry! Username must be <= 15 characters and begin with an @ symbol. Or type 'QUIT' to exit (ex: '@jrdnlCU)\n");
                    localErrorFlag = 0;
                }
                
                char *currentSetting = P->twitter_handle;
                char *twitterBuffer = createAllocatedString(16, "twitter handle", currentSetting);
                
                if(twitterBuffer[0] != '@')
                {
                    localErrorFlag += 1;
                }
                else
                {
                    P->twitter_handle = twitterBuffer;
                    break;
                }
            }
        }

        else if(userChoice == 2)
        {
            int localErrorFlag = 0;
            while(TRUE)
            {
                system("clear");
                if(localErrorFlag == 1)
                {
                    printf("Invalid entry! Phone number must be an input between (or equal to) 0 and 9999999999 (ex: 6532479976)\n");
                }

                localErrorFlag = 0;

                long int phoneBuffer;
                printf("Enter player phone number: \n");
                scanf(" %li", &phoneBuffer);

                if((phoneBuffer >= 0) && (phoneBuffer <= 9999999999))
                {
                    P->phone_number = phoneBuffer;
                    break;
                }
                else
                {
                    localErrorFlag = 1;
                }
            }
        }

        else if(userChoice == 3)
        {
            return;
        }
        else
        {
            errFlag = 1;
        }
    }
}



void printPhoneNumber(long int number)
/*

Arguments: 

number -> A long integer that simply represents the player's entered phone number in it's numeric form. 

Purpose: 

This is a glorified printf function that utilizes a looping structure that gets the value of each digit / "place" of the long int 
that represents the phone number and then stores them into an array, which is then printed in the specified format. 

I initially did it as a small way to test how I wanted to approach turning the numeric version of the phone number into a 
printable string, but kept it as I liked the neatness it brought to the original version of the optionSix function I had in all its if-else glory. 

Returns:

None (is Void)


*/
{
    int ii = 9;
    int X[10];
    while(number != 0)
    {
        X[ii] = (number % 10);
        number /= 10;
        ii--;
    }

    printf("Phone Number: (%i%i%i)%i%i%i-%i%i%i%i \n", X[0], X[1], X[2], X[3], X[4], X[5], X[6], X[7], X[8], X[9]);

    return;
}



void optionSix(struct Player *P)
/*

Arguments:
A pointer to the player struct

Purpose: 

This is the option to display all of the data. This is a pretty archaic solution -- I basically just go through every variable and check if it's 
'unset' by the program's standards -- if so, we treat it as such. If not, then we simply just print the data normally (with a few exceptions).

The three 'odd ones out' of the printing function are the NIL money, the star rating, and the telephone number.

The NIL money simply gets printed in scientific formatting if the number is over a certain size to avoid flooding the terminal/display with
a 10+ zeros. 

The phone number and star rating are special as they're stored as numeric datatypes but have to be displayed as strings. 

The star rating one is pretty simple as it's just 0-5 stars and we can take the integer value as an index and simply print the corresponding index in an array of strings 
that correspond to each rating. (e.g. 3 star rating would be the third index in an array of strings and would print "***" as seen later).

As for the phone number -- I basically bit the bullet and made a small little function that gets each digit of the number and stores them into an 
array with is custom-printed (as seen above this function). 



Returns:
None (is Void)


*/
{
    while(TRUE)
    {
        system("clear");
        if(P->middle_name == UNSET)
        {
            P->middle_name = ' ';
        }
        printf("Name: %s %c %s \n", P->first_name, P->middle_name, P->last_name);

        if(P->age == UNSET)
        {
            printf("Age: N/A \n");
        }
        else
        {
            printf("Age: %i \n", P->age);
        }

        if((P->height_in == UNSET))
        {
            printf("Height: N/A \n");
        }
        else
        {
            printf("Height: %i' %i'' \n", P->height_feet, P->height_in);
        }



        if(P->weight == UNSET)
        {
            printf("Weight: N/A lbs. \n");
        }
        else
        {
            printf("Weight: %i lbs. \n", P->weight);
        }

        printf("University Attending: %s \n", P->school);
        printf("Academic Standing/Year: %s \n", P->standing);
        printf("Position: %s \n", P->position);

        if(P->star_rating == UNSET)
        {
            printf("Star Rating: N/A \n");
        }
        else
        {
            char *allStars[] = {"", "*","**","***","****","*****"};
            printf("Star Rating: %s \n", allStars[P->star_rating]);
        }

        if((P->NIL_money == UNSET) || (P->NIL_money == 0.00))
        {
            printf("Current NIL Contract: N/A \n");
        }
        else
        {
            if(P->NIL_money > 1E10)
            {
                printf("Current NIL Contract: $%e \n", P->NIL_money);
            }
            else
            {
                printf("Current NIL Contract: $%.2f \n", P->NIL_money);
            }
        }


        if(P->phone_number == UNSET)
        {
            printf("Phone number: N/A \n");
        }
        else
        {
            long int number = P->phone_number;
            printPhoneNumber(number);
            
        }


        printf("Twitter username: %s \n", P->twitter_handle);


        int userChoice;
        printf("Enter [1] to stop viewing this screen. \n");
        scanf(" %i", &userChoice);
        if(userChoice == 1)
        {
            return;
        }
    }
}



void optionSeven(struct Player *P)
/*

Arguments:
A pointer to the player struct

Purpose: 
Function for the seventh option on the menu. Essentially asks the user to confirm if they want to wipe all the player 
information. If the user confirms, it just re-runs the intializePlayer command on the struct to reset it back to the 'blank' setting(s). 
If the user decides to back out, the player's information is kept the same. 


Returns:
None (is Void)


*/
{
    int errFlag = 0;
    while(TRUE)
    {
        int userChoice = 0;
        system("clear");

        if(errFlag == 1)
        {
            printf("%s \n", ERR_MESSAGE);
        }

        printf("Are you sure you want to clear all player info? This cannot be undone!\n");
        printf("Enter [1] to CONFIRM or [2] to CANCEL clearing all info.\n");
        scanf(" %i", &userChoice);
        if(userChoice == 1)
        {
            initializePlayer(P);
            return;
        }
        else if(userChoice == 2)
        {
            return;
        }
        else
        {
            errFlag = 1;
        }
    }
}



void optionZero(struct Player *P)
/*

Arguments:
A pointer to the player struct

Purpose: 

This is a function which represents the 'main menu' of the entire program. Rather than putting into the main function, I opted for this 
as it's a bit nicer to read, as well as let me use the main function to print debug certain attributes of the code as I went through how 
my program changed the struct as I progressed writing all of this. 

This function prints all of the options of the menu and takes and takes in the user input. After that it simply directs the program to the option 
function corresponding to the user choice! It's the root of the modular design I try and harness when writing code (as should everyone, but alas). 


Returns:
None (is Void)


*/
{
    void (*funcLists[])(struct Player *) = {optionOne, optionTwo, optionThree, optionFour, optionFive, optionSix, optionSeven};
    int errFlag = 0;
    while(TRUE)
    {
        system("clear");
        if(errFlag == 1)
        {
            printf("%s\n", ERR_MESSAGE);
            errFlag = 0;
        }
        

        int userChoice;

        char *X[] = {"Enter Name", "Enter School Info.", "Enter Age, Height, and Weight", "Enter Player Info.", "Enter Player Contacts", "Display Data", "Clear Data", "Quit Program"};

        int lenX = 8;

        printf("Main Menu:\n");

        for(int ii=0; ii<lenX;ii++)
        {
            printf("[%i] %s \n", (ii+1), X[ii]);
        }

        scanf(" %i", &userChoice);

        if(userChoice == 8)
        {
            return;
        }
        
        if((userChoice >= 1) && (userChoice <= 7))
        {
            userChoice--; // Makes sure the indices are correct since the menu/user start counting at 1, but obviously the programmers don't !
            funcLists[userChoice](P);
        }
        else
        {
            errFlag = 1;
        }
       
    
    }
}




int main(void)
{
    struct Player yourPlayer;
    initializePlayer(&yourPlayer);
    optionZero(&yourPlayer);

    return 0;
}