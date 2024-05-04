#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <ctype.h>
#include "Opcode.h"

const char opcodeList [NUM_OF_OPCODES][OPCODE_MAX_LEN] = {"mov" , "cmp" , "add" , "sub" , "not" , "clr" , "lea", "inc" , "dec"  , "jmp" , "bne" , "red"  , "prn" , "jsr" , "rts" , "stop" , ".data" , ".string"};
const char registersList[NUM_OF_REGISTER][REGISTER_MAX_LEN] ={ "@r0" , "@r1" , "@r2" , "@r3" , "@r4" , "@r5", "@r6" , "@r7"};
/**
 * Checks if a given string matches any of the opcode names.
 *
 * @param str  The string to be checked.
 * @return     1 if the string matches an opcode name, 0 otherwise.
 */
int nameOfOpCode(const char* str) {
    int i;
    for (i = 0; i < NUM_OF_OPCODES; i++) {
        if (strcmp(str, opcodeList[i] ) == 0) 
            return i+1;
    }
    return 0;
}


/**
 * Prints an error message based on the provided error code.
 *
 * @param error     The error code to print.
 * @return          The error code.
 */
int printError(int error) {
    switch (error) {
        case NO_ERROR:
            printf("\nno error\n");
            return NO_ERROR;

        case NOT_VALID_MACRO:
            printf("\nnot valid macro !\n");
            return NOT_VALID_MACRO;

        case CANT_OPEN_FILE:
            printf("\ncan't open file !\n");
            return CANT_OPEN_FILE;
			
		case MACRO_USED_BEFORE_DECLERATION:
			printf("\nmacro used befor decleration !\n");
            return MACRO_USED_BEFORE_DECLERATION;
			
		case ILLEGAL_LABEL:
			printf("\nIlligal label !\n");
            return MACRO_USED_BEFORE_DECLERATION;

    }
    printf("unknown error !");
    return UNKNOWN_ERROR;
}

int isRegister(char * str)
{
	int i = 0 ;
	for( i = 0; i < NUM_OF_REGISTER ; i++)
		if (strcmp(registersList[i] , str )==0)
			return i;
	
	return -1;
}
