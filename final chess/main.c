#include <stdio.h>
#include <stdlib.h>
#include <conio.h>
#include <string.h>
#include "chess.h"

gameState gameStack[];
int stackIndex=0,Redo=0;
void pushStack(gameState move)
{
    gameStack[stackIndex]=move;
    stackIndex++;
}
gameState popStack()
{
    gameState move=gameStack[stackIndex-1];
    stackIndex--;
    return move;
}
void save(gameState data,char file[] ){
FILE* output;

output = fopen("data.dat", "wb");

fwrite(&data, sizeof(data), 1, output);

fclose(output);
}
void undo()
{
  Redo++;
  popStack();
}
int redo()
{
    if(Redo==0)
    {
        return 0;
    }
    Redo--;
    stackIndex++;
    return 1;
}
int main()
{
    unsigned long long int board_bits,board_bitsd ;
    gameState X=initializeBoard(),current;
    board_bitsd=X.boardMatrix;
    char str[100],file[100];
    int flag=1;
    pushStack(X);
    //goto Q;
  //  gets(file);
   // puts(file);
    while(1)
    {

            X=gameStack[stackIndex-1];
            display(X);
            printf(" \n");
            gets(str);
            system("cls");
            if(strcmp(str,"save")==0)
            {
                save(X,file);
                continue;
            }
            if(strcmp(str,"load")==0)
            {
                stackIndex=0;
                      FILE* input;
        input = fopen("data.dat", "rb");
        if(input==NULL)
        {
            printf("Cannot load ");
            X=initializeBoard();
            continue;
        }
        else
        fread(&X, sizeof(X), 1, input);
        gameStack[0]=X;
        stackIndex=1;

            fclose(input);

                continue;
            }
            if(strcmp(str,"new")==0)
            {
             stackIndex=1;
             gameStack[0]=initializeBoard();
             continue;
            }
            if(strcmp(str,"exit")==0)
            {
                return 0;
            }
            if(X.gameRound>1)
            {
                continue;
            }
            if(strcmp(str,"undo")==0)
            {
                if(stackIndex<=1)
                    continue;
                stackIndex--;
                Redo++;
                continue;
            }
            if(strcmp(str,"redo")==0)
            {
                if(Redo==0)
                continue;
                stackIndex++;
                Redo--;
                continue;
            }
        if(validate_input(X,str)==0)
          {
              printf("Invalid \n");
              continue;
          }
        X.gameRound=0;
        pair initial,end;
        char promote=0;
        initial.column=str[0]-'A';
        initial.row=8-str[1]+'0';
        end.column=str[2]-'A';
        end.row=8+'0'-str[3];
        if(strlen(str)==5)
        {
            promote=str[4];
        }
        update(&X,initial,end,promote);
        int i,j,a[2]={0};
       a[0]=seeCheckTurn(X,0);
       if((a[0]&(1<<4))>0)
       {
            X.gameRound=1;
           if(a[0]==(1<<10)-1)
                X.gameRound=2;
            if(X.turn==1)
           if(a[0]==(1<<9)-1)
           {

               if(!seeCapture(X,Threat))
               {
                   X.gameRound=2;
               }
           }
       }
       printf("\n");
       a[1]=seeCheckTurn(X,1);
       if((a[1]&(1<<4))>0)
       {
                   X.gameRound=1;
           if(a[1]==(1<<10)-1)
                X.gameRound=2;
            if(X.turn==0)
           if(a[1]==(1<<9)-1)
           {
               if(!seeCapture(X,Threat))
               {
                   X.gameRound=2;
               }
           }

       }
        if((a[X.turn]&(1<<4))>0)
        {

            printf("Invalid Move\n");
            continue;
        }
        if(a[1-X.turn]==(((1<<9)-1)&(~(1<<4))))//All neighboring cells to king are not valid but king is not checked
        {

            if(seeStalemate(X))
            {
               X.gameRound=3;
            }
        }
        Redo=0;
        pushStack(X);

    }
    return 0;
}
/*
A7A5
A2A4
A8A6
B2B4
A5B4
A1A3
B4A3
C1B2
A3B2
D1C1
B2C1q
*/
/*
E7E5
E2E4
F7F5
F1C4
F5E4
D1H5
E8E7
H5E5
*/
/*
D7D5
G1F3
B8D7
E2E4
D5E4
F3G5
H7H6
G5E6
F7E6
D1H5
G7G6
H5G6
*/
/*
A7A6
E2E4
E7E5
G1F3
D7D6
F1C4
H7H6
B1C3
C8G4
F3E5
G4D1
C4F7
E8E7
C3D5
*/
/*
E7E5
E2E4
F8C5
F1C4
D8H4
G1F3
H4F2
*/
/*
E7E5
B1C3
G7G5
C3D5
G8E7
D5F6
*/
/*
E7E5
D2D4
E5D4
G1F3
F8B4
C2C3
D4C3
D1B3
C3B2
B3B4
B2C1q
*/

/*
B7B5
C2C3
B5B4
D1B3
B4C3
B3D5
C3B2
D5A8
B2C1r
*/
/*
stalemate
C7C5
H2H4
H7H5
A2A4
D8A5
A1A3
A5A4
A3H3
A4C2
F2F3
C2D2
E1F2
D2B2
D1D6
B2B1
D6H2
B1C1
F2G3
C1E3
*/
/*
stalemates
D7D5
D2D3
D8D7
E2E4
A7A5
E4E5
D7F5
F2F4
H7H6
F1E2
F5H7
C1E3
A8A6
C2C4
A6G6
D1A4
B8D7
E2H5
F7F6
E3B6
D5D4
E5E6
C7C5
F4F5
*/
/*
A7A5
A2A4
B7B5
A4B5
A8A6
B5A6
H7H5
A6A7
E7E5

*/
