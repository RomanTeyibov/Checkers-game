#include <iostream>
#include <string>
#include <stdio.h>
#include <stdlib.h>
#include <time.h>
/**
* Name: Roman Teyibov
* Student ID: 100269695
* Course: CPSC 1160
* Section - 01
* Purpose of the program:
* Write a program to create a board game.
* Your program should implement the following:
* - It can create and display the board.
* - It can get moves from the human player and place the pieces on the squares. 
*   If the human player enters an invalid move, your program must ask the player 
*   to enter a move again. Remember to treat all the pieces as regular pieces.
* -	It can automatically generate moves for the computer player and placethe pieces on the squares.
* -	If there is a jump over a piece, your program can remove that piece from the board.
* -	The two players play from a single computer and from a single interface.
* -	Create clear instructions to tell the user how to enter a move and ask the user 
*   if he/she wants to continue.
*
* As a bonus, I also implemented kings. As for AI, its priority is to eat a player piece.
* If there are no player pieces it can eat, it makes a random a move.
* You win the game if you ate all the enemy pieces or if the enemy cannot make any more moves.
* Likewise, you lose the game if the AI ate all your pieces or you cannot make any more moves.
*/
using namespace std;

string chooseColor();
void drawBoard(char pieces[][8]);
void fillArray(char pieces[][8], string color);
bool askToContinue();
string getPlayerMove();
bool makePlayerMove(char pieces[][8], string move, string color);
bool eatPlayer(char pieces[][8], string color);
void getPossibleMoves(char pieces[][8], string color, int possibleMoves[64]);
void makeEnemyMove(char pieces[][8], string color, int possibleMoves[64]);
bool checkIfEnemyHasMoves(int possibleMoves[64]);
bool checkIfPlayerHasMoves(char pieces[][8], string color);

int enemyPieces = 16;
int playerPieces = 16;

int main()
{
    string color = "";
    bool continuePlaying = true;
    int possibleMoves[64];
    while(color != "b" && color != "w")
    {
        color = chooseColor();
    }
    char pieces[8][8];
    fillArray(pieces, color);
    while(continuePlaying)
    {
        drawBoard(pieces);
        bool moveSuccessful = false;
        continuePlaying = askToContinue();
        if(!continuePlaying)
        {
            break;
        }
        if(checkIfPlayerHasMoves(pieces, color) == false)
        {
            cout << "You can't make any more moves. You lose!" << endl;
            break;
        }
        while(!moveSuccessful)
        {
            string move = getPlayerMove();
            moveSuccessful = makePlayerMove(pieces, move, color);
            if(enemyPieces == 0)
            {
                cout << "You win!" << endl;
                break;
            }
        }
        bool playerEaten;
        playerEaten = eatPlayer(pieces, color);
        if(playerPieces == 0)
        {
            cout << "You lose!" << endl;
            break;
        }
        if(!playerEaten)
        {
            getPossibleMoves(pieces, color, possibleMoves);
            bool hasMoves = checkIfEnemyHasMoves(possibleMoves);
            if(hasMoves)
            {
                makeEnemyMove(pieces, color, possibleMoves);
            }
            else
            {
                cout << "The enemy can't make any moves. You win!" << endl;
                break;
            }
        }
    }

    return 0;
}

//Draws the board with all the peices.
void drawBoard(char pieces[][8])
{
    int size = 8;
    cout << "   a   b   c   d   e   f   g   h  " << endl;
    cout << " |-------------------------------|" << endl;
    for(int i = 0; i < size; i++)
    {
        cout << i + 1 << "| ";
        for(int j = 0; j < size; j++)
        {
            cout << pieces[i][j] << " | ";
        }
        cout << endl;
        if(i != 7)
        {
            cout << " |---+---+---+---+---+---+---+---|" << endl;
        }
        else
        {
            cout << " |-------------------------------|" << endl;
        }
    }
}

//Prompts the user to choose the color he/she wants to play.
string chooseColor()
{
    string color = "";
    cout << "Choose the color you want to play (b/w): ";
    cin >> color;
    if(color != "b" && color != "w")
    {
        cout << "You didn't choose a valid color." << endl;
    }
    else if(color == "b")
    {
        cout << "You play black. Computer plays white." << endl;        
    }
    else if(color == "w")
    {
        cout << "You play white. Computer plays black." << endl;
    }
    return color;
}

