#ifndef CHESS_H_INCLUDED
#define CHESS_H_INCLUDED
#include <stdio.h>
#include <stdlib.h>
#include <conio.h>
#include <string.h>
typedef struct gameState gameState;
typedef struct Player Player;
typedef struct pair pair;
//structure contains coordinates on game board
struct pair
{
    int row;
    int column;
};
// potential threat to any king
pair Threat;
//Conversion of integers to board coordinates
pair makePair(int x,int y)
{
    pair A;
    A.row=x;
    A.column=y;
    return A;
}
//simple structure to contain pieces positions
struct Player
{
    pair positions[16];
};
//Grand structure that packs all game specs
struct gameState
{

    unsigned long long int boardMatrix; //Board representation as unsigned long for path validation
    char board[8][8]; // Board representation as character matrix for pieces validation and display
    signed char indexBoard[8][8]; //matrix that links pieces positions with their indexes in the player array
    char taken[2][16]; //arrays of taken out pieces
    int i,j; //indexes if taken array
    int turn; //players move
    int gameRound; // Integer that represents game state of (gameplay/check /checkmate/stalemate)
    Player Player[2]; // the 2 players pieces
    pair kingPos[2]; // kings positions on board
};
//Function that sets game board to start the game
gameState initializeBoard( )
{
    gameState startBoard= {.board={"rhbqkbhr","pppppppp","._._._._",
                                   "_._._._.","._._._._","_._._._.","PPPPPPPP","RHBQKBHR"
                                  },.i=0,.j=0,.turn=0,.gameRound=0
                          };
    startBoard.boardMatrix=(~0);
    startBoard.boardMatrix-=((unsigned long long )2<<47);//setting up pieces presence on game board for path validation
    startBoard.boardMatrix+=((unsigned long long)2<<15); //
    int i;
    //setting indexes for player 1 pieces position and linking them to board
    for(i=0; i<16; i++)
    {
        startBoard.Player[0].positions[i]=makePair(i>7,i%8);
        startBoard.indexBoard[i>7][i%8]=i;
    }
    //setting indexes for player 2 pieces position and linking them to board
    for(i=0; i<16; i++)
    {
        startBoard.Player[1].positions[i]=makePair((i>7)+6,i%8);
        startBoard.indexBoard[6+(i>7)][i%8]=i;
    }
    //setting up kings positions
    startBoard.kingPos[0]=makePair(0,4);
    startBoard.kingPos[1]=makePair(7,4);
    return startBoard;
}
//function for board display
void display(gameState move)
{
    int gt=move.turn;
    printf("  A B C D E F G H\n");
    int i,j;
    for(i=0; i<8; i++)
    {
        printf("%d ",8-i);
        for(j=0; j<8; j++)
        {
            printf("%c ",move.board[i][j]);
        }
        printf(" %d\n",8-i);
    }
    printf("  A B C D E F G H\n");
    printf("taken out from black : ");
    for(i=0; i<move.i; i++)
        printf(" %c",move.taken[1][i]);
    printf("\ntaken out from white : ");
    for(j=0; j<move.j; j++)
        printf(" %c",move.taken[0][j]);
    if (move.gameRound==0)
        printf("\nplay next turn player %d\n",gt%2+1);
    else if (move.gameRound==1)
        printf("\ncheck\n");
    else if (move.gameRound==2)
        printf("\ncheck mate player %d wins\nThank You For Playing please Enter \"New\" for a new game\n or \"Load\" for a saved game\n",(gt+1)%2+1);
    else
        printf("\nstalemate\nThank You For Playing please Enter \"New\" for a new game\n or \"Load\" for a saved game\n");
}
//function for changing board status
void update(gameState *move,pair initial,pair end,char promote)
{
    if(move->board[end.row][end.column]!='.'&&move->board[end.row][end.column]!='_')//move contains taking opponent piece out
    {
        int turn=move->turn;
        int k=(turn)?move->j:move->i;
        move->taken[1-turn][k]=move->board[end.row][end.column]; //adding taken out piece into array
        if(move->turn)
            (move->j)++;
        else
            (move->i)++;
        move->Player[1-turn].positions[move->indexBoard[end.row][end.column]]=makePair(-1,-1); //removing piece positions
    }
    move->board[end.row][end.column]=move->board[initial.row][initial.column]; //moving piece to new position
    if((initial.row+initial.column)%2==0)//removing piece from initial position
        move->board[initial.row][initial.column]='.';
    else
        move->board[initial.row][initial.column]='_';
    if(promote!=0) //checking promotion
    {
        move->board[end.row][end.column]=promote;
    }
    int a,b;
    a=end.row,b=end.column;
    move->boardMatrix|=((unsigned long long)1<<((8*a)+b)); //equation for clearing initial position of piece
    a=initial.row,b=initial.column;
    int P;
    move->boardMatrix&=(~((unsigned long long )1<<((8*a)+b))); //equation for loading new position
    P=move->indexBoard[end.row][end.column]=move->indexBoard[initial.row][initial.column];
    if(move->board[end.row][end.column]=='k'||move->board[end.row][end.column]=='K')move->kingPos[move->turn]=end;
    move->indexBoard[initial.row][initial.column]=0;
    move->Player[move->turn].positions[P]=end;//updating piece position
    move->turn=1-move->turn; //updating turn
}
//function for path validation
//gets 2 coordinates and board representation returns 1 if the path between them is empty
int validate_path(unsigned long long Board,pair initial,int rowDiff,int  powerDiff )
{
    int a=initial.row,b=initial.column,c=a+rowDiff;
    unsigned long long d;
    if(powerDiff==8) //vertical path
    {
        //creating unsigned long representing path
        d=((unsigned long long int)1<<(8*a+b));
        d=(d*(((((unsigned long long int)1<<8*(c-a))-1))/255))-d;
    }
    else if(powerDiff==9) //main diagonal path
    {
        d=((unsigned long long int)1<<(8*a+b));
        d=(d*(((((unsigned long long int)1<<9*(c-a))-1))/511))-d;
    }
    else if(powerDiff==7)//secondary diagonal path
    {
        d=((unsigned long long int)1<<(8*a+b));
        d=(d*(((((unsigned long long int)1<<7*(c-a))-1))/127))-d;
    }
    //if path representation and board representation have  common pieces then the path is loaded
    return ((d&Board)>0);
}
//function that checks the path is valid (vertical/horizontal/diagonal) and calls the above function
int startValidatePaths(unsigned long long boardMatrix,pair initial,pair end, int choose)
{
    //choose is for path verification (horizontal for rooks and queen...etc)
    //swapping coordinates if they are decreasing
    //this makes no difference to the logical process
    if(initial.row>end.row)
    {
        pair temp=initial;
        initial=end;
        end=temp;
    }
    //Horizontal move
    if(initial.row==end.row&&(choose&1))
    {
        int a=initial.row;
        int b=initial.column;
        int c=end.column;
        //swapping column to be increasing
        //this makes no difference to the logical process
        if(c<b)
        {
            int temp=c;
            c=b;
            b=temp;
        }
        //path representation (as in path validation function)
        unsigned long long T1=((unsigned long long int)1<<(8*a+c))-1;
        unsigned long long T2=((unsigned long long int)1<<(8*a+b))-1;
        T1=T1&(~T2)-(T2+1);
        if((T1&boardMatrix)>0)
        {
            return 0;
        }
        return 1;
    }
    //vertical path
    if(initial.column==end.column&&(choose&1))
    {
        if(validate_path(boardMatrix,initial,end.row-initial.row,8))
        {
            // loaded path
            return 0;
        }
        return 1;
    }
    //main diagonal path
    if(initial.row-end.row==initial.column-end.column&&(choose&2))
    {
        if(validate_path(boardMatrix,initial,end.row-initial.row,9))
        {
            // loaded path
            return 0;
        }
        return 1;

    }
    //secondary diagonal path
    if(initial.row-end.row==end.column-initial.column&&(choose&2))
    {
        if(validate_path(boardMatrix,initial,end.row-initial.row,7))
        {
            // loaded path
            return 0;
        }
        return 1;

    }
    return 0;
}
//function that validates user input and piece choice
int validate_input(gameState game,char inp[])
{
    int promotionFlag=0;
    if(strlen(inp)<4||strlen(inp)>5)
    {
        //garbage or invalid input
        return 0;
    }
    if(strlen(inp)==5)
    {
        //promotion validation activated
        promotionFlag=1;
    }
    //input conversion into coordinates
    char promoted=0;
    pair initial,end;
    initial.column=inp[0]-'A';
    initial.row=8-inp[1]+'0';
    end.column=inp[2]-'A';
    end.row=8+'0'-inp[3];
    if(promotionFlag)
        promoted=inp[4];
    char peice=game.board[initial.row][initial.column];
    int turn=game.turn;
    //boundary check
    if(initial.row<0||initial.row>7||end.row<0||end.row>7||initial.column<0||initial.column>7||end.column<0||end.column>7)
    {
        //out of boundaries or invalid input
        return 0;
    }
    if(initial.row==end.row&&initial.column==end.column)
    {
        //no move
        return 0;
    }
    if(game.board[end.row][end.column]>'a'+('A'-'a')*turn&&game.board[end.row][end.column]<'z'+('A'-'a')*turn)
    {
        // taking out own pieces
        return 0;
    }
    if(promotionFlag)
    {
        if(end.row!=(7-7*turn)||peice!='p'+('A'-'a')*turn)
        {
            //non pawn promotion or out of position promotion
            return 0;
        }
        if(promoted!='r'+('A'-'a')*turn&&promoted!='b'+('A'-'a')*turn&&promoted!='q'+('A'-'a')*turn&&promoted!='h'+('A'-'a')*turn)
        {
            //invalid position promotion
            return 0;
        }
        //not a promotion
    }

    //rook check
    if(peice=='r'+('A'-'a')*turn)
    {
        return startValidatePaths(game.boardMatrix,initial,end,1);
    }
    //bishop check
    if(peice=='b'+('A'-'a')*turn)
    {
        return startValidatePaths(game.boardMatrix,initial,end,2);
    }
    //queen check
    if(peice=='q'+('A'-'a')*turn)
    {
        return startValidatePaths(game.boardMatrix,initial,end,3);
    }
    //horse check
    if(peice=='h'+('A'-'a')*turn)
    {
        //Horse doesnt need path validation only move validation
        int rowJump=abs(end.row-initial.row);
        int colJump=abs(end.column-initial.column);
        if(rowJump>2||colJump>2||rowJump<1||colJump<1||rowJump==colJump)
            return 0;
        return 1;
    }
    //king validation
    if(peice=='k'+('A'-'a')*turn)
    {
        if(abs(end.row-initial.row)>1)
            return 0;
        if(abs(end.column-initial.column)>1)
            return 0;
        return 1;
    }
    //pawn check
    if(peice=='p'+('A'-'a')*turn)
    {
        if((game.board[end.row][end.column]=='.')||(game.board[end.row][end.column]=='_'))
        {

            if(end.column!=initial.column)
            {
                //
                return 0;
            }
            if(end.row<initial.row&&turn==0)
            {
                //
                return 0;
            }
            if(end.row>initial.row&&turn==1)
            {
                //
                return 0;
            }

            if(abs(end.row-initial.row)>2)
            {
                //
                return 0;
            }
            if(abs(end.row-initial.row)==2&&initial.row!=(6-5*(1-turn)))
            {
                //
                return 0;
            }
            if(abs(end.row-initial.row)==2&&game.board[initial.row+1-2*turn][initial.column]!='.'&&game.board[initial.row+1-2*turn][initial.column]!='_')
            {
                //
                return 0;
            }
            return 1;
        }
        if(end.row-initial.row!=(1-2*turn))
        {
            //
            return 0;
        }
        if(abs(end.column-initial.column)!=1)
        {
            //
            return 0;
        }
        return 1;
    }
    return 0;
}
//function that detects opponent pieces threat on king and neighboring cells
int kingThreat(gameState game,pair initial,pair end)
{
    int choose=0,turn=game.turn;
    int x=0;//integer that represent king cell and neighboring cells
    pair threat1=makePair(-1,-1),threat2=makePair(-1,-1);
    char peice=game.board[initial.row][initial.column];
    game.boardMatrix&=(~((unsigned long long)1<<(end.row*8+end.column)));//removing king presence from board representation as it can't cover other cells
    if(peice=='r'+('A'-'a')*turn)
    {
        choose=1;
    }
    else if(peice=='b'+('A'-'a')*turn)
    {
        choose=2;
    }
    else if(peice=='q'+('A'-'a')*turn)
    {
        choose=3;
    }
    else if(peice=='h'+('A'-'a')*turn)
    {
        choose=4;
    }
    else if(peice=='p'+('A'-'a')*turn)
    {
        threat1=makePair(initial.row+1-2*turn,initial.column+1);
        threat2=makePair(initial.row+1-2*turn,initial.column-1);
    }
    int i,j;
    for(i=-1; i<2; i++)
    {
        for(j=-1; j<2; j++)
        {
            pair temp=makePair(end.row+i,end.column+j);//neighboring cell holder
            if(temp.row==initial.row&&temp.column==initial.column) //if threatening piece in neighboring cell it is not a threat
                continue;
            if(temp.row<0||temp.row>7||temp.column<0||temp.column>7) //if neighboring cell is out of borders then the king can't move to it
            {
                x+=1<<((i+1)*3+(j+1));
            }
            else if(choose==4) //the threat is a horse no path validation
            {
                int rowJump=abs(temp.row-initial.row);
                int colJump=abs(temp.column-initial.column);
                if(rowJump>2||colJump>2||rowJump<1||colJump<1||rowJump==colJump)
                    continue;
                x+=1<<((i+1)*3+(j+1));
            }
            else if(startValidatePaths(game.boardMatrix,initial,temp,choose)) //threat has path validation
            {
                x+=1<<((i+1)*3+(j+1));
            }
            else if((temp.row==threat1.row&&temp.column==threat1.column)||(temp.row==threat2.row&&temp.column==threat2.column)) //threat is a pawn
            {
                x+=1<<((i+1)*3+(j+1));
            }
            else if(game.board[temp.row][temp.column]>'a'+(('A'-'a')*(1-turn))&&game.board[temp.row][temp.column]<'z'+(('A'-'a')*(1-turn)))//cell is occupied with own piece
            {
                if(temp.row!=end.row||temp.column!=end.column)
                    x+=1<<((i+1)*3+(j+1));
            }
        }
    }
    return x;
}
//function for coordinate addition
pair addPair(pair A,pair B)
{
    return makePair(A.row+B.row,A.column+B.column);
}
//function called on check that sees ability to block or remove threat
int seeCapture(gameState game,pair target)
{
    int i,j,choose=0;
    pair initial,sum,holder=target;
    int turn=game.turn;
    pair King=game.kingPos[turn];
    if(target.row==King.row)
    {
        sum.row=0;
        sum.column=1-2*(target.column>King.column);
    }
    else if(target.column==King.column)
    {
        sum.row=1-2*(target.row>King.row);
        sum.column=0;
    }
    else if(abs(target.row-King.row)==abs(target.column-King.column))
    {
        sum.row=-1+2*(target.row<King.row);
        sum.column=-1+2*(target.column<King.column);
    }

    for(j=0; target.row!=King.row||target.column!=King.column; target=addPair(target,sum),j++) //looping on threating cell and all path cells
    {
        for(i=0; i<16; i++)//looping on pieces to check if they can block
        {
            choose=0;
            initial =game.Player[turn].positions[i];
            if(initial.row<0)continue;
            char peice=game.board[initial.row][initial.column];
            if(peice=='r'+('A'-'a')*turn)
            {
                choose=1;
            }
            else if(peice=='b'+('A'-'a')*turn)
            {
                choose=2;
            }
            else if(peice=='q'+('A'-'a')*turn)
            {
                choose=3;
            }
            else if(peice=='h'+('A'-'a')*turn)
            {
                int rowJump=abs(target.row-initial.row);
                int colJump=abs(target.column-initial.column);
                if(rowJump>2||colJump>2||rowJump<1||colJump<1||rowJump==colJump)
                    continue;
                return 1;
            }
            else if(peice=='p'+('A'-'a')*turn)
            {
                if(j==0)
                    if(initial.row==target.row-1+2*(turn))
                        if(abs(initial.column-target.column)==1)
                        {
                            return 1;
                        }
                if(j>0)
                    if((initial.row==target.row-1+2*(turn))&&(initial.column==target.column))
                    {
                        return 1;
                    }
                continue;
            }
            if(startValidatePaths(game.boardMatrix,initial,target,choose))
            {
                return 1;
            }
        }
        if(j==0)
        {
            //no path for horse and pawn
            if(game.board[target.row][target.column]=='H'+('h'-'H')*turn||game.board[target.row][target.column]=='P'+('h'-'H')*turn)
            {

                return 0;
            }
        }
    }
    return 0;
}
//function to check stalemate
int seeStalemate(gameState game)
{
    int turn=game.turn,i;
    pair peicePos;
    char peice;
    for(i=0; i<16; i++)//looping on all pieces to detect possible
    {
        peicePos =game.Player[turn].positions[i];
        if(peicePos.row<0)continue;//removed piece
        peice=game.board[peicePos.row][peicePos.column];
        if(peice=='r'+('A'-'a')*turn||peice=='q'+('A'-'a')*turn)//vertical and horizontal moves
        {
            if(peicePos.row>0)
            {
                if(!(game.board[peicePos.row-1][peicePos.column]>'a'+('A'-'a')*turn&&game.board[peicePos.row-1][peicePos.column]<'z'+('A'-'a')*turn))
                {
                    gameState temp=game;
                    temp.boardMatrix&=(~((unsigned long long )1<<((8*peicePos.row)+peicePos.column)));//removing piece and detecting checks
                    if((seeCheckTurn(temp,1-temp.turn)&(1<<4))==0)
                    {

                        return 0;
                    }
                }
            }
            if(peicePos.row<7)
            {
                if(!(game.board[peicePos.row+1][peicePos.column]>'a'+('A'-'a')*turn&&game.board[peicePos.row+1][peicePos.column]<'z'+('A'-'a')*turn))
                {
                    gameState temp=game;
                    temp.boardMatrix&=(~((unsigned long long )1<<((8*peicePos.row)+peicePos.column)));
                    if((seeCheckTurn(temp,1-temp.turn)&(1<<4))==0)
                    {

                        return 0;
                    }
                }
            }
            if(peicePos.column>0)
            {
                if(!(game.board[peicePos.row][peicePos.column-1]>'a'+('A'-'a')*turn&&game.board[peicePos.row][peicePos.column-1]<'z'+('A'-'a')*turn))
                {
                    gameState temp=game;
                    temp.boardMatrix&=(~((unsigned long long )1<<((8*peicePos.row)+peicePos.column)));
                    if((seeCheckTurn(temp,1-temp.turn)&(1<<4))==0)
                    {

                        return 0;
                    }
                }
            }
            if(peicePos.column<7)
            {
                if(!(game.board[peicePos.row][peicePos.column+1]>'a'+('A'-'a')*turn&&game.board[peicePos.row][peicePos.column+1]<'z'+('A'-'a')*turn))
                {
                    gameState temp=game;
                    temp.boardMatrix&=(~((unsigned long long )1<<((8*peicePos.row)+peicePos.column)));
                    if((seeCheckTurn(temp,1-temp.turn)&(1<<4))==0)
                    {

                        return 0;
                    }
                }
            }
        }
        else if(peice=='b'+('A'-'a')*turn||peice=='q'+('A'-'a')*turn)
        {
            if(peicePos.row>0&&peicePos.column>0)
            {
                if(!(game.board[peicePos.row-1][peicePos.column-1]>'a'+('A'-'a')*turn&&game.board[peicePos.row-1][peicePos.column-1]<'z'+('A'-'a')*turn))
                {
                    gameState temp=game;
                    temp.boardMatrix&=(~((unsigned long long )1<<((8*peicePos.row)+peicePos.column)));
                    if((seeCheckTurn(temp,1-temp.turn)&(1<<4))==0)
                    {

                        return 0;
                    }
                }
            }
            if(peicePos.row>0&&peicePos.column<7)
            {
                if(!(game.board[peicePos.row-1][peicePos.column+1]>'a'+('A'-'a')*turn&&game.board[peicePos.row-1][peicePos.column+1]<'z'+('A'-'a')*turn))
                {
                    gameState temp=game;
                    temp.boardMatrix&=(~((unsigned long long )1<<((8*peicePos.row)+peicePos.column)));
                    if((seeCheckTurn(temp,1-temp.turn)&(1<<4))==0)
                    {

                        return 0;
                    }
                }
            }
            if(peicePos.row<7&&peicePos.column<0)
            {
                if(!(game.board[peicePos.row+1][peicePos.column-1]>'a'+('A'-'a')*turn&&game.board[peicePos.row+1][peicePos.column-1]<'z'+('A'-'a')*turn))
                {
                    gameState temp=game;
                    temp.boardMatrix&=(~((unsigned long long )1<<((8*peicePos.row)+peicePos.column)));
                    if((seeCheckTurn(temp,1-temp.turn)&(1<<4))==0)
                    {

                        return 0;
                    }
                }
            }
            if(peicePos.row<7&&peicePos.column<7)
            {
                if(!(game.board[peicePos.row+1][peicePos.column+1]>'a'+('A'-'a')*turn&&game.board[peicePos.row+1][peicePos.column+1]<'z'+('A'-'a')*turn))
                {
                    gameState temp=game;
                    temp.boardMatrix&=(~((unsigned long long )1<<((8*peicePos.row)+peicePos.column)));
                    if((seeCheckTurn(temp,1-temp.turn)&(1<<4))==0)
                    {

                        return 0;
                    }
                }
            }
        }
        else if(peice=='h'+('A'-'a')*turn)
        {
            if(peicePos.row>0&&peicePos.column>1)
            {
                if(!(game.board[peicePos.row-1][peicePos.column-2]>'a'+('A'-'a')*turn&&game.board[peicePos.row-1][peicePos.column-2]<'z'+('A'-'a')*turn))
                {
                    gameState temp=game;
                    temp.boardMatrix&=(~((unsigned long long )1<<((8*peicePos.row)+peicePos.column)));
                    if((seeCheckTurn(temp,1-temp.turn)&(1<<4))==0)
                    {

                        return 0;
                    }
                }
            }
            if(peicePos.row>0&&peicePos.column<6)
            {
                if(!(game.board[peicePos.row-1][peicePos.column+2]>'a'+('A'-'a')*turn&&game.board[peicePos.row-1][peicePos.column+2]<'z'+('A'-'a')*turn))
                {
                    gameState temp=game;
                    temp.boardMatrix&=(~((unsigned long long )1<<((8*peicePos.row)+peicePos.column)));
                    if((seeCheckTurn(temp,1-temp.turn)&(1<<4))==0)
                    {

                        return 0;
                    }
                }
            }
            if(peicePos.row>1&&peicePos.column>0)
            {
                if(!(game.board[peicePos.row-2][peicePos.column-1]>'a'+('A'-'a')*turn&&game.board[peicePos.row-2][peicePos.column-1]<'z'+('A'-'a')*turn))
                {
                    gameState temp=game;
                    temp.boardMatrix&=(~((unsigned long long )1<<((8*peicePos.row)+peicePos.column)));
                    if((seeCheckTurn(temp,1-temp.turn)&(1<<4))==0)
                    {

                        return 0;
                    }
                }
            }
            if(peicePos.row>1&&peicePos.column<7)
            {
                if(!(game.board[peicePos.row-2][peicePos.column+1]>'a'+('A'-'a')*turn&&game.board[peicePos.row-2][peicePos.column+1]<'z'+('A'-'a')*turn))
                {
                    gameState temp=game;
                    temp.boardMatrix&=(~((unsigned long long )1<<((8*peicePos.row)+peicePos.column)));
                    if((seeCheckTurn(temp,1-temp.turn)&(1<<4))==0)
                    {

                        return 0;
                    }
                }
            }
            if(peicePos.row<7&&peicePos.column>1)
            {
                if(!(game.board[peicePos.row+1][peicePos.column-2]>'a'+('A'-'a')*turn&&game.board[peicePos.row+1][peicePos.column-2]<'z'+('A'-'a')*turn))
                {
                    gameState temp=game;
                    temp.boardMatrix&=(~((unsigned long long )1<<((8*peicePos.row)+peicePos.column)));
                    if((seeCheckTurn(temp,1-temp.turn)&(1<<4))==0)
                    {

                        return 0;
                    }
                }
            }
            if(peicePos.row<7&&peicePos.column<6)
            {
                if(!(game.board[peicePos.row+1][peicePos.column+2]>'a'+('A'-'a')*turn&&game.board[peicePos.row+1][peicePos.column+2]<'z'+('A'-'a')*turn))
                {
                    gameState temp=game;
                    temp.boardMatrix&=(~((unsigned long long )1<<((8*peicePos.row)+peicePos.column)));
                    if((seeCheckTurn(temp,1-temp.turn)&(1<<4))==0)
                    {

                        return 0;
                    }
                }
            }
            if(peicePos.row<6&&peicePos.column>0)
            {
                if(!(game.board[peicePos.row+2][peicePos.column-1]>'a'+('A'-'a')*turn&&game.board[peicePos.row+2][peicePos.column-1]<'z'+('A'-'a')*turn))
                {
                    gameState temp=game;
                    temp.boardMatrix&=(~((unsigned long long )1<<((8*peicePos.row)+peicePos.column)));
                    if((seeCheckTurn(temp,1-temp.turn)&(1<<4))==0)
                    {

                        return 0;
                    }
                }
            }
            if(peicePos.row<6&&peicePos.column<7)
            {
                if(!(game.board[peicePos.row+2][peicePos.column+1]>'a'+('A'-'a')*turn&&game.board[peicePos.row+2][peicePos.column+1]<'z'+('A'-'a')*turn))
                {
                    gameState temp=game;
                    temp.boardMatrix&=(~((unsigned long long )1<<((8*peicePos.row)+peicePos.column)));
                    if((seeCheckTurn(temp,1-temp.turn)&(1<<4))==0)
                    {

                        return 0;
                    }
                }
            }
        }
        else if(peice=='p'+('A'-'a')*turn)
        {
            if(game.board[peicePos.row+1-2*turn][peicePos.column]=='.'||game.board[peicePos.row+1-2*turn][peicePos.column]=='_')
            {
                gameState temp=game;
                temp.boardMatrix&=(~((unsigned long long )1<<((8*peicePos.row)+peicePos.column)));
                if((seeCheckTurn(temp,1-temp.turn)&(1<<4))==0)
                {

                    return 0;
                }
            }
            if(peicePos.column>0)
                if(game.board[peicePos.row+1-2*turn][peicePos.column-1]>'A'+('a'-'A')*turn&&game.board[peicePos.row+1-2*turn][peicePos.column-1]<'Z'+('a'-'A')*turn)
                {
                    gameState temp=game;
                    temp.boardMatrix&=(~((unsigned long long )1<<((8*peicePos.row)+peicePos.column)));
                    if((seeCheckTurn(temp,1-temp.turn)&(1<<4))==0)
                    {

                        return 0;
                    }
                }
            if(peicePos.column<7)
                if(game.board[peicePos.row+1-2*turn][peicePos.column+1]>'A'+('a'-'A')*turn&&game.board[peicePos.row+1-2*turn][peicePos.column+1]<'Z'+('a'-'A')*turn)
                {
                    gameState temp=game;
                    temp.boardMatrix&=(~((unsigned long long )1<<((8*peicePos.row)+peicePos.column)));
                    if((seeCheckTurn(temp,1-temp.turn)&(1<<4))==0)
                    {

                        return 0;
                    }
                }
        }
    }
    return 1;
}
//function for check detection
int seeCheckTurn(gameState game,int turn)
{
    pair end,initial;
    char inp[5];
    int i,j,isCheck=0;
    game.turn=turn;
    int x=0;
    for(i=0; i<16; i++)//looping pieces
    {
        initial =game.Player[turn].positions[i];
        if(initial.row<0)continue;
        end=game.kingPos[1-turn];
        int y=kingThreat(game,initial,end);
        x|=y;
        if((y&(1<<4))>0)//kings cell is in danger
        {
            isCheck++;
            Threat=initial;
        }
    }
//int    y=x;
    /* for(i=0;i<3;i++)
     {
         for(j=0;j<3;j++)
         {
             printf("%d",y%2);
             y/=2;
         }
         printf("\n");
     }*/
    if(x==(1<<9)-1&&isCheck>1)
        x|=1<<9;
    return x;
}
#endif // CHESS_H_INCLUDED
