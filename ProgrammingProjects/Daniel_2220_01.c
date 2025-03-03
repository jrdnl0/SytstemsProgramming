
#include <stdio.h>
#include <stdlib.h>
#include <time.h>

#define ROWS 5
#define COLUMNS 5

#define FILLED 100



int GENERATE_RANDOM(int min, int max)
{
    int number = ((rand() % (max- min)) + min);
    return number;
}


void GENERATE_COLUMN(int *X, int min, int max)
{
    for(int ii=0; ii<5; ii++)
    {
        X[ii] = GENERATE_RANDOM(min, max);

    }


}

void GENERATE_NEW_LETTERS(int *X)
{
    int min = 1;
    int max = 20;
    for(int ii=0; ii<5; ii++)
    {
        X[ii] = GENERATE_RANDOM(min, max);
        max += 20;
        min += 20;
    }
}


void EDIT_BOARD_X(int **X, int *A)
{
    for(int ii=0;ii<5;ii++)
    {
        // Iterate through columns 

        for(int jj=0;jj<5;jj++)
        {
            if(X[ii][jj] == A[ii])
            {
                X[ii][jj] = FILLED;

            }
            else
            {
                continue;
            }
            

        }
    }
}



void PRINT_BOARD(int **X)
{
    
    int ii;
    int jj;
   
    puts("     T        I        G        E        R     ");
    puts("---------------------------------------------");
    
    

    for(ii=0;ii<5;ii++)
    {


        for (jj=0;jj<5;jj++)
        {   
            
            int ii_jj = X[jj][ii];

            if ((ii == 2) & (jj == 2)){
                printf("|  Free  ");

            }
            else
            {
                if (jj == 4)
                {
                    if ((ii_jj >= 10) & (ii_jj != FILLED))
                    {  
                        printf("|   %i  \n", ii_jj);
                    }
                    else if (ii_jj == FILLED)
                    {
                        printf("|   X    \n");
                    }
                    else
                    {
                        printf("|   %i   \n", ii_jj);
                    }
                }
                else if(jj != 4)
                {
                    if ((ii_jj >= 10) & (ii_jj != 100))
                    {
                        printf("|   %i   ", ii_jj);
                    }
                    else if(ii_jj == 100)
                    {
                        printf("|   X    ");
                    }
                    else
                    {
                        printf("|    %i   ", ii_jj);
                    }
                }
            }
            
            
               
            
        }

        puts("|--------|--------|--------|--------|--------|");
        
    }


}


void PRINT_LETTERS(int *X)
{
    printf("T - %i\n", X[0]);
    printf("I - %i\n", X[1]);
    printf("G - %i\n", X[2]);
    printf("E - %i\n", X[3]);
    printf("R - %i\n", X[4]);

}

int CHECK_ROW_COL_WIN(int *X)
{
    int ii;
    int counter = 0;
    for(ii=0;ii<5;ii++)
    {
        if(X[ii] == 100)
        {
            counter += 1;
        }
        else
        {
            return 0;
        }

    }

    if(counter == 5)
    {
        return 1;
    }
    
    return 0;

}

int CHECK_WIN_CONDITIONS(int **X)
{

    int ii, jj;

    int diagonal_one[5] = {(int)X[0][0], (int)X[1][1], (int)X[2][2], (int)X[3][3], (int)X[4][4]};
    int diagonal_two[5] = {(int)X[0][4], (int)X[1][3], (int)X[2][2], (int)X[3][1], (int)X[4][0]};

    int check_diagonal_one = CHECK_ROW_COL_WIN(diagonal_one);
    int check_diagonal_two = CHECK_ROW_COL_WIN(diagonal_two);

    if ((check_diagonal_one == 1) | (check_diagonal_two == 1))
    {
        return 1;
    }
   

    for(ii=0;ii<5;ii++)
    {
        int row_ii[5];
        for(jj=0;jj<5;jj++)
        {
            row_ii[jj] = X[jj][ii];
            
        }

        int check_result_one = CHECK_ROW_COL_WIN(row_ii);
        int check_result_two = CHECK_ROW_COL_WIN(X[ii]);
        if ((check_result_one == 1) | (check_result_two == 1))
        {
            return 1;
        }
        else
        {
            continue;
        }


    }
    

    return 0;
}




int main(void)
{
    srand(time(NULL));
    int T[5];
    int I[5];
    int G[5];
    int E[5];
    int R[5];

    int *GAME_BOARD[5] = {T, I, G, E, R};
    int min = 1;
    int max = 20;

    int ALL_LETTERS[5];

    for(int ii=0; ii<5; ii++)
    {
        GENERATE_COLUMN(GAME_BOARD[ii], min, max);
        ALL_LETTERS[ii] = GENERATE_RANDOM(min, max);
        max += 20;
        min += 20;
    }

    G[2] = 100;


    PRINT_BOARD(GAME_BOARD);
    PRINT_LETTERS(ALL_LETTERS); 
    int flag = 1;
    int attempt_counter = 1;

    while(flag == 1)
    {

        char input;
        printf("Press 'q' to quit or another key to continue!\n");
        scanf("%c", &input);
        if (input == 'q') 
        {
            system("clear");
            flag = 0;
        } 
        else 
        {   
            system("clear");
            attempt_counter += 1;
            GENERATE_NEW_LETTERS(ALL_LETTERS);  
            EDIT_BOARD_X(GAME_BOARD, ALL_LETTERS);
            PRINT_BOARD(GAME_BOARD);
            PRINT_LETTERS(ALL_LETTERS);
            printf("Attempts Made: %i\n", attempt_counter);

            int check_results = CHECK_WIN_CONDITIONS(GAME_BOARD);
            if(check_results == 1)
            {
                flag = 0;
            }
            else
            {
                continue;
            }
        }


    }

    return 0;


}