//
//  main.c
//  Five In A Row
//
//  Created by 王浩宇 on 2018/9/21.
//  Copyright © 2018 UCAS Developers. All rights reserved.
//

#include <stdio.h>
#include <stdlib.h>
#include <locale.h>

#define NOWIN    0
#define WHITEWIN 1
#define BLACKWIN 2
#define NOVIOLATE 0
#define VIOLATE 1
#define VIOLATE33 3
#define VIOLATE44 4

void init(char white[112][3], char black[112][3]);
void initBoard(wchar_t * board);
void setBoard(wchar_t * board, char position[112][3], wchar_t piece);
void printBoard(wchar_t * board);
void refreshBoard(wchar_t * board, char white[112][3], char black[112][3]);
int checkWin(wchar_t * board);
int checkWhoWins(wchar_t * board, int WHOWINS, wchar_t piece);
int checkForbiddenMoves(wchar_t * board, char lastPiece[3]);
int checkThreeThree(wchar_t * board, char lastPiece[3]);
int checkFourFour(wchar_t * board, char lastPiece[3]);
wchar_t * getBoardThings(wchar_t * board, char lastPiece[3], int count, int position, int direction);
int transformCoordinate(char position[3]);
int min(int a, int b);

int main(void) {
    setlocale(LC_ALL,"zh_CN.UTF-8");
    int i;
    wchar_t board[225];    //used to store every wchar_t of the 15x15 board
    
    /*
     the 112 1 dimensional arrays represent 112 pieces
     each 1 dimensional array store a coordinate like a1, h12, or c10
     */
    char white[112][3], black[112][3];
    init(white, black);
    refreshBoard(board, white, black);
    
    for (i = 0; i < 112; i++) {
        printf("Please enter a position(like h7, now you are the black):");
        scanf("%s", black[i]);
        refreshBoard(board, white, black);
        if (checkWin(board) == BLACKWIN) { printf("Black Wins!\n"); break; }
        if (checkForbiddenMoves(board, black[i]) == VIOLATE33) { printf("White Wins! Black has made a 3,3 forbidden move.\n"); break; }
        if (checkForbiddenMoves(board, black[i]) == VIOLATE44) { printf("White Wins! Black has made a 4,4 forbidden move.\n"); break; }
        
        printf("Please enter a position(like h7, now you are the white):");
        scanf("%s", white[i]);
        refreshBoard(board, white, black);
        if (checkWin(board) == WHITEWIN) { printf("White Wins!\n"); break; }
    }
    
    return 0;
}

/*
 this function is used to initialize white and black pieces
 make the coordinate of each piece become 00'\0'
 whatever the third one is, if you input a coordinate like h7, it will automatically be '\0'
 */
void init(char white[112][3], char black[112][3]) {
    int i, j;
    
    for (i = 0; i < 112; i++) {
        for (j = 0; j < 3; j++) {
            white[i][j] = 0;
            if (j == 2) { white[i][j] = '\0'; }
        }
    }
    
    for (i = 0; i < 112; i++) {
        for (j = 0; j < 3; j++) {
            black[i][j] = 0;
            if (j == 2) { black[i][j] = '\0'; }
        }
    }
}

/*
 this function is used to refresh the chess board
 when you input a new coordinate, the board will be refreshed once
 the command "clear" is for macOS/Linux, for Windows, it should be "cls"
 */
void refreshBoard(wchar_t * board, char white[112][3], char black[112][3]) {
    system("clear");
    printf("Welcome to the Five In A Row game!\n\n\n");
    initBoard(board);
    setBoard(board, white, L'○');
    setBoard(board, black, L'●');
    printBoard(board);
}

/*
 this function is used to initialize the chess board
 remove all pieces on the board and initialize it
 */
void initBoard(wchar_t * board) {
    int i;
    for (i = 0; i < 225; i++) {
        if (i == 0) { board[i] = L'┌'; }
        else if (i == 14) { board[i] = L'┐'; }
        else if (i == 210) { board[i] = L'└'; }
        else if (i == 224) { board[i] = L'┘'; }
        else if ((i > 0) && (i < 14)) { board[i] = L'┬'; }
        else if ((i > 210) && (i < 224)) { board[i] = L'┴'; }
        else if (i % 15 == 0) { board[i] = L'├'; }
        else if ((i + 1) % 15 == 0) { board[i] = L'┤'; }
        else { board[i] = L'┼'; }
    }
}