//Asks if the user wants to continue or not. The game terminates if
//the answer is n (no).
bool askToContinue()
{
    string answer = "";
    while(answer != "y" && answer != "n")
    {
        cout << "Do you want to continue? (y/n): ";
        cin >> answer;
        if(answer == "y")
        {
            return true;
        }
        else if(answer == "n")
        {
            return false;
        }
        else
        {
            cout << "You didn't enter a valid answer." << endl;
        }
    }
}

//Fills the board with pieces. 
//Runs only once after the the player has chosen the color.
void fillArray(char pieces[][8], string color)
{
    int size = 8;
    if(color == "w")
    {
        for(int i = 0; i < 2; i++)
        {
            for(int j = 0; j < size; j++)
            {
                pieces[i][j] = 'b';
            }
        }
        for(int i = 6; i < size; i++)
        {
            for(int j = 0; j < size; j++)
            {
                pieces[i][j] = 'w';
            }
        }
    }
    else if(color == "b")
    {
        for(int i = 0; i < 2; i++)
        {
            for(int j = 0; j < size; j++)
            {
                pieces[i][j] = 'w';
            }
        }
        for(int i = 6; i < size; i++)
        {
            for(int j = 0; j < size; j++)
            {
                pieces[i][j] = 'b';
            }
        }
    }
    for(int i = 2; i < size - 2; i++)
    {
        for(int j = 0; j < size; j++)
        {
            pieces[i][j] = ' ';
        }
    }
}

//Prompts the user to enter the move.
//Keeps prompting for input until the given move is in the correct format.
string getPlayerMove()
{
    string move = "";
    string move1 = "";
    string move2 = "";
    bool validMove = false;
    while(!validMove)
    {
        cout << "Enter the from-square followed by the to-square (e.g. 7a 6b): ";
        cin >> move1 >> move2;
        move = move1 + " " + move2;
        if(move.length() == 5 && move[2] == ' ' && move[0] >= '1' && move[0] <= '8' 
            && move[3] >= '1' && move[3] <= '8' && move[1] >= 'a' && move[1] <= 'h' 
            && move[4] >= 'a' && move[4] <= 'h')
        {
            validMove = true;
            return move;
        }
        else
        {
            cout << "You didn't enter a move in the valid format." << endl;
        }
    }
}

