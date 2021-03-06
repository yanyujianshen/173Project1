/*
 * File: dfa.c
 * Creator: 
 * Created: 
 * Time-stamp: 
 */

#include <stdlib.h>
#include <stdio.h>
#include <string.h>
#include <stdbool.h>
#include "dfa.h"
#include "IntHashSet.h"

struct DFA {
    int DFA_N_STATE;
    IntHashSet **transitionTable;
    int currentState;
    int acceptingState;
};

/**
 * Allocate and return a new DFA containing the given number of states.
 */
DFA new_DFA(int nstates){
    DFA this = (DFA)malloc(sizeof(struct DFA));
    if (this == NULL) {
        return NULL; // Out of memory...
    }
    this->DFA_N_STATE = nstates;
    this->currentState = 0;
    this->acceptingState = 1;
    this->transitionTable = (IntHashSet**)malloc(nstates*sizeof(IntHashSet*));

    for (int i = 0; i < nstates; i++){
    	 this->transitionTable[i] = (IntHashSet*)malloc(DFA_INPUT*sizeof(IntHashSet));
        for (int j = 0; j < DFA_INPUT; j++){
            this->transitionTable[i][j] = new_IntHashSet(1);
        }
    }
    return this;
}


/**
 * Free the given DFA.
 */
void DFA_free(DFA dfa){
	if (dfa == NULL) {
		return;
	}
	for (int i = 0; i < dfa->DFA_N_STATE; i++){
	    for (int j = 0; j < DFA_INPUT; j++){
	    	IntHashSet_free(dfa->transitionTable[i][j]);
	    }
	}
	free(dfa);
}

//making sym into ASCII value between 0-36
int convertSymbtoInt(char sym){
    int symb = (int)sym;
    if(symb < 60){ //if symb is a 'number'
        return symb = symb-22;
    }else if(symb >= 60){ //else if symb is a 'char'
        return symb = symb-97;
    }
    return -1;
}

/**
 * Return the number of states in the given DFA.
 */
int DFA_get_size(DFA dfa){
    return dfa -> DFA_N_STATE;
}

/**
 * Return the state specified by the given DFA's transition function from
 * state src on input symbol sym.
 */
int DFA_get_transition(DFA dfa, int src, char sym){

    int input = convertSymbtoInt(sym);
    IntHashSet set = dfa ->transitionTable[src][input];
    if(IntHashSet_count(set) == false){
        return -1;
    }
    IntHashSetIterator iterator = IntHashSet_iterator(set);
	int nextState = IntHashSetIterator_next(iterator); // state is the first element in the set
    return nextState;
}

/**
 * For the given DFA, set the transition from state src on input symbol
 * sym to be the state dst.
 */
void DFA_set_transition(DFA dfa, int src, char sym, int dst){
    int input = convertSymbtoInt(sym);
	IntHashSet_insert(dfa->transitionTable[src][input], dst);
	dfa->currentState = dst;
}


/**
 * Set the transitions of the given DFA for each symbol in the given str.
 * This is a nice shortcut when you have multiple labels on an edge between
 * two states.
 */
void DFA_set_transition_str(DFA dfa, int src, char *str, int dst) {
	for(int i = 0; i < sizeof(*str); i++){
		DFA_set_transition(dfa, src, str[i], dst);
	}
   
}

/**
 * Set the transitions of the given DFA for all input symbols.
 * Another shortcut method.
 */
void DFA_set_transition_all(DFA dfa, int src, int dst) {
    
	for(int i = 0; i < DFA_INPUT; i++){
		IntHashSet_insert(dfa->transitionTable[src][i], dst);
	}
}

/**
 * Set whether the given DFA's state is accepting or not.
 */
void DFA_set_accepting(DFA dfa, int state, bool value){
	if(value == true){
		dfa -> acceptingState = state;
	}
}

/**
 * Return true if the given DFA's state is an accepting state.
 */
bool DFA_get_accepting(DFA dfa, int state){
    if (state == dfa -> acceptingState){
        return true;
    }else{
        return false;
    }
}

/**
 * Run the given DFA on the given input string, and return true if it accepts
 * the input, otherwise false.
 */
bool DFA_execute(DFA dfa, char *sym){
    dfa->currentState = 0;
    int cur = 0;
    for(int i = 0; i < strlen(sym); i++) {
        cur = DFA_get_transition(dfa, cur, sym[i]);
        dfa->currentState = cur;
        if(cur == -1){
            break;
        }
    }
    if(cur != -1 && DFA_get_accepting(dfa, cur)){
        return true;
    } 
    return false;
}


/**
 * Print the given DFA to System.out.
 */

void DFA_print(DFA dfa){
   for (int i = 0; i < dfa -> DFA_N_STATE; i++){
       for (int j = 0; j < DFA_INPUT; j++){
       }
   }
}

void dfa1a(char* input) {
    DFA dfa1a = new_DFA(7);
    DFA_set_transition(dfa1a, 0, 'c', 1);
    DFA_set_transition(dfa1a, 1, 's', 2);
    DFA_set_transition(dfa1a, 2, 'c', 3);
    DFA_set_transition(dfa1a, 3, '1', 4);
    DFA_set_transition(dfa1a, 4, '7', 5);
    DFA_set_transition(dfa1a, 5, '3', 6);
    DFA_set_accepting(dfa1a, 0, false);
    DFA_set_accepting(dfa1a, 1, false);
    DFA_set_accepting(dfa1a, 2, false);
    DFA_set_accepting(dfa1a, 3, false);
    DFA_set_accepting(dfa1a, 4, false);
   	DFA_set_accepting(dfa1a, 5, false);
    DFA_set_accepting(dfa1a, 6, true);
    if(DFA_execute(dfa1a, input)){
        printf("Accept\n");
    } else {
        printf("Fail\n");
    }
    DFA_free(dfa1a);
}