/*
 this function is used to put pieces on the board
 it receives coordinates of the pieces of a specific color
 and use the function "transformCoordinate" to get the proper index of the board array
 */
void setBoard(wchar_t * board, char position[112][3], wchar_t piece) {
    int i;
    for (i = 0; i < 112; i++) {
        if (!position[i][0]) { break; }
        board[transformCoordinate(position[i])] = piece;
    }
}

/*
 this function is used to print out the board
 */
void printBoard(wchar_t * board) {
    int i, j;
    
    for (i = 15; i > 0; i--) {
        printf("%2d  ", i);
        for (j = (15 - i) * 15; j < (15 - i) * 15 + 15; j++) {
            printf("%lc", board[j]);
        }
        printf("\n");
    }
    
    printf("    A B C D E F G H I J K L M N O\n\n");
}

/*
 this function is used to check whether black or white has won the game
 */
int checkWin(wchar_t * board) {
    if (checkWhoWins(board, WHITEWIN, L'○')) { return WHITEWIN; }
    if (checkWhoWins(board, BLACKWIN, L'●')) { return BLACKWIN; }
    return NOWIN;
}

/*
 this function is used to check if a specific color has won the game
 according to the board array received
 its algorithm is to check whether each of the 4 directions has 5 adjoining pieces with same color
 */
int checkWhoWins(wchar_t * board, int WHOWINS, wchar_t piece) {
    int i, j;
    /* check horizontally */
    for (i = 0; i < 15; i++) {
        for (j = 0; j < 11; j++) {
            if ((board[i * 15 + j] == piece) && (board[i * 15 + j + 1] == piece) && (board[i * 15 + j + 2] == piece) && (board[i * 15 + j + 3] == piece) && (board[i * 15 + j + 4] == piece)) { return WHOWINS; }
        }
    }
    
    /* check vertically */
    for (j = 0; j < 15; j++) {
        for (i = 0; i < 11; i++) {
            if ((board[j + i * 15] == piece) && (board[j + (i + 1) * 15] == piece) && (board[j + (i + 2) * 15] == piece) && (board[j + (i + 3) * 15] == piece) && (board[j + (i + 4) * 15] == piece)) { return WHOWINS; }
        }
    }
    
    /* check from upper left to lower right */
    for (i = 4; i < 15; i++) {  //the first part, to the diagonal of the chess board
        for (j = 0; j <= (i - 4); j++) {
            if ((board[i + j * 14] == piece) && (board[(i + j * 14) + 14] == piece) && (board[(i + j * 14) + 2 * 14] == piece) && (board[(i + j * 14) + 3 * 14] == piece) && (board[(i + j * 14) + 4 * 14] == piece)) { return WHOWINS; }
        }
    }
    for (i = 0; i < 10; i++) {  //the second part, from the diagonal to the lower right
        for (j = 0; j <= (9 - i); j++) {
            if ((board[29 + i * 15 + j * 14] == piece) && (board[29 + i * 15 + j * 14 + 14] == piece) && (board[29 + i * 15 + j * 14 + 2 * 14] == piece) && (board[29 + i * 15 + j * 14 + 3 * 14] == piece) && (board[29 + i * 15 + j * 14 + 4 * 14] == piece)) { return WHOWINS; }
        }
    }
    
    /* check from upper right to lower left */
    for (i = 0; i < 11; i++) {  //the first part, to the diagonal of the chess board
        for (j = 0; j <= i; j++) {
            if ((board[10 - i + j * 16] == piece) && (board[10 - i + j * 16 + 16] == piece) && (board[10 - i + j * 16 + 2 * 16] == piece) && (board[10 - i + j * 16 + 3 * 16] == piece) && (board[10 - i + j * 16 + 4 * 16] == piece)) { return WHOWINS; }
        }
    }
    for (i = 0; i < 10; i++) {  //the second part, from the diagonal to the lower left
        for (j = 0; j <= (9 - i); j++) {
            if ((board[15 * (i + 1) + j * 16] == piece) && (board[15 * (i + 1) + j * 16 + 16] == piece) && (board[15 * (i + 1) + j * 16 + 2 * 16] == piece) && (board[15 * (i + 1) + j * 16 + 3 * 16] == piece) && (board[15 * (i + 1) + j * 16 + 4 * 16] == piece)) { return WHOWINS; }
        }
    }
    
    return NOWIN;
}