//Makes the move the player has chosen.
//If the move is not valid, returns false and notifies the player.
bool makePlayerMove(char pieces[][8], string move, string color)
{
    bool validMove = false;
    int fromIndex1 = move[0] - '0' - 1;
    int fromIndex2 = move[1] - 'a';
    int toIndex1 = move[3] - '0' - 1;
    int toIndex2 = move[4] - 'a';
    char playerKing;
    char temp;
    if(color[0] == 'w')
    {
        playerKing = 'W';
    }
    else
    {
        playerKing = 'B';
    }

    char enemy;
    char enemyKing;
    if(color[0] == 'w')
    {
        enemy = 'b';
        enemyKing = 'B';
    }
    else
    {
        enemy = 'w';
        enemyKing = 'W';
    }

    if((pieces[fromIndex1][fromIndex2] == color[0] || pieces[fromIndex1][fromIndex2] == playerKing) 
        && pieces[toIndex1][toIndex2] == ' ')
    {
        if(fromIndex1 > toIndex1 && fromIndex2 != toIndex2)
        {
            if(toIndex1 - fromIndex1 == -2 && toIndex2 - fromIndex2 == 2)
            {
                if((pieces[fromIndex1 - 1][fromIndex2 + 1] == enemy || pieces[fromIndex1 - 1][fromIndex2 + 1] == enemyKing) 
                    && pieces[toIndex1][toIndex2] == ' ')
                {
                    validMove = true;
                    temp = pieces[fromIndex1][fromIndex2];
                    pieces[fromIndex1][fromIndex2] = ' ';
                    pieces[toIndex1][toIndex2] = temp;
                    if(toIndex1 == 0)
                    {
                        pieces[toIndex1][toIndex2] = playerKing;
                    }
                    pieces[fromIndex1 - 1][toIndex2 - 1] = ' ';
                    cout << "Move successful!" << endl;
                    enemyPieces -= 1;
                    return true;
                }
            }
            else if(toIndex1 - fromIndex1 == -2 && toIndex2 - fromIndex2 == -2)
            {
                if((pieces[fromIndex1 - 1][fromIndex2 - 1] == enemy || pieces[fromIndex1 - 1][fromIndex2 - 1] == enemyKing) 
                    && pieces[toIndex1][toIndex2] == ' ')
                {
                    validMove = true;
                    temp = pieces[fromIndex1][fromIndex2];
                    pieces[fromIndex1][fromIndex2] = ' ';
                    pieces[toIndex1][toIndex2] = temp;
                    if(toIndex1 == 0)
                    {
                        pieces[toIndex1][toIndex2] = playerKing;
                    }
                    pieces[fromIndex1 - 1][toIndex2 + 1] = ' ';
                    cout << "Move successful!" << endl;
                    enemyPieces -= 1;
                    return true;
                }
            }
            else if(toIndex1 - fromIndex1 == -1 && (toIndex2 - fromIndex2 == 1 || toIndex2 - fromIndex2 == -1))
            {
                validMove = true;
                temp = pieces[fromIndex1][fromIndex2];
                pieces[fromIndex1][fromIndex2] = ' ';
                pieces[toIndex1][toIndex2] = temp;
                if(toIndex1 == 0)
                {
                    pieces[toIndex1][toIndex2] = playerKing;
                }
                cout << "Move successful!" << endl;
                return true;
            }
        }
        //For kings only
        if(pieces[fromIndex1][fromIndex2] == playerKing && fromIndex1 < toIndex1 && fromIndex2 != toIndex2)
        {
            if(toIndex1 - fromIndex1 == 2 && toIndex2 - fromIndex2 == 2)
            {
                if((pieces[fromIndex1 + 1][fromIndex2 + 1] == enemy || pieces[fromIndex1 + 1][fromIndex2 + 1] == enemyKing) 
                    && pieces[toIndex1][toIndex2] == ' ')
                {
                    validMove = true;
                    pieces[fromIndex1][fromIndex2] = ' ';
                    pieces[fromIndex1 + 1][fromIndex2 + 1] = ' ';
                    pieces[toIndex1][toIndex2] = playerKing;
                    cout << "Move successful!" << endl;
                    enemyPieces -= 1;
                    return true;
                }
            }
            else if(toIndex1 - fromIndex1 == 2 && toIndex2 - fromIndex2 == -2)
            {
                if((pieces[fromIndex1 + 1][fromIndex2 - 1] == enemy || pieces[fromIndex1 + 1][fromIndex2 - 1] == enemyKing) 
                    && pieces[toIndex1][toIndex2] == ' ')
                {
                    validMove = true;
                    pieces[fromIndex1][fromIndex2] = ' ';
                    pieces[fromIndex1 + 1][fromIndex2 - 1] = ' ';
                    pieces[toIndex1][toIndex2] = playerKing;
                    cout << "Move successful!" << endl;
                    enemyPieces -= 1;
                    return true;
                }
            }
            else if(toIndex1 - fromIndex1 == 1 && (toIndex2 - fromIndex2 == 1 || toIndex2 - fromIndex2 == -1))
            {
                validMove = true;
                pieces[fromIndex1][fromIndex2] = ' ';
                pieces[toIndex1][toIndex2] = playerKing;
                cout << "Move successful!" << endl;
                return true;
            }
        }
    }

    cout << "The selected move is not valid. Please try again." << endl;
    return false;
}

