#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include "board.h"
#include<ctype.h>


char fen[] = "rnbqkbnr/pppppppp/00000000/00000000/00000000/00000000/PPPPPPPP/RNBQKBNR w KQkq - 0 1";
int validMoves[100];
int validMoveIndex = 0;

int hover = 66;
int selected = 100;

int row(int index){
    return index/9;
}
int column(int index){
    return index-(9*(row(index)));
}

// calculates the moves that are legal and add them to the calidMoves array
void setValidMoves(int index){
    char p = getPiece(index);
    for(int i = 0; i < strlen(fen); i++){
        if(p == 'p'){
            if(index+9  == i && getPiece(i) == '0') addValidMove(i);
            if(index+8  == i && getPiece(i) != '0' && isWhite(i)) addValidMove(i);
            if(index+10 == i && getPiece(i) != '0' && isWhite(i)) addValidMove(i);
        }
        if(p == 'P'){
            if(index-9  == i && getPiece(i) == '0') addValidMove(i);
            if(index-8  == i && getPiece(i) != '0' && !isWhite(i)) addValidMove(i);
            if(index-10 == i && getPiece(i) != '0' && !isWhite(i)) addValidMove(i);
        }
        if(p == 'Q'){
            if(row(i)    ==    row(index) && getPiece(i) == '0') addValidMove(i);
            if(column(i) == column(index) && getPiece(i) == '0') addValidMove(i);
        }
    }
}

void setSelected(int index){
    selected = index;
    setValidMoves(index);
}

void move(int from, int to){
    setPiece(to,getPiece(from));

    setPiece(from,'0');
    clearValidMoves();

    // mv cursor to oponents side
    if(getTurn()=='w'){
        hover = 12;
    }
    else
        hover = 57;

    changeTurn();
}

char getTurn(){
    for(int i = 0; i < strlen(fen); i++){
        char c = fen[i];
        if(c == ' '){
            return fen[i+1];
        }
    }
    return 'w';
}

void setFen(char* newFen){
    memset(fen, 0, sizeof(char)+strlen(newFen));
    strcpy(fen, newFen);
}

void changeTurn(){
    for(int i = 0; i < strlen(fen); i++){
        char c = fen[i];
        if(c == ' '){
            if(fen[i+1] == 'w'){
                fen[i+1] = 'b';
            }
            else if(fen[i+1] == 'b'){
                fen[i+1] = 'w';
            }
            break;
        }
    }
}

int isWhite(int index){
    return isupper(getPiece(index));
}
char getColor(int index){
    if(isWhite(index)) return 'w';
    return 'b';
}
void addValidMove(int index){
    validMoves[validMoveIndex] = index;
    validMoveIndex++;
}
void clearValidMoves(){
    for(int i = 0; i < 100; i++){
        validMoves[i] = 100;
    }
    validMoveIndex = 0;
}
//returns 1 if the argument index is inside the valid moves list
int pieceInValid(int index){
    for(int i = 0; i < 100; i++){
        if(index == validMoves[i]){
            return 1;
        }
    }
    return 0;
}

void setPiece(int index, char v){
    fen[index] = v;
}
char getPiece(int index){
    return fen[index];
}

// returns a long fen from a fen
char* parseFen(char* fen){
    // allocats new fen string
    char* newFen = malloc(sizeof(char)*80);

    int nfeni = 0; // index fornew string
    for(int i = 0; i < strlen(fen); i++){
        char c = fen[i];
        // if a is a number etween 0-9 we add a 0
        int a = (int)c - '0';
        if(a > 0 && a < 9){
            for(int j = 0;j < a; j++){
                newFen[nfeni] = '0';
                nfeni++;
            }
            continue;
        }
        // else we just add the piece
        else{
            newFen[nfeni] = fen[i];
        }
        nfeni++;
    }
    return newFen;
}
//retuns normal fen from long fen
char* getFen(char* longfen){
    // new fen string
    char* newFen = malloc(sizeof(char)*80);
    // new fen string index
    int nfeni = 0;
    // keep track of how many empty
    int index = 0;
    for(int i = 0; i < strlen(longfen); i++){
        char c = longfen[i];
        // if we encounter empty string we start counting
        if(c == '0'){
            index++;
            continue;
        }
        // if the char is not empty and the index is > 0
        // we add the index and go back a step
        else{
            if(index > 0){
                newFen[nfeni] = index+'0';
                index=0;
                i--;
            }
            else{
                newFen[nfeni] = fen[i];
            }
        }
        nfeni++;
    }
    return newFen;
}