/*
 this function is used to check whether the black has made a forbidden move
 */
int checkForbiddenMoves(wchar_t * board, char lastPiece[3]) {
    if (checkThreeThree(board, lastPiece)) { return VIOLATE33; }
    if (checkFourFour(board, lastPiece)) { return VIOLATE44; }
    return NOVIOLATE;
}

/*
 this function is used to check whether the black has made a 3,3 forbidden move
 it needs the coordinate of the last black piece
 about its algorithm, I won't explain it here because it is too complex
 */
int checkThreeThree(wchar_t * board, char lastPiece[3]) {
    int direction;
    int count = 0;
    wchar_t * things;
    wchar_t blackPiece = L'●';
    wchar_t whitePiece = L'○';
    
    for (direction = 0; direction <= 3; direction++) {
        things = getBoardThings(board, lastPiece, 7, 2, direction);
        if ((things[2] == blackPiece) && (things[3] == blackPiece) && (things[4] == blackPiece) && (things[1] != blackPiece) && (things[1] != whitePiece) && (things[5] != blackPiece) && (things[5] != whitePiece) && (things[0] != blackPiece) && (things[6] != blackPiece)) {
            count += 1;
            continue;
        }
        
        things = getBoardThings(board, lastPiece, 7, 4, direction);
        if ((things[2] == blackPiece) && (things[3] == blackPiece) && (things[4] == blackPiece) && (things[1] != blackPiece) && (things[1] != whitePiece) && (things[5] != blackPiece) && (things[5] != whitePiece) && (things[0] != blackPiece) && (things[6] != blackPiece)) {
            count += 1;
            continue;
        }
        
        things = getBoardThings(board, lastPiece, 7, 3, direction);
        if ((things[2] == blackPiece) && (things[3] == blackPiece) && (things[4] == blackPiece) && (things[1] != blackPiece) && (things[1] != whitePiece) && (things[5] != blackPiece) && (things[5] != whitePiece) && (things[0] != blackPiece) && (things[6] != blackPiece)) {
            count += 1;
        }
    }
    
    for (direction = 0; direction <= 3; direction++) {
        things = getBoardThings(board, lastPiece, 6, 1, direction);
        if ((things[0] != blackPiece) && (things[0] != whitePiece) && (things[5] != blackPiece) && (things[5] != whitePiece) && (things[1] == blackPiece) && (things[4] == blackPiece) && (((things[2] == blackPiece) && ((things[3] != blackPiece) && (things[3] != whitePiece))) || (((things[2] != blackPiece) && (things[2] != whitePiece)) && (things[3] == blackPiece)))) {
            count += 1;
            continue;
        }
        
        things = getBoardThings(board, lastPiece, 6, 4, direction);
        if ((things[0] != blackPiece) && (things[0] != whitePiece) && (things[5] != blackPiece) && (things[5] != whitePiece) && (things[1] == blackPiece) && (things[4] == blackPiece) && (((things[2] == blackPiece) && ((things[3] != blackPiece) && (things[3] != whitePiece))) || (((things[2] != blackPiece) && (things[2] != whitePiece)) && (things[3] == blackPiece)))) {
            count += 1;
            continue;
        }
        
        things = getBoardThings(board, lastPiece, 6, 2, direction);
        if ((things[0] != blackPiece) && (things[0] != whitePiece) && (things[5] != blackPiece) && (things[5] != whitePiece) && (things[1] == blackPiece) && (things[4] == blackPiece) && (((things[2] == blackPiece) && ((things[3] != blackPiece) && (things[3] != whitePiece))) || (((things[2] != blackPiece) && (things[2] != whitePiece)) && (things[3] == blackPiece)))) {
            count += 1;
            continue;
        }
        
        things = getBoardThings(board, lastPiece, 6, 3, direction);
        if ((things[0] != blackPiece) && (things[0] != whitePiece) && (things[5] != blackPiece) && (things[5] != whitePiece) && (things[1] == blackPiece) && (things[4] == blackPiece) && (((things[2] == blackPiece) && ((things[3] != blackPiece) && (things[3] != whitePiece))) || (((things[2] != blackPiece) && (things[2] != whitePiece)) && (things[3] == blackPiece)))) {
            count += 1;
        }
    }
    
    if (count >= 2) { return VIOLATE; }
    
    return NOVIOLATE;
}

