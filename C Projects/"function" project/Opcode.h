#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <ctype.h>
/*error codes */
#define NO_ERROR 0
#define NOT_VALID_MACRO 1
#define CANT_OPEN_FILE 2
#define MACRO_USED_BEFORE_DECLERATION 3
#define ILLEGAL_LABEL 4
#define UNKNOWN_ERROR -1


/* constant vlaues*/
#define NUM_OF_OPCODES 18
#define OPCODE_MAX_LEN 10
#define NUM_OF_REGISTER 8
#define REGISTER_MAX_LEN 5


/*end of defines and constants */



/**
 * Checks if a given string matches any of the opcode names.
 *
 * @param str  The string to be checked.
 * @return     1 if the string matches an opcode name, 0 otherwise.
 */
int nameOfOpCode(const char* str );



/**
 * Prints an error message based on the provided error code.
 *
 * @param error     The error code to print.
 * @return          The error code.
 */
int printError(int error);


int isRegister(char * str);
