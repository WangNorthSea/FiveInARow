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
    wchar_t board[225];
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

void refreshBoard(wchar_t * board, char white[112][3], char black[112][3]) {
    system("clear");
    printf("Welcome to the Five In A Row game!\n\n\n");
    initBoard(board);
    setBoard(board, white, L'○');
    setBoard(board, black, L'●');
    printBoard(board);
}

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

void setBoard(wchar_t * board, char position[112][3], wchar_t piece) {
    int i;
    for (i = 0; i < 112; i++) {
        if (!position[i][0]) { break; }
        board[transformCoordinate(position[i])] = piece;
    }
}

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

int checkWin(wchar_t * board) {
    if (checkWhoWins(board, WHITEWIN, L'○')) { return WHITEWIN; }
    if (checkWhoWins(board, BLACKWIN, L'●')) { return BLACKWIN; }
    return NOWIN;
}

int checkWhoWins(wchar_t * board, int WHOWINS, wchar_t piece) {
    int i, j;
    for (i = 0; i < 15; i++) {
        for (j = 0; j < 11; j++) {
            if ((board[i * 15 + j] == piece) && (board[i * 15 + j + 1] == piece) && (board[i * 15 + j + 2] == piece) && (board[i * 15 + j + 3] == piece) && (board[i * 15 + j + 4] == piece)) { return WHOWINS; }
        }
    }
    
    for (j = 0; j < 15; j++) {
        for (i = 0; i < 11; i++) {
            if ((board[j + i * 15] == piece) && (board[j + (i + 1) * 15] == piece) && (board[j + (i + 2) * 15] == piece) && (board[j + (i + 3) * 15] == piece) && (board[j + (i + 4) * 15] == piece)) { return WHOWINS; }
        }
    }
    
    for (i = 4; i < 15; i++) {
        for (j = 0; j <= (i - 4); j++) {
            if ((board[i + j * 14] == piece) && (board[(i + j * 14) + 14] == piece) && (board[(i + j * 14) + 2 * 14] == piece) && (board[(i + j * 14) + 3 * 14] == piece) && (board[(i + j * 14) + 4 * 14] == piece)) { return WHOWINS; }
        }
    }
    for (i = 0; i < 10; i++) {
        for (j = 0; j <= (9 - i); j++) {
            if ((board[29 + i * 15 + j * 14] == piece) && (board[29 + i * 15 + j * 14 + 14] == piece) && (board[29 + i * 15 + j * 14 + 2 * 14] == piece) && (board[29 + i * 15 + j * 14 + 3 * 14] == piece) && (board[29 + i * 15 + j * 14 + 4 * 14] == piece)) { return WHOWINS; }
        }
    }
    
    for (i = 0; i < 11; i++) {
        for (j = 0; j <= i; j++) {
            if ((board[10 - i + j * 16] == piece) && (board[10 - i + j * 16 + 16] == piece) && (board[10 - i + j * 16 + 2 * 16] == piece) && (board[10 - i + j * 16 + 3 * 16] == piece) && (board[10 - i + j * 16 + 4 * 16] == piece)) { return WHOWINS; }
        }
    }
    for (i = 0; i < 10; i++) {
        for (j = 0; j <= (9 - i); j++) {
            if ((board[15 * (i + 1) + j * 16] == piece) && (board[15 * (i + 1) + j * 16 + 16] == piece) && (board[15 * (i + 1) + j * 16 + 2 * 16] == piece) && (board[15 * (i + 1) + j * 16 + 3 * 16] == piece) && (board[15 * (i + 1) + j * 16 + 4 * 16] == piece)) { return WHOWINS; }
        }
    }
    
    return NOWIN;
}

int checkForbiddenMoves(wchar_t * board, char lastPiece[3]) {
    if (checkThreeThree(board, lastPiece)) { return VIOLATE33; }
    if (checkFourFour(board, lastPiece)) { return VIOLATE44; }
    return NOVIOLATE;
}

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

/* directions:
   0 means horizontal,
   1 means vertical,
   2 means from left up to right down,
   3 means from right up to left down
   0 position is left or top
*/
wchar_t * getBoardThings(wchar_t * board, char lastPiece[3], int count, int position, int direction) {
    wchar_t * things = (wchar_t *)malloc(sizeof(wchar_t) * count);
    wchar_t whitePiece = L'○';
    int boardPos = transformCoordinate(lastPiece);
    int i;
    if (direction == 0) {
        for (i = 0; i < count; i++) {
            if (position > (boardPos % 15)) {
                if (i < (position - (boardPos % 15))) { things[i] = whitePiece; }
                else { things[i] = board[boardPos + i - position]; }
            }
            else if ((count - position - 1) > (14 - (boardPos % 15))) {
                if (i >= (position + 15 - (boardPos % 15))) { things[i] = whitePiece; }
                else { things[i] = board[boardPos + i - position]; }
            }
            else { things[i] = board[boardPos + i - position]; }
        }
    }
    else if (direction == 1) {
        for (i = 0; i < count; i++) {
            if (position > (boardPos / 15)) {
                if (i < (position - (boardPos / 15))) { things[i] = whitePiece; }
                else { things[i] = board[boardPos + (i - position) * 15]; }
            }
            else if ((count - position - 1) > (14 - (boardPos / 15))) {
                if (i >= (position + 15 - (boardPos / 15))) { things[i] = whitePiece; }
                else { things[i] = board[boardPos + (i - position) * 15]; }
            }
            else { things[i] = board[boardPos + (i - position) * 15]; }
        }
    }
    else if (direction == 2) {
        for (i = 0; i < count; i++) {
            if (position > min((boardPos % 15), (boardPos / 15))) {
                if (i < (position - min((boardPos % 15), (boardPos / 15)))) { things[i] = whitePiece; }
                else { things[i] = board[boardPos + (i - position) * 16]; }
            }
            else if ((count - position - 1) > (min((14 - (boardPos % 15)), (14 - (boardPos / 15))))) {
                if (i >= (position + 1 + min((14 - (boardPos % 15)), (14 - (boardPos / 15))))) { things[i] = whitePiece; }
                else { things[i] = board[boardPos + (i - position) * 16]; }
            }
            else { things[i] = board[boardPos + (i - position) * 16]; }
        }
    }
    else {
        for (i = 0; i < count; i++) {
            if (position > min((14 - (boardPos % 15)), (boardPos / 15))) {
                if (i < (position - min((14 - (boardPos % 15)), (boardPos / 15)))) { things[i] = whitePiece; }
                else { things[i] = board[boardPos + (i - position) * 14]; }
            }
            else if ((count - position - 1) > (min((boardPos % 15), (14 - (boardPos / 15))))) {
                if (i >= (position + 1 + min((boardPos % 15), (14 - (boardPos / 15))))) { things[i] = whitePiece; }
                else { things[i] = board[boardPos + (i - position) * 14]; }
            }
            else { things[i] = board[boardPos + (i - position) * 14]; }
        }
    }
    return things;
}

int transformCoordinate(char position[3]) {
    if (position[1] == '1') {
        if (position[2] == '\0') {
            return ((position[0] - 97) + (14 * 15));
        }
        else {
            return ((15 - (10 * (position[1] - 48) + (position[2] - 48))) * 15 + (position[0] - 97));
        }
    }
    else {
        return ((15 - (position[1] - 48)) * 15 + (position[0] - 97));
    }
}

int min(int a, int b) { return (a - b <= 0) ? a : b; }