/*
 this function is used to check whether the black has made a 4,4 forbidden move
 it needs the coordinate of the last black piece
 about its algorithm, I won't explain it here because it is too complex
 */
int checkFourFour(wchar_t * board, char lastPiece[3]) {
    int direction;
    int count = 0;
    wchar_t * things;
    wchar_t blackPiece = L'●';
    wchar_t whitePiece = L'○';
    
    for (direction = 0; direction <= 3; direction++) {
        things = getBoardThings(board, lastPiece, 6, 1, direction);
        if ((things[0] != blackPiece) && (things[0] != whitePiece) && (things[5] != blackPiece) && (things[5] != whitePiece) && (things[1] == blackPiece) && (things[2] == blackPiece) && (things[3] == blackPiece) && (things[4] == blackPiece)) {
            count += 1;
            continue;
        }
        
        things = getBoardThings(board, lastPiece, 6, 4, direction);
        if ((things[0] != blackPiece) && (things[0] != whitePiece) && (things[5] != blackPiece) && (things[5] != whitePiece) && (things[1] == blackPiece) && (things[2] == blackPiece) && (things[3] == blackPiece) && (things[4] == blackPiece)) {
            count += 1;
            continue;
        }
        
        things = getBoardThings(board, lastPiece, 6, 2, direction);
        if ((things[0] != blackPiece) && (things[0] != whitePiece) && (things[5] != blackPiece) && (things[5] != whitePiece) && (things[1] == blackPiece) && (things[2] == blackPiece) && (things[3] == blackPiece) && (things[4] == blackPiece)) {
            count += 1;
            continue;
        }
        
        things = getBoardThings(board, lastPiece, 6, 3, direction);
        if ((things[0] != blackPiece) && (things[0] != whitePiece) && (things[5] != blackPiece) && (things[5] != whitePiece) && (things[1] == blackPiece) && (things[2] == blackPiece) && (things[3] == blackPiece) && (things[4] == blackPiece)) {
            count += 1;
        }
    }
    
    for (direction = 0; direction <= 3; direction++) {
        things = getBoardThings(board, lastPiece, 6, 1, direction);
        if ((things[1] == blackPiece) && (things[2] == blackPiece) && (things[3] == blackPiece) && (things[4] == blackPiece) && ((((things[0] == whitePiece) && ((things[5] != blackPiece) && (things[5] != whitePiece))) || ((things[5] == whitePiece) && ((things[0] != blackPiece) && (things[0] != whitePiece)))))) {
            count += 1;
            continue;
        }
        
        things = getBoardThings(board, lastPiece, 6, 4, direction);
        if ((things[1] == blackPiece) && (things[2] == blackPiece) && (things[3] == blackPiece) && (things[4] == blackPiece) && ((((things[0] == whitePiece) && ((things[5] != blackPiece) && (things[5] != whitePiece))) || ((things[5] == whitePiece) && ((things[0] != blackPiece) && (things[0] != whitePiece)))))) {
            count += 1;
            continue;
        }
        
        things = getBoardThings(board, lastPiece, 6, 2, direction);
        if ((things[1] == blackPiece) && (things[2] == blackPiece) && (things[3] == blackPiece) && (things[4] == blackPiece) && ((((things[0] == whitePiece) && ((things[5] != blackPiece) && (things[5] != whitePiece))) || ((things[5] == whitePiece) && ((things[0] != blackPiece) && (things[0] != whitePiece)))))) {
            count += 1;
            continue;
        }
        
        things = getBoardThings(board, lastPiece, 6, 3, direction);
        if ((things[1] == blackPiece) && (things[2] == blackPiece) && (things[3] == blackPiece) && (things[4] == blackPiece) && ((((things[0] == whitePiece) && ((things[5] != blackPiece) && (things[5] != whitePiece))) || ((things[5] == whitePiece) && ((things[0] != blackPiece) && (things[0] != whitePiece)))))) {
            count += 1;
        }
    }
    
    for (direction = 0; direction <= 3; direction++) {
        things = getBoardThings(board, lastPiece, 5, 0, direction);
        if ((things[0] == blackPiece) && (things[4] == blackPiece) && ((((things[1] != blackPiece) && (things[1] != whitePiece)) && (things[2] == blackPiece) && (things[3] == blackPiece)) || (((things[2] != blackPiece) && (things[2] != whitePiece)) && (things[1] == blackPiece) && (things[3] == blackPiece)) || (((things[3] != blackPiece) && (things[3] != whitePiece)) && (things[1] == blackPiece) && (things[2] == blackPiece)))) {
            count += 1;
        }
        
        things = getBoardThings(board, lastPiece, 5, 4, direction);
        if ((things[0] == blackPiece) && (things[4] == blackPiece) && ((((things[1] != blackPiece) && (things[1] != whitePiece)) && (things[2] == blackPiece) && (things[3] == blackPiece)) || (((things[2] != blackPiece) && (things[2] != whitePiece)) && (things[1] == blackPiece) && (things[3] == blackPiece)) || (((things[3] != blackPiece) && (things[3] != whitePiece)) && (things[1] == blackPiece) && (things[2] == blackPiece)))) {
            count += 1;
        }
        
        things = getBoardThings(board, lastPiece, 5, 1, direction);
        if ((things[0] == blackPiece) && (things[4] == blackPiece) && ((((things[1] != blackPiece) && (things[1] != whitePiece)) && (things[2] == blackPiece) && (things[3] == blackPiece)) || (((things[2] != blackPiece) && (things[2] != whitePiece)) && (things[1] == blackPiece) && (things[3] == blackPiece)) || (((things[3] != blackPiece) && (things[3] != whitePiece)) && (things[1] == blackPiece) && (things[2] == blackPiece)))) {
            count += 1;
        }
        
        things = getBoardThings(board, lastPiece, 5, 3, direction);
        if ((things[0] == blackPiece) && (things[4] == blackPiece) && ((((things[1] != blackPiece) && (things[1] != whitePiece)) && (things[2] == blackPiece) && (things[3] == blackPiece)) || (((things[2] != blackPiece) && (things[2] != whitePiece)) && (things[1] == blackPiece) && (things[3] == blackPiece)) || (((things[3] != blackPiece) && (things[3] != whitePiece)) && (things[1] == blackPiece) && (things[2] == blackPiece)))) {
            count += 1;
        }
        
        things = getBoardThings(board, lastPiece, 5, 2, direction);
        if ((things[0] == blackPiece) && (things[4] == blackPiece) && ((((things[1] != blackPiece) && (things[1] != whitePiece)) && (things[2] == blackPiece) && (things[3] == blackPiece)) || (((things[2] != blackPiece) && (things[2] != whitePiece)) && (things[1] == blackPiece) && (things[3] == blackPiece)) || (((things[3] != blackPiece) && (things[3] != whitePiece)) && (things[1] == blackPiece) && (things[2] == blackPiece)))) {
            count += 1;
        }
    }
    
    if (count >= 2) { return VIOLATE; }
    
    return NOVIOLATE;
}