//Checks if the enemy can eat a player piece and does so if it can.
bool eatPlayer(char pieces[][8], string color)
{
    int size = 8;
    char enemy;
    char playerKing;
    char enemyKing;
    char temp;
    if(color[0] == 'w')
    {
        enemy = 'b';
        enemyKing = 'B';
        playerKing = 'W';
    }
    else
    {
        enemy = 'w';
        enemyKing = 'W';
        playerKing = 'B';
    }

    for(int i = 0; i < size; i++)
    {
        for(int j = 0; j < size; j++)
        {
            if(pieces[i][j] == enemy || pieces[i][j] == enemyKing)
            {
                if(j == 0 || j == 1)
                {
                    if((pieces[i + 1][j + 1] == color[0] || pieces[i + 1][j + 1] == playerKing) && pieces[i + 2][j + 2] == ' ')
                    {
                        temp = pieces[i][j];
                        pieces[i][j] = ' ';
                        pieces[i + 1][j + 1] = ' ';
                        pieces[i + 2][j + 2] = temp;
                        playerPieces -= 1;
                        return true;
                    }
                }
                else if(j == 7 || j == 6)
                {
                    if((pieces[i + 1][j - 1] == color[0] || pieces[i + 1][j - 1] == playerKing) && pieces[i + 2][j - 2] == ' ')
                    {
                        temp = pieces[i][j];
                        pieces[i][j] = ' ';
                        pieces[i + 1][j - 1] = ' ';
                        pieces[i + 2][j - 2] = temp;
                        playerPieces -= 1;
                        return true;
                    }
                }
                else if(j > 1 && j < 6)
                {
                    if((pieces[i + 1][j + 1] == color[0] || pieces[i + 1][j + 1] == playerKing) && pieces[i + 2][j + 2] == ' ')
                    {
                        temp = pieces[i][j];
                        pieces[i][j] = ' ';
                        pieces[i + 1][j + 1] = ' ';
                        pieces[i + 2][j + 2] = temp;
                        playerPieces -= 1;
                        return true;
                    }
                    else if((pieces[i + 1][j - 1] == color[0] || pieces[i + 1][j - 1] == playerKing) && pieces[i + 2][j - 2] == ' ')
                    {
                        temp = pieces[i][j];
                        pieces[i][j] = ' ';
                        pieces[i + 1][j - 1] = ' ';
                        pieces[i + 2][j - 2] = temp;
                        playerPieces -= 1;
                        return true;
                    }
                }
            }
            //For kings only
            if(pieces[i][j] == enemyKing)
            {
                if(j == 0)
                {
                    if((pieces[i - 1][j + 1] == color[0] || pieces[i - 1][j + 1] == playerKing) && pieces[i - 2][j + 2] == ' ')
                    {
                        pieces[i][j] = ' ';
                        pieces[i - 1][j + 1] = ' ';
                        pieces[i - 2][j + 2] = enemyKing;
                        playerPieces -= 1;
                        return true;
                    }
                }
                else if(j == 7)
                {
                    if((pieces[i - 1][j - 1] == color[0] || pieces[i - 1][j - 1] == playerKing) && pieces[i - 2][j - 2] == ' ')
                    {
                        pieces[i][j] = ' ';
                        pieces[i - 1][j - 1] = ' ';
                        pieces[i + 2][j - 2] = enemyKing;
                        playerPieces -= 1;
                        return true;
                    }
                }
                else if(j > 1 && j < 6)
                {
                    if((pieces[i - 1][j + 1] == color[0] || pieces[i - 1][j + 1] == playerKing) && pieces[i - 2][j + 2] == ' ')
                    {
                        pieces[i][j] = ' ';
                        pieces[i - 1][j + 1] = ' ';
                        pieces[i - 2][j + 2] = enemyKing;
                        playerPieces -= 1;
                        return true;
                    }
                    else if((pieces[i - 1][j - 1] == color[0] || pieces[i - 1][j - 1] == playerKing) && pieces[i - 2][j - 2] == ' ')
                    {
                        pieces[i][j] = ' ';
                        pieces[i - 1][j - 1] = ' ';
                        pieces[i + 2][j - 2] = enemyKing;
                        playerPieces -= 1;
                        return true;
                    }
                }
            }
        }
    }
    return false;
}

