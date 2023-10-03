#include <stdio.h>
#include <string.h>
#include <stdlib.h>
#include <stdbool.h>
#include "Automata.h"

bool inArray(char c, char searhcArray[], int arraySize){

    for(int i = 0; i < arraySize; i++){
        if(c == searhcArray[i]) return true;
    }
    return false;
}

int findfinalStates(char* states, char alphabet[], int alpSize, struct Automata a){

    int index = 0;
    for(int i = 0; i < a.rows; i++){

        if(!inArray(a.transitionFunction[i][1], alphabet, alpSize) || !inArray(a.transitionFunction[i][2], a.allFStates, a.fAmount)) continue;
        states[index] = a.transitionFunction[i][1];
        index++;
    }

    return index;
}

bool checkValidity(struct Automata a1, struct Automata a2, char alphabet[], int alpSize){

    char finStates1[alpSize];
    char finStates2[alpSize];
    int i1 = 0;
    int i2 = 0;

    i1 = findfinalStates(finStates1, alphabet, alpSize, a1);
    i2 = findfinalStates(finStates2, alphabet, alpSize, a2);
   
    if(i1 == 0 || i2 == 0) return false;

    for(int i = 0; i < i1; i++){
        if(inArray(finStates1[i], finStates2, i2)) return true;
    }

    return false;
}