/*
this function is used to get things on the chess board and output them as an array
what you will get is determined by the coordinate of the last piece, count of elements,
the position of the last piece in the array to output and the direction
*/
/* directions:
   0 means horizontal,
   1 means vertical,
   2 means from upper left to lower right,
   3 means from upper right to lower left
 
   0 position of the array to output is left or top (important!!!)
*/
wchar_t * getBoardThings(wchar_t * board, char lastPiece[3], int count, int position, int direction) {
    wchar_t * things = (wchar_t *)malloc(sizeof(wchar_t) * count);
    wchar_t whitePiece = L'○';
    /* get the proper index of the board array from the coordinate of the last piece */
    int boardPos = transformCoordinate(lastPiece);
    int i;
    if (direction == 0) {
        for (i = 0; i < count; i++) {
            /* entering the "if" branch below means that the array has some elements gone beyond the left edge of the board */
            if (position > (boardPos % 15)) {
                /* entering the "if" branch below means that the specific element 'i' is beyond the left edge of the board */
                if (i < (position - (boardPos % 15))) { things[i] = whitePiece; }
                else { things[i] = board[boardPos + i - position]; }
            }
            /* entering the "else if" branch below means that the array has some elements gone beyond the right edge of the board */
            else if ((count - position - 1) > (14 - (boardPos % 15))) {
                /* entering the "if" branch below means that the specific element 'i' is beyond the right edge of the board */
                if (i >= (position + 15 - (boardPos % 15))) { things[i] = whitePiece; }
                else { things[i] = board[boardPos + i - position]; }
            }
            else { things[i] = board[boardPos + i - position]; }
        }
    }
    else if (direction == 1) {
        for (i = 0; i < count; i++) {
            /* entering the "if" branch below means that the array has some elements gone beyond the top edge of the board */
            if (position > (boardPos / 15)) {
                /* entering the "if" branch below means that the specific element 'i' is beyond the top edge of the board */
                if (i < (position - (boardPos / 15))) { things[i] = whitePiece; }
                else { things[i] = board[boardPos + (i - position) * 15]; }
            }
            /* entering the "else if" branch below means that the array has some elements gone beyond the bottom edge of the board */
            else if ((count - position - 1) > (14 - (boardPos / 15))) {
                /* entering the "if" branch below means that the specific element 'i' is beyond the bottom edge of the board */
                if (i >= (position + 15 - (boardPos / 15))) { things[i] = whitePiece; }
                else { things[i] = board[boardPos + (i - position) * 15]; }
            }
            else { things[i] = board[boardPos + (i - position) * 15]; }
        }
    }
    else if (direction == 2) {
        for (i = 0; i < count; i++) {
            /* entering the "if" branch below means that the array has some elements gone beyond the edge of the board in upper left direction */
            if (position > min((boardPos % 15), (boardPos / 15))) {
                /* entering the "if" branch below means that the specific element 'i' is beyond the edge of the board in upper left direction */
                if (i < (position - min((boardPos % 15), (boardPos / 15)))) { things[i] = whitePiece; }
                else { things[i] = board[boardPos + (i - position) * 16]; }
            }
            /* entering the "else if" branch below means that the array has some elements gone beyond the edge of the board in lower right direction */
            else if ((count - position - 1) > (min((14 - (boardPos % 15)), (14 - (boardPos / 15))))) {
                /* entering the "if" branch below means that the specific element 'i' is beyond the edge of the board in lower right direction */
                if (i >= (position + 1 + min((14 - (boardPos % 15)), (14 - (boardPos / 15))))) { things[i] = whitePiece; }
                else { things[i] = board[boardPos + (i - position) * 16]; }
            }
            else { things[i] = board[boardPos + (i - position) * 16]; }
        }
    }
    else {
        for (i = 0; i < count; i++) {
            /* entering the "if" branch below means that the array has some elements gone beyond the edge of the board in upper right direction */
            if (position > min((14 - (boardPos % 15)), (boardPos / 15))) {
                /* entering the "if" branch below means that the specific element 'i' is beyond the edge of the board in upper right direction */
                if (i < (position - min((14 - (boardPos % 15)), (boardPos / 15)))) { things[i] = whitePiece; }
                else { things[i] = board[boardPos + (i - position) * 14]; }
            }
            /* entering the "else if" branch below means that the array has some elements gone beyond the edge of the board in lower left direction */
            else if ((count - position - 1) > (min((boardPos % 15), (14 - (boardPos / 15))))) {
                /* entering the "if" branch below means that the specific element 'i' is beyond the edge of the board in lower left direction */
                if (i >= (position + 1 + min((boardPos % 15), (14 - (boardPos / 15))))) { things[i] = whitePiece; }
                else { things[i] = board[boardPos + (i - position) * 14]; }
            }
            else { things[i] = board[boardPos + (i - position) * 14]; }
        }
    }
    return things;
}

/*
 this function is used to get the proper index of the board array from the coordinate of a piece
 */
int transformCoordinate(char position[3]) {
    /* entering the "if" branch below means that the coordinate of the piece is x1 or x10 ~ x15 */
    if (position[1] == '1') {
        /* entering the "if" branch below means that the coordinate of the piece is x1 */
        if (position[2] == '\0') {
            return ((position[0] - 97) + (14 * 15));
        }
        /* entering the "else" branch below means that the coordinate of the piece is x10 ~ x15 */
        else {
            return ((15 - (10 * (position[1] - 48) + (position[2] - 48))) * 15 + (position[0] - 97));
        }
    }
    /* entering the "if" branch below means that the coordinate of the piece is x2 ~ x9 */
    else {
        return ((15 - (position[1] - 48)) * 15 + (position[0] - 97));
    }
}

/*
 this function is used to get the minimum one in two integers
 */
int min(int a, int b) { return (a - b <= 0) ? a : b; }