//Gets all the possible moves the enemy can make. Doesn't include any moves
//that involve eating the player. 
//This function is not executed if the enemy ate a player piece this turn.
void getPossibleMoves(char pieces[][8], string color, int possibleMoves[64])
{
    int size = 8;
    int size2 = 64;
    int index = 0;
    char enemy;
    char enemyKing;
    if(color[0] == 'w')
    {
        enemy = 'b';
        enemyKing = 'B';
    }
    else
    {
        enemy = 'w';
        enemyKing = 'W';
    }

    for(int i = 0; i < size2; i++)
    {
        possibleMoves[i] = -1;
    }

    for(int i = 0; i < size; i++)
    {
        for(int j = 0; j < size; j++)
        {
            if(pieces[i][j] == enemy || pieces[i][j] == enemyKing)
            {
                if(j == 0)
                {
                    if(pieces[i + 1][j + 1] == ' ')
                    {
                        possibleMoves[index] = (i + 1) * 1000 + (j + 1) * 100 + (i + 2) * 10 + (j + 2);
                        index++;
                    }
                }
                if(j == 7)
                {
                    if(pieces[i + 1][j - 1] == ' ')
                    {
                        possibleMoves[index] = (i + 1) * 1000 + (j + 1) * 100 + ((i + 2) * 10 + j);
                        index++;
                    }
                }
                if(j > 0 && j < 7)
                {
                    if(pieces[i + 1][j - 1] == ' ')
                    {
                        possibleMoves[index] = (i + 1) * 1000 + (j + 1) * 100 + ((i + 2) * 10 + j);
                        index++;
                    }
                    if(pieces[i + 1][j + 1] == ' ')
                    {
                        possibleMoves[index] = (i + 1) * 1000 + (j + 1) * 100 + (i + 2) * 10 + (j + 2);
                        index++;
                    }
                }
            }
            //For kings only
            if(pieces[i][j] == enemyKing)
            {
                if(j == 0)
                {
                    if(pieces[i - 1][j + 1] == ' ')
                    {
                        possibleMoves[index] = (i + 1) * 1000 + (j + 1) * 100 + (i + 2) * 10 + (j + 2);
                        index++;
                    }
                }
                if(j == 7)
                {
                    if(pieces[i - 1][j - 1] == ' ')
                    {
                        possibleMoves[index] = (i + 1) * 1000 + (j + 1) * 100 + ((i + 2) * 10 + j);
                        index++;
                    }
                }
                if(j > 0 && j < 7)
                {
                    if(pieces[i - 1][j - 1] == ' ')
                    {
                        possibleMoves[index] = (i + 1) * 1000 + (j + 1) * 100 + ((i + 2) * 10 + j);
                        index++;
                    }
                    if(pieces[i - 1][j + 1] == ' ')
                    {
                        possibleMoves[index] = (i + 1) * 1000 + (j + 1) * 100 + (i + 2) * 10 + (j + 2);
                        index++;
                    }
                }
            }
        }
    }
}

//The AI randomly chooses from the array of possible moves and makes that move.
//Again, this function doesn't run if the enemy ate a player piece this turn.
void makeEnemyMove(char pieces[][8], string color, int possibleMoves[64])
{
    int size = 8;
    int size2 = 64;
    int numberOfPossibleMoves = 0;
    for(int i = 0; i < size2; i++)
    {
        if(possibleMoves[i] != -1)
        {
            numberOfPossibleMoves += 1;
        }
    }

    srand(time(0));
    int chosenMove = rand() % (numberOfPossibleMoves + 1);
    int move = possibleMoves[chosenMove];
    int toIndex2 = (move % 10) - 1;
    move /= 10;
    int toIndex1 = (move % 10) - 1;
    move /= 10;
    int fromIndex2 = (move % 10) - 1;
    move /= 10;
    int fromIndex1 = move - 1;

    char enemy;
    char enemyKing;
    char temp;
    if(color[0] == 'w')
    {
        enemy = 'b';
        enemyKing = 'B';
    }
    else
    {
        enemy = 'w';
        enemyKing = 'W';
    }

    temp = pieces[fromIndex1][fromIndex2];
    pieces[fromIndex1][fromIndex2] = ' ';
    pieces[toIndex1][toIndex2] = temp;

    if(toIndex1 == 7 && temp != enemyKing)
    {
        pieces[toIndex1][toIndex2] = enemyKing;
    }
}

//Checks if enemy has any more moves this turn by checking if the array of possible moves.
//Again, this function doesn't run if the enemy ate a player piece this turn.
bool checkIfEnemyHasMoves(int possibleMoves[64])
{
    int size = 64;
    for(int i = 0; i < size; i++)
    {
        if(possibleMoves[i] != -1)
        {
            return true;
        }
    }
    return false;
}