void dfa1b(char* input){
	DFA dfa1b = new_DFA(4);
	DFA_set_transition(dfa1b, 0, 'c', 1);
	DFA_set_transition(dfa1b, 1, 'a', 2);
	DFA_set_transition(dfa1b, 2, 't', 3);
	DFA_set_transition_all(dfa1b, 3, 3);
	DFA_set_accepting(dfa1b, 0, false);
	DFA_set_accepting(dfa1b, 1, false);
	DFA_set_accepting(dfa1b, 2, false);
	DFA_set_accepting(dfa1b, 3, true);
	if (DFA_execute(dfa1b, input)){
        printf("Accept\n");
	} else {
        printf("Fail\n");
	}
    DFA_free(dfa1b);
}

void dfa1c(char* input){
	DFA dfa1c = new_DFA(2);
	DFA_set_transition(dfa1c, 0, '1', 0);
	DFA_set_transition(dfa1c, 0, '0', 1);
	DFA_set_transition(dfa1c, 1, '0', 0);
	DFA_set_transition(dfa1c, 1, '1', 1);
	DFA_set_accepting(dfa1c, 0, true);
	DFA_set_accepting(dfa1c, 1, false);
	if (DFA_execute(dfa1c, input)){
		printf("Accept\n");

	} else {
        printf("Fail\n");
	}
    DFA_free(dfa1c);
}

void dfa1d(char* input){
	DFA dfa1d = new_DFA(4);
	DFA_set_transition(dfa1d, 0, '0', 1);
	DFA_set_transition(dfa1d, 0, '1', 2);
	DFA_set_transition(dfa1d, 1, '0', 0);
	DFA_set_transition(dfa1d, 1, '1', 3);
	DFA_set_transition(dfa1d, 2, '0', 3);
	DFA_set_transition(dfa1d, 2, '1', 0);
	DFA_set_transition(dfa1d, 3, '0', 2);
	DFA_set_transition(dfa1d, 3, '1', 1);
	DFA_set_accepting(dfa1d, 0, true);
	DFA_set_accepting(dfa1d, 1, false);
	DFA_set_accepting(dfa1d, 2, false);
	DFA_set_accepting(dfa1d, 3, false);
	if (DFA_execute(dfa1d, input)){
        printf("Accept\n");
	} else {
        printf("Fail\n");
	}
    DFA_free(dfa1d);
}

// DFA that recognizes binary strings starting with 1 and ending with 0
void dfa1e(char* input){
    DFA dfa1e = new_DFA(4);
    DFA_set_transition(dfa1e, 0, '1', 1);
	DFA_set_transition(dfa1e, 0, '0', 3);
	DFA_set_transition(dfa1e, 1, '0', 2);
	DFA_set_transition(dfa1e, 1, '1', 1);
	DFA_set_transition(dfa1e, 2, '0', 2);
	DFA_set_transition(dfa1e, 2, '1', 1);
	DFA_set_transition(dfa1e, 3, '0', 3);
	DFA_set_transition(dfa1e, 3, '1', 3);
	DFA_set_accepting(dfa1e, 0, false);
	DFA_set_accepting(dfa1e, 1, false);
	DFA_set_accepting(dfa1e, 2, true);
	DFA_set_accepting(dfa1e, 3, false);
	if (DFA_execute(dfa1e, input)){
        printf("Accept\n");
	} else {
        printf("Fail\n");
	}
    DFA_free(dfa1e);
}

#ifdef MAIN
//int main(int argc, char* argv[]){
//	char input[50];
//	printf("Type a character: ");
//	scanf("%s", input);
//    dfapart1(input);
//

int main(int argc, char* argv[]){
    int menu;
    
    printf("Welcome! Type in the number to select which DFA you want to test\n");
    printf("Type 1 for 'csc173'\n");
    printf("Type 2 for any string starts with 'cat'\n");
    printf("Type 3 for any binary input with an even number of 0’s\n");
    printf("Type 4 for binary input with an even number of both 0’s and 1’s\n");
    scanf("%d", &menu);
    if (menu == 1) {
        printf("Testing DFA that recognizes exactly 'csc173' \n");
        printf("Input csc173: ");
        char input[10];
        scanf("%s", input);
        dfa1a(input);
    } else if (menu == 2){
        printf("Testing DFA that's for any string starts with 'cat', limit the length of your input under 100\n");
        printf("Type a string:\n");
        char input[100];
        scanf("%s", input);
        dfa1b(input);
    } else if (menu == 3){
        printf("Testing DFA that recognizes even number of 0s: \n");
        printf("Binary input: \n");
        char input[100];
        scanf("%s", input);
        dfa1c(input);
    } else if (menu == 4){
        printf("Testing DFA that recognizes even number of both 0s and 1s");
        printf("Binary input: \n");
        char input[100];
        scanf("%s", input);
        dfa1d(input);
    }

}
 
#endif
