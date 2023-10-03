#include <stdio.h>
#include <string.h>
#include <stdlib.h>
#include "Automata.h"

struct Automata readAutomata(char filename[]){

    FILE *file = fopen (filename, "r");
    struct Automata automata;

    if(file == NULL){
        printf("Error! No file found");
        return automata;
    }

    automata.fAmount = 0;
    automata.transitionFunction = NULL;

    char c;
    int flag = 0;
    int charPos = 0;
    int row = 0;
    int col = 3;

    fseek(file, 0L, SEEK_SET);

    do{

        c = fgetc(file);
        if(!('A' <= c && c <='Z') && !('a' <= c && c <= 'z') && !('0' <= c && c <= '9')) continue;

        switch (flag) {

            case 0:
                automata.startAmount = c - '0';
                flag++;
                break;

            case 1:
                automata.statesAmount = c - '0';
                flag++;
                break;

            case 2:
                automata.startState = c - '0';
                flag++;
                break;
                
            case 3:
                if (automata.fAmount == 0) {
                    automata.fAmount = c - '0';
                    automata.allFStates = (char*)malloc(automata.fAmount * sizeof(char));
                    continue;
                }
        
                if (charPos < automata.fAmount) {
                    automata.allFStates[charPos] = c;
                    charPos++;
                } else flag++;
                break;

            default:
                break;    
    
        }

        if(flag != 4) continue;

        if(col == 3){
            col = 0;
            automata.transitionFunction = realloc(automata.transitionFunction, (row + 1) * sizeof(char*));
            automata.transitionFunction[row] = malloc(3 * sizeof(char));
            row++;
            automata.rows = row;
        }

        automata.transitionFunction[row - 1][col] = c;
        col++;
        automata.columns = col;
        
        
    }while(c != EOF);
    
    fclose(file);
    return automata;
}

void displayAutomata(struct Automata automata, int number){

    printf("Automata number %d is:\n", number);
    printf("%d\n", automata.startAmount);
    printf("%d\n", automata.statesAmount);
    printf("%d\n", automata.startState);

    for(int i = 0; i < automata.fAmount; i++){
        printf("%c ", automata.allFStates[i]);
    }
    printf("\n");

    for(int i = 0; i < automata.rows; i++){
        for(int j = 0; j < automata.columns; j++){
            printf("%c ", automata.transitionFunction[i][j]);
        }
        printf("\n");
    }
    printf("\n");
}