//Checks if player has any more available moves. 
//Returns true as soon as its finds a possible move.
bool checkIfPlayerHasMoves(char pieces[][8], string color)
{
    int size = 8;
    char enemy;
    char playerKing;
    if(color[0] == 'b')
    {
        playerKing = 'B';
    }
    else
    {
        playerKing = 'W';
    }

    if(color[0] == 'w')
    {
        enemy = 'b';
    }
    else
    {
        enemy = 'w';
    }
    for(int i = 0; i < size; i++)
    {
        for(int j = 0; j < size; j++)
        {
            if(pieces[i][j] == color[0] || pieces[i][j] == playerKing)
            {
                if(j == 0)
                {
                    if(pieces[i - 1][j + 1] == ' ')
                    {
                        return true;
                    }
                    else if(pieces[i - 1][j + 1] == enemy && pieces[i - 2][j + 2] == ' ')
                    {
                        return true;
                    }
                }
                if(j == 7)
                {
                    if(pieces[i - 1][j - 1] == ' ')
                    {
                        return true;
                    }
                    else if(pieces[i - 1][j - 1] == enemy && pieces[i - 2][j - 2] == ' ')
                    {
                        return true;
                    }
                }
                else if(j == 1)
                {
                    if(pieces[i - 1][j - 1] == ' ')
                    {
                        return true;
                    }
                    else if(pieces[i - 1][j + 1] == ' ' || (pieces[i - 1][j + 1] == enemy && pieces[i - 2][j + 2] == ' '))
                    {
                        return true;
                    }
                }
                else if(j == 6)
                {
                    if(pieces[i - 1][j + 1] == ' ')
                    {
                        return true;
                    }
                    else if(pieces[i - 1][j - 1] == ' ' || (pieces[i - 1][j - 1] == enemy && pieces[i - 2][j - 2] == ' '))
                    {
                        return true;
                    }
                }
                else if(j > 1 && j < 6)
                {
                    if(pieces[i - 1][j - 1] == ' ' || (pieces[i - 1][j - 1] == enemy && pieces[i - 2][j - 2] == ' '))
                    {
                        return true;
                    }
                    else if(pieces[i - 1][j + 1] == ' ' || (pieces[i - 1][j + 1] == enemy && pieces[i - 2][j + 2] == ' '))
                    {
                        return true;
                    }
                }
            }
            //For kings only
            if(pieces[i][j] == playerKing)
            {
                if(j == 0)
                {
                    if(pieces[i + 1][j + 1] == ' ')
                    {
                        return true;
                    }
                    else if(pieces[i + 1][j + 1] == enemy && pieces[i + 2][j + 2] == ' ')
                    {
                        return true;
                    }
                }
                if(j == 7)
                {
                    if(pieces[i + 1][j - 1] == ' ')
                    {
                        return true;
                    }
                    else if(pieces[i + 1][j - 1] == enemy && pieces[i + 2][j - 2] == ' ')
                    {
                        return true;
                    }
                }
                else if(j == 1)
                {
                    if(pieces[i + 1][j - 1] == ' ')
                    {
                        return true;
                    }
                    else if(pieces[i + 1][j + 1] == ' ' || (pieces[i + 1][j + 1] == enemy && pieces[i + 2][j + 2] == ' '))
                    {
                        return true;
                    }
                }
                else if(j == 6)
                {
                    if(pieces[i + 1][j + 1] == ' ')
                    {
                        return true;
                    }
                    else if(pieces[i + 1][j - 1] == ' ' || (pieces[i + 1][j - 1] == enemy && pieces[i + 2][j - 2] == ' '))
                    {
                        return true;
                    }
                }
                else if(j > 1 && j < 6)
                {
                    if(pieces[i + 1][j - 1] == ' ' || (pieces[i + 1][j - 1] == enemy && pieces[i + 2][j - 2] == ' '))
                    {
                        return true;
                    }
                    else if(pieces[i + 1][j + 1] == ' ' || (pieces[i + 1][j + 1] == enemy && pieces[i + 2][j + 2] == ' '))
                    {
                        return true;
                    }
                }
            }
        }
    }
    return false;
}