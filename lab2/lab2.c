#include <stdio.h>
#include <string.h>
#include <stdlib.h>
#include <stdbool.h>
#include <math.h>
#include "Automata.h"

struct Automata readAutomata(char filename[]);

void displayAutomata(struct Automata automata, int number);

bool inArray(char c, char searhcArray[], int arraySize);

int findfinalStates(char* states, char alphabet[], int alpSize, struct Automata a);

bool checkValidity(struct Automata a1, struct Automata a2, char alphabet[], int alpSize);


void checkPossibleWords(char**words, char*** wordsTree, int pos, int alpSize, struct Automata a){
    
    int p1 = 0;
    int p2 = 0;

    for(int n = 0; n < pow(alpSize, (pos + 1)); n++){
        words[n] = NULL;
    }
    
    for(int i = 0; i < pow(alpSize, (pos + 1)); i++){
        
        if(wordsTree[pos][i] == NULL) continue;
        if(wordsTree[pos][i][0] - '0' !=  a.startState) continue;

        int prevInd = i;
        words[p1] = malloc((pos + 1) * sizeof(char));

        for(int j = pos; j >= 0; j--){

            words[p1][p2] = wordsTree[j][prevInd][1];
            prevInd = (int) prevInd/alpSize;
            p2++;
        }
        p2 = 0;
        p1++;          
    }
}


void addLetter(char*** wordTree, char alphabet[], int pos, int alpSize, struct Automata a){

    int index = 0;
    if(pos == 0){

        for(int n = 0; n < alpSize; n++){
            wordTree[pos][n] = NULL;
        }

        for(int i = 0; i < a.rows; i++){

            if(!inArray(a.transitionFunction[i][2], a.allFStates, a.fAmount) || !inArray(a.transitionFunction[i][1], alphabet, alpSize)) continue;
            wordTree[pos][index] = a.transitionFunction[i];
            index++;
        }

        return;
    }

    for(int i = 0; i < pow(alpSize, pos); i++){

        index = 0; 
        for(int n = 0; n < alpSize; n++){
            wordTree[pos][alpSize * i + n] = NULL;
        }

        for(int j = 0; j < a.rows; j++){

            if(wordTree[pos - 1][i] == NULL) break;
            if(!(a.transitionFunction[j][2] == wordTree[pos - 1][i][0]) || !inArray(a.transitionFunction[j][1], alphabet, alpSize)) continue;
            wordTree[pos][alpSize * i + index] = a.transitionFunction[j];
            index++;              
        }
    }

    return;
}


char* findSameWord(char**words1, char**words2, int arraySize){

    for(int i = 0; i < arraySize; i++){

        if(words1[i] == NULL) continue;
        for(int j = 0; j < arraySize; j++){

            if(words2[j] == NULL) continue;
            if(!strcmp(words1[i], words2[j])) return words1[i];
        } 
    }

    return NULL;
}



int main(){

    char file1[] = "automata1.txt";
    char file2[] = "automata2.txt";
    int maxLength = 10;
    
    struct Automata a1 = readAutomata(file1);
    struct Automata a2 = readAutomata(file2);

    displayAutomata(a1, 1);
    displayAutomata(a2, 2);

    int *repeated = (int *)calloc(a1.rows, sizeof(int));
    char *alphabet = NULL;
    int size = 0;

    for(int i = 0; i < a1.rows; i++){

        if (repeated[i] != 0) continue;
        for (int j = i + 1; j < a1.rows; j++){

            if(a1.transitionFunction[i][1] != a1.transitionFunction[j][1]) continue;
            repeated[j] = 1;  
        }
    }

    for(int i = 0; i < a1.rows; i++){

        if(repeated[i] == 1) continue;
        for(int j = 0; j < a2.rows; j++){

            if(a1.transitionFunction[i][1] != a2.transitionFunction[j][1]) continue;
            alphabet = realloc(alphabet, sizeof(char) * (size + 1));
            alphabet[size] = a1.transitionFunction[i][1];
            size++;
            break;
        }
    }

    if(!checkValidity(a1, a2, alphabet, size)){
        printf("No possible words!\n");
        return 0;
    }

    free(repeated);

    char ***wordsTree1 = NULL;
    char ***wordsTree2 = NULL;
    int resultSize = 0;
    char *result = NULL;

    for(int i = 0; i < maxLength; i++){
        
        int resize = pow(size, (i + 1)); 

        wordsTree1 = realloc(wordsTree1, (i + 1) * sizeof(char*));
        wordsTree1[i] = malloc(resize * sizeof(char*));
        wordsTree2 = realloc(wordsTree2, (i + 1) * sizeof(char*));
        wordsTree2[i] = malloc(resize * sizeof(char*));

        addLetter(wordsTree1, alphabet, i, size, a1);
        addLetter(wordsTree2, alphabet, i, size, a2);

        char **words1 = malloc(resize * sizeof(char*));
        char **words2 = malloc(resize * sizeof(char*));
        
        checkPossibleWords(words1, wordsTree1, i, size, a1);
        checkPossibleWords(words2, wordsTree2, i, size, a2);
        result = findSameWord(words1, words2, resize);

        free(words1);
        free(words2);
        
        if(result != NULL){
            resultSize = i + 1;
            break;
        } 
    }

    if(result != NULL){
        printf("The smalles possible word is: ");
        for(int i = 0; i < resultSize; i++){
            printf("%c", result[i]);
        }

    }else{
        printf("No word with length less than %d ", maxLength + 1);
    }
    
    free(wordsTree1);
    free(wordsTree2);
    return 0;
 }