#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <ctype.h>
#include "function.h"
#include "myfile.h"
#include "Opcode.h"
Function* createFunction(char* name, char** arguments, size_t argumentCount, unsigned int instructionCount) {
    Function* newFunction = (Function*)malloc(sizeof(Function));
    newFunction->name = name;
    newFunction->arguments = arguments;
    newFunction->argumentCount = argumentCount;
    newFunction->instructionCount = instructionCount;
    newFunction->next = NULL;
    return newFunction;
}


void insertFunctionEnd(Function** head, Function* newFunction) {
    if (*head == NULL) {
        *head = newFunction;
    } else {
        Function* current = *head;
        while (current->next != NULL) {
            current = current->next;
        }
        current->next = newFunction;
    }
}

void printFunctions(Function* head) {
	int i  = 0;
    Function* current = head;
    while (current != NULL) {
        printf("Function Name: %s\n", current->name);
        printf("Argument Count: %d\n", current->argumentCount);
        printf("Arguments:\n");
        for ( i = 0; i < current->argumentCount; i++) {
            printf("%s\n", current->arguments[i]);
        }
        printf("Instruction Count: %u\n", current->instructionCount);
        printf("\n");
        current = current->next;
    }
}

void freeFunctionsList(Function* head) {
    Function* current = head;
    while (current != NULL) {
        Function* temp = current;
        current = current->next;
        free(temp);
    }
}

void freeFunction(Function* function) {
	int i  = 0 ;
    if (function == NULL)
        return;

    free(function->name);

    for ( i = 0; i < function->argumentCount; i++)
        free(function->arguments[i]);
    free(function->arguments);

    free(function);
}

int disassembleAssembly(const char* assemblyLine, Function** func) {
    Function* function = (Function*)malloc(sizeof(Function));
	int i ;
	char* argCopy;
    /* Copy the assembly line to avoid modifying the original string */
    char* lineCopy = strdup(assemblyLine);

    /* Tokenize the assembly line by spaces */
    char* token = strtok(lineCopy, " ");
	
	 function->name = NULL;
    function->arguments = NULL;
    function->argumentCount = 0;
    function->instructionCount = 0;
    function->next = NULL;
    if (token == NULL) {
        /* No tokens found */
        free(lineCopy);
        *func = NULL;
        return -6;
    }

    /* Assign opcode name */
    function->name = strdup(token);

    /* Check if there are arguments */
    token = strtok(NULL, " ,");
    if (token == NULL) {
        /* No arguments found */
        free(lineCopy);
        *func = copyFunction(function);
        return 0;
    }

    /* Count the number of arguments */
    while (token != NULL) {
        function->argumentCount++;
        token = strtok(NULL, " ,");
    }

    /* Allocate memory for arguments */
    function->arguments = (char**)malloc(function->argumentCount * sizeof(char*));

    /* Tokenize the assembly line again to assign arguments */
    argCopy = strdup(assemblyLine);
    token = strtok(argCopy, " ,"); /* Skip opcode name */

    /* Assign arguments to the struct */
    for ( i = 0; i < function->argumentCount; i++) {
        token = strtok(NULL, " ,");
        function->arguments[i] = strdup(token);
    }

    free(lineCopy);
    free(argCopy);

    /* Set the instruction count */
    function->instructionCount = function->argumentCount;
    if (countCharOccurrences(assemblyLine, ',') > function->argumentCount-1)  {
		if (DEBUG >=1)
			printf("%s-\n More commas than arguments\n" , assemblyLine);
        freeFunction(function);
        *func = NULL;
        return -6;
    }

    *func = copyFunction(function);
    return 0;
}

Function* copyFunction(const Function* source) {
	int i  = 0 ; 
    Function* destination = (Function*)malloc(sizeof(Function));
    destination->name = strdup(source->name);
    destination->argumentCount = source->argumentCount;
    destination->instructionCount = source->instructionCount;
    destination->next = NULL;

    /* Copy arguments */
    if (source->argumentCount > 0) {
        destination->arguments = (char**)malloc(source->argumentCount * sizeof(char*));
        for ( i = 0; i < source->argumentCount; i++) {
            destination->arguments[i] = strdup(source->arguments[i]);
        }
    } else {
        destination->arguments = NULL;
    }

    return destination;
}


Function* createFunctionListFromFile(const char* fileName) {
    char line[256];
	int error = 0;
	 FILE* file = fopen(fileName, "r");
	Function* head = NULL;
    Function* tail = NULL;
	Function* function = NULL;
    if (file == NULL) {
        printf("Failed to open file: %s\n", fileName);
        return NULL;
    }

    
    while (fgets(line, sizeof(line), file)) {
        /* Remove the newline character at the end */
        line[strcspn(line, "\n")] = '\0';

        /* Create a new Function struct */
        
        error = disassembleAssembly(line, &function);
        if (error != 0) {
            printf("Error processing line: %s\n", line);
            freeFunctionList(head); /* Free the previously created functions */
            fclose(file);
            return NULL;
        }


        /* Add the Function struct to the linked list */
        if (head == NULL) {
            head = function;
            tail = function;
        } else {
            tail->next = function;
            tail = function;
        }
    }

    fclose(file);
    return head;
}


void freeFunctionList(Function* head) {
    while (head != NULL) {
        Function* next = head->next;
        freeFunction(head);
        head = next;
    }
}




int functionValidator(Function* function , const char* labelFileName)
{
	switch(nameOfOpCode(function->name) )
	{
		case 1: 
			return movValidator(function , labelFileName);
			break;
		case 2: 
			return cmpValidator(function , labelFileName);
			break;
		case 3: 
			return addValidator(function , labelFileName);
			break;
		case 4: 
			return subValidator(function , labelFileName);
			break;
		case 5: 
			return notValidator(function , labelFileName);
			break;
		case 6: 
			return clrValidator(function , labelFileName);
			break;
		case 7: 
			return leaValidator(function , labelFileName);
			break;
		case 8: 
			return incValidator(function , labelFileName);
			break;
		case 9: 
			return decValidator(function , labelFileName);
			break;
		case 10: 
			return jmpValidator(function , labelFileName);
			break;
		case 11: 
			return bneValidator(function , labelFileName);
			break;
		case 12: 
			return redValidator(function , labelFileName);
			break;
		case 13: 
			return prnValidator(function , labelFileName);
			break;
		case 14: 
			return jsrValidator(function , labelFileName);
			break;
		case 15: 
			return rtsValidator(function );
			break;
		case 16: 
			return stopValidator(function );
			break;
		case 17: 
			return dataValidator(function);
			break;
		case 18: 
			return stringValidator(function, labelFileName);
			break;
	    default:
			printf("%s -no such function\n" , function->name);
			return -1;
			break;
	}
	
	return 0 ;
}


int listValidator(Function *head , const char* labelFileName)
{
	Function* current = head;
	while (current != NULL) {
        if(functionValidator(current , labelFileName)==-1)
			return -1;
        current = current->next;
	}	
	return 0;
}


int movValidator(Function* function , const char* labelFileName)
{
	char* c;
	if (function->argumentCount != 2 ) 
	{
		printf("%s - no right amount of arguments" , function->name);
		return -1;
	}
    c = concatenateStrings(function->arguments[1] , ":");
	if(isNumber(function->arguments[1]) || (isRegister(function->arguments[1]) == -1 && !isWordInFile(labelFileName,c)))
	{
		printf("\nIlligal second argument %s\n", function->arguments[1]);
		return -1;
	}
	else
	{
		c = concatenateStrings(function->arguments[0] , ":");
		if(!isNumber(function->arguments[0]) && isRegister(function->arguments[0]) == -1 && !isWordInFile(labelFileName ,c))
		{
				printf("\nIlligal first argument %s\n", function->arguments[0]);
				return -1;
		}
	}

	return 1;
	
		
}


int  cmpValidator(Function* function, const char* labelFileName)
{
	char* c;
	if (function->argumentCount != 2 ) 
	{
		printf("%s - no right amount of arguments" , function->name);
		return -1;
	}
	c = concatenateStrings(function->arguments[1] , ":");
	if(!isNumber(function->arguments[1]) && isRegister(function->arguments[1]) == -1 && !isWordInFile(labelFileName ,c))
	{
		printf("\nIlligal second argument %s\n", function->arguments[1]);
		return -1;
	}
	else
	{
	    c = concatenateStrings(function->arguments[0] , ":");
		if(!isNumber(function->arguments[0]) && isRegister(function->arguments[0]) == -1 && !isWordInFile(labelFileName ,c))
		{
				printf("\nIlligal first argument %s\n", function->arguments[0]);
				return -1;
		}
	}
	return 1;
	
}


int addValidator(Function* function, const char* labelFileName)
{
	char* c;
	if (function->argumentCount != 2 ) 
	{
		printf("%s - no right amount of arguments" , function->name);
		return -1;
	}
    c = concatenateStrings(function->arguments[1] , ":");
	if(isNumber(function->arguments[1]) || (isRegister(function->arguments[1]) == -1 && !isWordInFile(labelFileName ,c)))
	{
		printf("\nIlligal second argument %s\n", function->arguments[1]);
		return -1;
	}
	else
	{
		c = concatenateStrings(function->arguments[0] , ":");
		if(!isNumber(function->arguments[0]) && isRegister(function->arguments[0]) == -1 && !isWordInFile(labelFileName ,c))
		{
				printf("\nIlligal first argument %s\n", function->arguments[0]);
				return -1;
		}
	}
	return 1;
	
		
}

int subValidator(Function* function, const char* labelFileName)
{
	char* c;
	if (function->argumentCount != 2 ) 
	{
		printf("%s - no right amount of arguments" , function->name);
		return -1;
	}
    c = concatenateStrings(function->arguments[1] , ":");
	if(isNumber(function->arguments[1]) || (isRegister(function->arguments[1]) == -1 && !isWordInFile(labelFileName ,c)))
	{
		printf("\nIlligal second argument %s\n", function->arguments[1]);
		return -1;
	}
	else
	{
		c = concatenateStrings(function->arguments[0] , ":");
		if(!isNumber(function->arguments[0]) && isRegister(function->arguments[0]) == -1 && !isWordInFile(labelFileName ,c))
		{
				printf("\nIlligal first argument %s\n", function->arguments[0]);
				return -1;
		}
	}
	return 1;
	
		
}


int notValidator(Function* function, const char* labelFileName)
{
	char* c;
	if (function->argumentCount != 1 ) 
	{
		printf("%s - no right amount of arguments" , function->name);
		return -1;
	}
	
	c = concatenateStrings(function->arguments[0] , ":");
	if(isNumber(function->arguments[0]) || (isRegister(function->arguments[0]) == -1 && !isWordInFile(labelFileName ,c)))
	{
		printf("\nIlligal  argument %s\n", function->arguments[0]);
		return -1;
	}

	return 1;
}


int clrValidator(Function* function, const char* labelFileName)
{
	char* c;
	if (function->argumentCount != 1 ) 
	{
		printf("%s - no right amount of arguments" , function->name);
		return -1;
	}
	
	c = concatenateStrings(function->arguments[0] , ":");
	if(isNumber(function->arguments[0]) || (isRegister(function->arguments[0]) == -1 && !isWordInFile(labelFileName ,c)))
	{
		printf("\nIlligal  argument %s\n", function->arguments[0]);
		return -1;
	}

	return 1;
}

int leaValidator(Function* function, const char* labelFileName)
{
	char* c;
	if (function->argumentCount != 2 ) 
	{
		printf("%s - no right amount of arguments" , function->name);
		return -1;
	}
    c = concatenateStrings(function->arguments[1] , ":");
	if(isNumber(function->arguments[1]) || (isRegister(function->arguments[1]) == -1 && !isWordInFile(labelFileName,c)))
	{
		printf("\nIlligal second argument %s\n", function->arguments[1]);
		return -1;
	}
	else
	{
		c = concatenateStrings(function->arguments[0] , ":");
		if( !isWordInFile(labelFileName ,c))
		{
				printf("\nIlligal first argument %s\n", function->arguments[0]);
				return -1;
		}
	}
	return 1;
	
		
}



int incValidator(Function* function, const char* labelFileName)
{
	char* c;
	if (function->argumentCount != 1 ) 
	{
		printf("%s - no right amount of arguments" , function->name);
		return -1;
	}
	
	c = concatenateStrings(function->arguments[0] , ":");
	if(isNumber(function->arguments[0]) || (isRegister(function->arguments[0]) == -1 && !isWordInFile(labelFileName,c)))
	{
		printf("\nIlligal  argument %s\n", function->arguments[0]);
		return -1;
	}

	return 1;
}



int decValidator(Function* function, const char* labelFileName)
{
	char* c;
	if (function->argumentCount != 1 ) 
	{
		printf("%s - no right amount of arguments" , function->name);
		return -1;
	}
	
	c = concatenateStrings(function->arguments[0] , ":");
	if(isNumber(function->arguments[0]) || (isRegister(function->arguments[0]) == -1 && !isWordInFile(labelFileName ,c)))
	{
		printf("\nIlligal  argument %s\n", function->arguments[0]);
		return -1;
	}

	return 1;
}


int jmpValidator(Function* function, const char* labelFileName)
{
	char* c;
	if (function->argumentCount != 1 ) 
	{
		printf("%s - no right amount of arguments" , function->name);
		return -1;
	}
	
	c = concatenateStrings(function->arguments[0] , ":");
	if(isNumber(function->arguments[0]) || (isRegister(function->arguments[0]) == -1 && !isWordInFile(labelFileName ,c)))
	{
		printf("\nIlligal  argument %s\n", function->arguments[0]);
		return -1;
	}

	return 1;
}


int bneValidator(Function* function, const char* labelFileName)
{
	char* c;
	if (function->argumentCount != 1 ) 
	{
		printf("%s - no right amount of arguments" , function->name);
		return -1;
	}
	
	c = concatenateStrings(function->arguments[0] , ":");
	if(isNumber(function->arguments[0]) || (isRegister(function->arguments[0]) == -1 && !isWordInFile(labelFileName ,c)))
	{
		printf("\nIlligal  argument %s\n", function->arguments[0]);
		return -1;
	}

	return 1;
}


int redValidator(Function* function, const char* labelFileName)
{
	char* c;
	if (function->argumentCount != 1 ) 
	{
		printf("%s - no right amount of arguments" , function->name);
		return -1;
	}
	
	c = concatenateStrings(function->arguments[0] , ":");
	if(isNumber(function->arguments[0]) || (isRegister(function->arguments[0]) == -1 && !isWordInFile(labelFileName ,c)))
	{
		printf("\nIlligal  argument %s\n", function->arguments[0]);
		return -1;
	}

	return 1;
}


int jsrValidator(Function* function, const char* labelFileName)
{
	char* c;
	if (function->argumentCount != 1 ) 
	{
		printf("%s - no right amount of arguments" , function->name);
		return -1;
	}
	
	c = concatenateStrings(function->arguments[0] , ":");
	if(isNumber(function->arguments[0]) || (isRegister(function->arguments[0]) == -1 && !isWordInFile(labelFileName ,c)))
	{
		printf("\nIlligal  argument %s\n", function->arguments[0]);
		return -1;
	}

	return 1;
}


int prnValidator(Function* function, const char* labelFileName)
{
	char* c;
	if (function->argumentCount != 1 ) 
	{
		printf("%s - no right amount of arguments" , function->name);
		return -1;
	}
	
	c = concatenateStrings(function->arguments[0] , ":");
	if(!isNumber(function->arguments[0]) && (isRegister(function->arguments[0]) == -1 && !isWordInFile(labelFileName ,c)))
	{
		printf("\nIlligal  argument %s\n", function->arguments[0]);
		return -1;
	}

	return 1;
}


int rtsValidator(Function* function)
{
	if (function->argumentCount != 0 ) 
	{
		printf("%s - no right amount of arguments" , function->name);
		return -1;
	}
		return 1;

}

int stopValidator(Function* function)
{
	if (function->argumentCount != 0 ) 
	{
		printf("%s - no right amount of arguments" , function->name);
		return -1;
	}
	return 1;
}



int dataValidator(Function* function)
{
	int i ;
	if (function->argumentCount == 0 ) 
		{
		printf("%s - no right amount of arguments" , function->name);
		return -1;
	     }
		 
	
	for(i  = 0 ; i <function->argumentCount  ; i++)
	{
		if(!isNumber(function->arguments[i]))
		{
			printf("\nIlligal  argument %s\n", function->arguments[i]);
			return -1;
		}
	}
		return 1;
}


int stringValidator(Function* function, const char* labelFileName)
{
	char* c;
	if (function->argumentCount != 1 ) 
		{
		printf("%s - no right amount of arguments" , function->name);
		return -1;
	     }
		c = concatenateStrings(function->arguments[0] , ":");

	if(isNumber(function->arguments[0]) || (isRegister(function->arguments[0]) != -1 || isWordInFile(labelFileName,c)))
	{
		printf("\nIlligal  argument %s\n", function->arguments[0]);
		return -1;
	}
	if(getCharAtIndex(function->arguments[0] , 0) != 34 || getCharAtIndex(function->arguments[0] , strlen(function->arguments[0] ) - 1) != 34)
	{
		printf("%c  %c  " , getCharAtIndex(function->arguments[0] , 0) , getCharAtIndex(function->arguments[0] , strlen(function->arguments[0] ) - 1));
		printf("\nIlligal  argument %s\n", function->arguments[0]);
		return -1;
	}
		return 1;
}



/* Function to generate machine code based on opcode 2*/
int MachineCodeType1(Function* function) {
    /* Check if the opcode of the function is 1, 3, or 4 */
    if (nameOfOpCode(function->name) == 1 || nameOfOpCode(function->name) == 3 || nameOfOpCode(function->name) == 4) {

        /* Check if the first argument is a register (starts with '@') */
        if (isRegister(function->arguments[0]) > -1) {
            /* Print the first argument (register) */

            /* Check if the second argument is also a register */
            if (isRegister(function->arguments[1]) > -1) {
                /* Print the second argument (register) */
                /* Set instructionCount to 2 */
                function->instructionCount = 2;
            } else {
                /* Print the second argument (either a number or LENGTH) */
                /* Set instructionCount to 3 */
                function->instructionCount = 3;
            }
        } else {
            /* If the first argument is not a register (either a number or LENGTH) */
            /* Print the first argument as is (either a number or LENGTH) */

            /* Check if the second argument is a register */
            if (isRegister(function->arguments[1]) > -1) {
                /* Print the second argument (register) */
                printf("%s ", function->arguments[1]); /* @r3 */
                /* Set instructionCount to 3 */
                function->instructionCount = 3;
            } else {
                /* If the second argument is not a register, assume it's LENGTH */
                /* Print the second argument (LENGTH) */
                /* Set instructionCount to 3 */
                function->instructionCount = 3;
            }
        }

        /* If the first argument is a number, print its value */
        if (isNumber(function->arguments[0])) {
            printf("%s ", function->arguments[0]);

            /* Check if the second argument is a register */
            if (isRegister(function->arguments[1]) > -1) {
                /* Print the second argument (register) */
                /* Set instructionCount to 3 */
                function->instructionCount = 3;
            } else {
                /* If the second argument is not a register, assume it's LENGTH */
                /* Print the second argument (LENGTH) */
                /* Set instructionCount to 3 */
                function->instructionCount = 3;
            }
        }

        /* Return 1 to indicate successful handling of opcodes 1, 3, and 4 */
        return function->instructionCount ;
    }

    /* Return 0 if the opcode is not 1, 3, or 4 */
    return -1;
}

/* Function to generate machine code based on opcode 2*/
int MachineCodeType2(Function* function) {
    /* Check if the opcode of the function is 2 */
    if (nameOfOpCode(function->name) == 2) {
        /* Check if the first argument is a register (starts with '@') */
        if (isRegister(function->arguments[0]) > -1) {
            /* Print the first argument (register) */

            /* Check if the second argument is also a register */
            if (isRegister(function->arguments[1]) > -1) {
                /* Print the second argument (register) */
                /* Set instructionCount to 2 */
                function->instructionCount = 2;
            } else {
                /* Check if the second argument is a number */
                if (isNumber(function->arguments[1])) {
                    /* Print the second argument (number) */
                    /* Set instructionCount to 3 */
                    function->instructionCount = 3;
                } else {
                    /* If the second argument is neither a register nor a number, assume it's a LENGTH value */
                    /* Print the second argument (LENGTH) */
                    /* Set instructionCount to 3 */
                    function->instructionCount = 3;
                }
            }
        } else {
            /* If the first argument is not a register (either a number or a string) */
            /* Print the first argument as is */
            printf("%s ", function->arguments[0]);

            /* Check if the second argument is a number */
            if (isNumber(function->arguments[1])) {
                /* Print the second argument (number) */
                /* Set instructionCount to 3 */
                function->instructionCount = 3;
            }
            /* Check if the second argument is a register */
            if (isRegister(function->arguments[1]) > -1) {
                /* Print the second argument (register) */
                /* Set instructionCount to 3 */
                function->instructionCount = 3;
            } else {
                /* If the second argument is neither a number nor a register, assume it's a LENGTH value */
                /* Print the second argument (LENGTH) */
                /* Set instructionCount to 3 */
                function->instructionCount = 3;
            }
        }
        /* Return 1 to indicate successful handling of opcode 2 */
        return function->instructionCount ;
    }

    /* Return 0 if the opcode is not 2 */
    return -1;
}

/* Function to generate machine code based on opcode 5,6,8,9,10,11,12,14*/
int MachineCodeType3(Function* function) {
    /* Check if the opcode matches any of the specified opcodes */
    if (nameOfOpCode(function->name) == 5 || nameOfOpCode(function->name) == 6 ||
        nameOfOpCode(function->name) == 8 || nameOfOpCode(function->name) == 9 ||
        nameOfOpCode(function->name) == 10 || nameOfOpCode(function->name) == 11 ||
        nameOfOpCode(function->name) == 12 || nameOfOpCode(function->name) == 14) {
        
        /* Check if the first argument is a valid register */
        if (isRegister(function->arguments[0]) > -1) { 
            function->instructionCount = 2;
        } else {
            /* Assume the first argument is a string representing length */
            function->instructionCount = 2;
        }
        
        return function->instructionCount ;
    }
    
    return -1; 
}

/* Function to generate machine code based on opcode 7*/
int MachineCodeType4(Function* function) {
    /* Check if the opcode is 7 */
    if (nameOfOpCode(function->name) == 7) {

        /* Check if the first argument is not a valid register */
        if (isRegister(function->arguments[0]) > -1) {
            
            /* Check if the second argument is a valid register */
            if (isRegister(function->arguments[1]) > -1) {
                function->instructionCount = 2;
            } else {
                /* Assume the second argument is a string representing length */
                function->instructionCount = 2;
            }
            
           return function->instructionCount ;
        }
    }
    return -1; 
}

/* Function to generate machine code based on opcode 13*/
int MachineCodeType5(Function* function) {
    /* Check if the opcode is 13 */
    if (nameOfOpCode(function->name) == 13) {
        /* Check if the first argument is a valid register */
        if (isRegister(function->arguments[0]) > -1) { 
            function->instructionCount = 2;
        } else {
            /* Check if the first argument is a valid number */
            if (isNumber(function->arguments[0])) { 
                function->instructionCount = 2;
            } else {
                /* Assume the first argument is a string representing length */
                function->instructionCount = 2;
            }
        }
        return function->instructionCount ;
    }
    return -1; 
}


/**
 * The function machineCodeString takes a pointer to a Function structure and computes instructionCount.
 * The string in arguments[0] of the Function structure is calculated using the stringLength function.
 * The estimated value of instructionCount is the length of the string in the Function structure excluding the '\0' character.
 * As per the user's request, the function returns the calculated length (stringLength(function->arguments[0]) - 1).
 * 
 * @param function A pointer to a Function structure containing the function's arguments.
 * @return The function returns the length of the string in the Function structure excluding the '\0' character.
 */
int  machineCodeString(Function* function)
{
    function->instructionCount = stringLength(function->arguments[0]) - 1;
	return function->instructionCount;
}



int functionCounter(Function* function )
{
	switch(nameOfOpCode(function->name) )
	{
		case 1: 
		    return MachineCodeType1(function );
			break;
		case 2: 
		    return MachineCodeType2(function );
			break;
		case 3: 
		    return MachineCodeType1(function );
			break;
		case 4: 
		    return MachineCodeType1(function );
			break;
		case 5: 
		    return MachineCodeType3(function );
			break;
		case 6: 
		    return MachineCodeType3(function );
			break;
		case 7: 
		    return MachineCodeType4(function );
			break;
		case 8: 
		    return MachineCodeType3(function );
			break;
		case 9: 
		    return MachineCodeType3(function );
			break;
		case 10: 
		    return MachineCodeType3(function );
			break;
		case 11: 
		    return MachineCodeType3(function );
			break;
		case 12: 
		    return MachineCodeType3(function );
			break;
		case 13: 
		    return MachineCodeType5(function );
			break;
		case 14: 
		    return MachineCodeType3(function );
			break;
		case 15: 
			return function->instructionCount = 1;
			break;
		case 16: 
		    return function->instructionCount = 1;
			break;
		case 17: 
			break;
		case 18: 
		    return machineCodeString(function);
			break;
	    default:
			printf("%s -no such function\n" , function->name);
			return -1;
			break;
	}
	
	return 0 ;
}


int listInstructionCount(Function *head , char * instruction , char *  data)
{
	
	Function* current = head;
	int temp  ;
	int tempInstructions = 0 ;
	int tempData = 0; 
	while (current != NULL) {
		temp = functionCounter(current );
        if(temp==-1)
			return -1;
		if (nameOfOpCode(current->name ) <= 17 )
			tempData = tempData + temp;
		else 
			tempInstructions = tempInstructions + temp;
        current = current->next;
	}	

	sprintf(instruction, "%d", tempInstructions);
	sprintf(data, "%d", tempData);
	return 0;
}


unsigned int sumInstructionCount(Function* head, size_t index) {
	unsigned int sum = 0;
	size_t currentIndex = 0;
	 Function* current = head;
    if (head == NULL) {
        printf("Error: The linked list is empty.\n");
        return 0;
    }

    while (current != NULL && currentIndex <= index) {
        sum += current->instructionCount;
        current = current->next;
        currentIndex++;
    }

    if (currentIndex <= index) {
        return 0;
    }

    return sum;
}


char* binaryCodeWithinSize(int num1, int num2) {
    int mask = (1 << num2) - 1;
	int i ;
    int result = num1 & mask;
    static char binaryString[33];

    for ( i = num2 - 1; i >= 0; i--) {
        binaryString[i] = (result & 1) + '0';
        result >>= 1;
    }

    binaryString[num2] = '\0'; 

    return binaryString;
}


char* movGetBInaryCode(Function* func , const char* labelFileName  )
{
	char* opcode = "0000";
	char* firstArg ;
	char* secondArg ;
    char*	firstArgAdd ;
   char*	secondArgAdd ;
   char* output;
	if (func->instructionCount == 2)
	{
		
		firstArg = "101";
		secondArg = "101";
		firstArgAdd = binaryCodeWithinSize(isRegister(func->arguments[0]) , 5);
		secondArgAdd =  binaryCodeWithinSize(isRegister(func->arguments[1]) , 5);
		output = concatenateStrings(firstArg , opcode);
		output = concatenateStrings(output , secondArg);
		output = concatenateStrings(output , "00\n");
		output = concatenateStrings(output , firstArgAdd);
		output = concatenateStrings(output , secondArgAdd);
		output = concatenateStrings(output , "00\n");
		return output;
		
	}
	else
	{
		/*determined first argument*/
		if (isRegister(func->arguments[0])>-1)
		{
			firstArg = "101";
		    firstArgAdd = binaryCodeWithinSize(isRegister(func->arguments[0]) , 5);
			firstArgAdd = concatenateStrings(firstArgAdd , "0000000\n");
		}
		else if(isNumber(func->arguments[0]))
		{
				firstArg = "001";
				firstArgAdd = binaryCodeWithinSize(atoi(func->arguments[0]), 5);
				firstArgAdd = concatenateStrings(firstArgAdd , "0000000\n");
		}
		else
		{
			firstArg = "011";
			if(getNumberFromWord(labelFileName , func->arguments[0]) == -1)
			{
				firstArgAdd = "000000000001";
			}
			else
			{
				firstArgAdd = binaryCodeWithinSize(getNumberFromWord(labelFileName , func->arguments[0]), 10);
				firstArgAdd = concatenateStrings(firstArgAdd , "10\n");
			}
		}
		
		/*determined second argument*/
		if (isRegister(func->arguments[1]) == 1)
		{
			
			secondArg = "101";
		    secondArgAdd = binaryCodeWithinSize(isRegister(func->arguments[1]) , 5);
			secondArgAdd = concatenateStrings(secondArgAdd , "0000000\n");
		}
		else
		{
			secondArg = "011";
			if(getNumberFromWord(labelFileName , func->arguments[1]) == -1)
			{
				secondArgAdd = "000000000001";
			}
			else
			{
				secondArgAdd = binaryCodeWithinSize(getNumberFromWord(labelFileName , func->arguments[1]), 10);
				secondArgAdd = concatenateStrings(secondArgAdd , "10\n");
			}
		}
		
		
		
	}
	
	    output = concatenateStrings(firstArg , opcode);
		output = concatenateStrings(output , secondArg);
		output = concatenateStrings(output , "00\n");
		output = concatenateStrings(output , firstArgAdd);
		output = concatenateStrings(output , secondArgAdd);

	return output;
}



char* cmpGetBInaryCode(Function* func , const char* labelFileName  )
{
	char* opcode = "0001";
	char* firstArg ;
	char* secondArg ;
    char*	firstArgAdd ;
   char*	secondArgAdd ;
   char* output;
	if (func->instructionCount == 2)
	{
		
		firstArg = "101";
		secondArg = "101";
		firstArgAdd = binaryCodeWithinSize(isRegister(func->arguments[0]) , 5);
		secondArgAdd =  binaryCodeWithinSize(isRegister(func->arguments[1]) , 5);
		output = concatenateStrings(firstArg , opcode);
		output = concatenateStrings(output , secondArg);
		output = concatenateStrings(output , "00\n");
		output = concatenateStrings(output , firstArgAdd);
		output = concatenateStrings(output , secondArgAdd);
		output = concatenateStrings(output , "00\n");
		return output;
		
	}
	else
	{
		/*determined first argument*/
		if (isRegister(func->arguments[0])>-1)
		{
			firstArg = "101";
		    firstArgAdd = binaryCodeWithinSize(isRegister(func->arguments[0]) , 5);
			firstArgAdd = concatenateStrings(firstArgAdd , "0000000\n");
		}
		else if(isNumber(func->arguments[0]))
		{
				firstArg = "001";
				firstArgAdd = binaryCodeWithinSize(atoi(func->arguments[0]), 5);
				firstArgAdd = concatenateStrings(firstArgAdd , "0000000\n");
		}
		else
		{
			firstArg = "011";
			if(getNumberFromWord(labelFileName , func->arguments[0]) == -1)
			{
				firstArgAdd = "000000000001";
			}
			else
			{
				firstArgAdd = binaryCodeWithinSize(getNumberFromWord(labelFileName , func->arguments[0]), 10);
				firstArgAdd = concatenateStrings(firstArgAdd , "10\n");
			}
		}
		
		/*determined second argument*/
		if (isRegister(func->arguments[1]) == 1)
		{
			
			secondArg = "101";
		    secondArgAdd = binaryCodeWithinSize(isRegister(func->arguments[1]) , 5);
			secondArgAdd = concatenateStrings(secondArgAdd , "0000000\n");
		}
		else if(isNumber(func->arguments[1]))
		{
				secondArg = "001";
				secondArgAdd = binaryCodeWithinSize(atoi(func->arguments[1]), 5);
				secondArgAdd = concatenateStrings(secondArgAdd , "0000000\n");
		}
		else
		{
			secondArg = "011";
			if(getNumberFromWord(labelFileName , func->arguments[1]) == -1)
			{
				secondArgAdd = "000000000001";
			}
			else
			{
				secondArgAdd = binaryCodeWithinSize(getNumberFromWord(labelFileName , func->arguments[1]), 10);
				secondArgAdd = concatenateStrings(secondArgAdd , "10\n");
			}
		}
		
		
		
	}
	
	    output = concatenateStrings(firstArg , opcode);
		output = concatenateStrings(output , secondArg);
		output = concatenateStrings(output , "00\n");
		output = concatenateStrings(output , firstArgAdd);
		output = concatenateStrings(output , secondArgAdd);

	return output;
}


char* addGetBInaryCode(Function* func , const char* labelFileName  )
{
	char* opcode = "0010";
	char* firstArg ;
	char* secondArg ;
    char*	firstArgAdd ;
   char*	secondArgAdd ;
   char* output;
	if (func->instructionCount == 2)
	{
		
		firstArg = "101";
		secondArg = "101";
		firstArgAdd = binaryCodeWithinSize(isRegister(func->arguments[0]) , 5);
		secondArgAdd =  binaryCodeWithinSize(isRegister(func->arguments[1]) , 5);
		output = concatenateStrings(firstArg , opcode);
		output = concatenateStrings(output , secondArg);
		output = concatenateStrings(output , "00\n");
		output = concatenateStrings(output , firstArgAdd);
		output = concatenateStrings(output , secondArgAdd);
		output = concatenateStrings(output , "00\n");
		return output;
		
	}
	else
	{
		/*determined first argument*/
		if (isRegister(func->arguments[0])>-1)
		{
			firstArg = "101";
		    firstArgAdd = binaryCodeWithinSize(isRegister(func->arguments[0]) , 5);
			firstArgAdd = concatenateStrings(firstArgAdd , "0000000\n");
		}
		else if(isNumber(func->arguments[0]))
		{
				firstArg = "001";
				firstArgAdd = binaryCodeWithinSize(atoi(func->arguments[0]), 5);
				firstArgAdd = concatenateStrings(firstArgAdd , "0000000\n");
		}
		else
		{
			firstArg = "011";
			if(getNumberFromWord(labelFileName , func->arguments[0]) == -1)
			{
				firstArgAdd = "000000000001";
			}
			else
			{
				firstArgAdd = binaryCodeWithinSize(getNumberFromWord(labelFileName , func->arguments[0]), 10);
				firstArgAdd = concatenateStrings(firstArgAdd , "10\n");
			}
		}
		
		/*determined second argument*/
		if (isRegister(func->arguments[1]) == 1)
		{
			
			secondArg = "101";
		    secondArgAdd = binaryCodeWithinSize(isRegister(func->arguments[1]) , 5);
			secondArgAdd = concatenateStrings(secondArgAdd , "0000000\n");
		}
		else
		{
			secondArg = "011";
			if(getNumberFromWord(labelFileName , func->arguments[1]) == -1)
			{
				secondArgAdd = "000000000001";
			}
			else
			{
				secondArgAdd = binaryCodeWithinSize(getNumberFromWord(labelFileName , func->arguments[1]), 10);
				secondArgAdd = concatenateStrings(secondArgAdd , "10\n");
			}
		}
		
		
		
	}
	
	    output = concatenateStrings(firstArg , opcode);
		output = concatenateStrings(output , secondArg);
		output = concatenateStrings(output , "00\n");
		output = concatenateStrings(output , firstArgAdd);
		output = concatenateStrings(output , secondArgAdd);

	return output;
}


char* subGetBInaryCode(Function* func , const char* labelFileName  )
{
	char* opcode = "0011";
	char* firstArg ;
	char* secondArg ;
    char*	firstArgAdd ;
   char*	secondArgAdd ;
   char* output;
	if (func->instructionCount == 2)
	{
		
		firstArg = "101";
		secondArg = "101";
		firstArgAdd = binaryCodeWithinSize(isRegister(func->arguments[0]) , 5);
		secondArgAdd =  binaryCodeWithinSize(isRegister(func->arguments[1]) , 5);
		
		output = concatenateStrings(firstArg , opcode);
		output = concatenateStrings(output , secondArg);
		output = concatenateStrings(output , "00\n");
		output = concatenateStrings(output , firstArgAdd);
		output = concatenateStrings(output , secondArgAdd);
		output = concatenateStrings(output , "00\n");
		return output;
		
	}
	else
	{
		/*determined first argument*/
		if (isRegister(func->arguments[0])>-1)
		{
			firstArg = "101";
		    firstArgAdd = binaryCodeWithinSize(isRegister(func->arguments[0]) , 5);
			firstArgAdd = concatenateStrings(firstArgAdd , "0000000\n");
		}
		else if(isNumber(func->arguments[0]))
		{
				firstArg = "001";
				firstArgAdd = binaryCodeWithinSize(atoi(func->arguments[0]), 5);
				firstArgAdd = concatenateStrings(firstArgAdd , "0000000\n");
		}
		else
		{
			firstArg = "011";
			if(getNumberFromWord(labelFileName , func->arguments[0]) == -1)
			{
				firstArgAdd = "000000000001\n";
			}
			else
			{
				firstArgAdd = binaryCodeWithinSize(getNumberFromWord(labelFileName , func->arguments[0]), 10);
				firstArgAdd = concatenateStrings(firstArgAdd , "10\n");
			}
		}
		
		/*determined second argument*/
		if (isRegister(func->arguments[1]) == 1)
		{
			
			secondArg = "101";
		    secondArgAdd = binaryCodeWithinSize(isRegister(func->arguments[1]) , 5);
			secondArgAdd = concatenateStrings(secondArgAdd , "0000000\n");
		}
		else
		{
			secondArg = "011";
			if(getNumberFromWord(labelFileName , func->arguments[1]) == -1)
			{
				secondArgAdd = "000000000001\n";
			}
			else
			{
				secondArgAdd = binaryCodeWithinSize(getNumberFromWord(labelFileName , func->arguments[1]), 10);
				secondArgAdd = concatenateStrings(secondArgAdd , "10\n");
			}
		}
		
		
		
	}
	
	    output = concatenateStrings(firstArg , opcode);
		output = concatenateStrings(output , secondArg);
		output = concatenateStrings(output , "00\n");
		output = concatenateStrings(output , firstArgAdd);
		output = concatenateStrings(output , secondArgAdd);

	return output;
}


char* notGetBInaryCode(Function* func , const char* labelFileName  )
{
	char* opcode = "0100";
	char* firstArg = "000" ;
	char* secondArg ;
    char*	firstArgAdd ;
   char* output;	

		/*determined first argument*/
		if (isRegister(func->arguments[0])>-1)
		{
			secondArg = "101";
		    firstArgAdd = binaryCodeWithinSize(isRegister(func->arguments[0]) , 5);
			firstArgAdd = concatenateStrings(firstArgAdd , "0000000\n");
		}
		
		else
		{
			secondArg = "011";
			if(getNumberFromWord(labelFileName , func->arguments[0]) == -1)
			{
				firstArgAdd = "000000000001";
			}
			else
			{
				firstArgAdd = binaryCodeWithinSize(getNumberFromWord(labelFileName , func->arguments[0]), 10);
				firstArgAdd = concatenateStrings(firstArgAdd , "10\n");
			}
		}
		
		
	
	    output = concatenateStrings(firstArg , opcode);
		output = concatenateStrings(output , secondArg);
		output = concatenateStrings(output , "00\n");
		output = concatenateStrings(output , firstArgAdd);


	return output;
}


char* clrGetBInaryCode(Function* func , const char* labelFileName  )
{
	char* opcode = "0101";
	char* firstArg = "000" ;
	char* secondArg ;
    char*	firstArgAdd ;
   char* output;	

		/*determined first argument*/
		if (isRegister(func->arguments[0])>-1)
		{
			secondArg = "101";
		    firstArgAdd = binaryCodeWithinSize(isRegister(func->arguments[0]) , 5);
			firstArgAdd = concatenateStrings(firstArgAdd , "0000000\n");
		}
		
		else
		{
			secondArg = "011";
			if(getNumberFromWord(labelFileName , func->arguments[0]) == -1)
			{
				firstArgAdd = "000000000001";
			}
			else
			{
				firstArgAdd = binaryCodeWithinSize(getNumberFromWord(labelFileName , func->arguments[0]), 10);
				firstArgAdd = concatenateStrings(firstArgAdd , "10\n");
			}
		}
		
		
	
	    output = concatenateStrings(firstArg , opcode);
		output = concatenateStrings(output , secondArg);
		output = concatenateStrings(output , "00\n");
		output = concatenateStrings(output , firstArgAdd);


	return output;
}

char* incGetBInaryCode(Function* func , const char* labelFileName  )
{
	char* opcode = "0111";
	char* firstArg = "000" ;
	char* secondArg ;
    char*	firstArgAdd ;
   char* output;
	

		/*determined first argument*/
		if (isRegister(func->arguments[0])>-1)
		{
			secondArg = "101";
		    firstArgAdd = binaryCodeWithinSize(isRegister(func->arguments[0]) , 5);
			firstArgAdd = concatenateStrings(firstArgAdd , "0000000\n");
		}
		
		else
		{
			secondArg = "011";
			if(getNumberFromWord(labelFileName , func->arguments[0]) == -1)
			{
				firstArgAdd = "000000000001\n";
			}
			else
			{
				firstArgAdd = binaryCodeWithinSize(getNumberFromWord(labelFileName , func->arguments[0]), 10);
				firstArgAdd = concatenateStrings(firstArgAdd , "10\n");
			}
		}
		
		
	
	    output = concatenateStrings(firstArg , opcode);
		output = concatenateStrings(output , secondArg);
		output = concatenateStrings(output , "00\n");
		output = concatenateStrings(output , firstArgAdd);


	return output;
}


char* decGetBInaryCode(Function* func , const char* labelFileName  )
{
	char* opcode = "1000";
	char* firstArg = "000" ;
	char* secondArg ;
    char*	firstArgAdd ;
   char* output;	

		/*determined first argument*/
		if (isRegister(func->arguments[0])>-1)
		{
			secondArg = "101";
		    firstArgAdd = binaryCodeWithinSize(isRegister(func->arguments[0]) , 5);
			firstArgAdd = concatenateStrings(firstArgAdd , "0000000\n");
		}
		
		else
		{
			secondArg = "011";
			if(getNumberFromWord(labelFileName , func->arguments[0]) == -1)
			{
				firstArgAdd = "000000000001";
			}
			else
			{
				firstArgAdd = binaryCodeWithinSize(getNumberFromWord(labelFileName , func->arguments[0]), 10);
				firstArgAdd = concatenateStrings(firstArgAdd , "10\n");
			}
		}
		
		
	
	    output = concatenateStrings(firstArg , opcode);
		output = concatenateStrings(output , secondArg);
		output = concatenateStrings(output , "00\n");
		output = concatenateStrings(output , firstArgAdd);


	return output;
}

char* jmpGetBInaryCode(Function* func , const char* labelFileName  )
{
	char* opcode = "1001";
	char* firstArg = "000" ;
	char* secondArg ;
    char*	firstArgAdd ;
   char* output;
	

		/*determined first argument*/
		if (isRegister(func->arguments[0])>-1)
		{
			secondArg = "101";
		    firstArgAdd = binaryCodeWithinSize(isRegister(func->arguments[0]) , 5);
			firstArgAdd = concatenateStrings(firstArgAdd , "0000000\n");
		}
		
		else
		{
			secondArg = "011";
			if(getNumberFromWord(labelFileName , func->arguments[0]) == -1)
			{
				firstArgAdd = "000000000001";
			}
			else
			{
				firstArgAdd = binaryCodeWithinSize(getNumberFromWord(labelFileName , func->arguments[0]), 10);
				firstArgAdd = concatenateStrings(firstArgAdd , "10\n");
			}
		}
		
		
	
	    output = concatenateStrings(firstArg , opcode);
		output = concatenateStrings(output , secondArg);
		output = concatenateStrings(output , "00\n");
		output = concatenateStrings(output , firstArgAdd);


	return output;
}

char* bneGetBInaryCode(Function* func , const char* labelFileName  )
{
	char* opcode = "1010";
	char* firstArg = "000" ;
    char*	firstArgAdd ;
char*	secondArg ;
   char* output;
	

		/*determined first argument*/
		if (isRegister(func->arguments[0])>-1)
		{
			secondArg = "101";
		    firstArgAdd = binaryCodeWithinSize(isRegister(func->arguments[0]) , 5);
			firstArgAdd = concatenateStrings(firstArgAdd , "0000000\n");
		}
		
		else
		{
			secondArg = "011";
			if(getNumberFromWord(labelFileName , func->arguments[0]) == -1)
			{
				firstArgAdd = "000000000001\n";
			}
			else
			{
				firstArgAdd = binaryCodeWithinSize(getNumberFromWord(labelFileName , func->arguments[0]), 10);
				firstArgAdd = concatenateStrings(firstArgAdd , "10\n");
			}
		}
		
		
	
	    output = concatenateStrings(firstArg , opcode);
		output = concatenateStrings(output , secondArg);
		output = concatenateStrings(output , "00\n");
		output = concatenateStrings(output , firstArgAdd);


	return output;
}


char* redGetBInaryCode(Function* func , const char* labelFileName  )
{
	char* opcode = "1011";
	char* firstArg = "000" ;
    char*	firstArgAdd ;
   char*	secondArg ;
   char* output;
	

		/*determined first argument*/
		if (isRegister(func->arguments[0])>-1)
		{
			secondArg = "101";
		    firstArgAdd = binaryCodeWithinSize(isRegister(func->arguments[0]) , 5);
			firstArgAdd = concatenateStrings(firstArgAdd , "0000000\n");
		}
		
		else
		{
			secondArg = "011";
			if(getNumberFromWord(labelFileName , func->arguments[0]) == -1)
			{
				firstArgAdd = "000000000001";
			}
			else
			{
				firstArgAdd = binaryCodeWithinSize(getNumberFromWord(labelFileName , func->arguments[0]), 10);
				firstArgAdd = concatenateStrings(firstArgAdd , "10\n");
			}
		}
		
		
	
	    output = concatenateStrings(firstArg , opcode);
		output = concatenateStrings(output , secondArg);
		output = concatenateStrings(output , "00\n");
		output = concatenateStrings(output , firstArgAdd);


	return output;
}

char* jsrGetBInaryCode(Function* func , const char* labelFileName  )
{
	char* opcode = "1100";
	char* firstArg  = "000" ;
    char*	firstArgAdd ;
	char* secondArg;
   char* output;
	

		/*determined first argument*/
		if (isRegister(func->arguments[0])>-1)
		{
			secondArg = "101";
		    firstArgAdd = binaryCodeWithinSize(isRegister(func->arguments[0]) , 5);
			firstArgAdd = concatenateStrings(firstArgAdd , "0000000\n");
		}
		
		else
		{
			secondArg = "011";
			if(getNumberFromWord(labelFileName , func->arguments[0]) == -1)
			{
				firstArgAdd = "000000000001";
			}
			else
			{
				firstArgAdd = binaryCodeWithinSize(getNumberFromWord(labelFileName , func->arguments[0]), 10);
				firstArgAdd = concatenateStrings(firstArgAdd , "10\n");
			}
		}
		
		
	
	    output = concatenateStrings(firstArg , opcode);
		output = concatenateStrings(output , secondArg);
		output = concatenateStrings(output , "00\n");
		output = concatenateStrings(output , firstArgAdd);


	return output;
}


char* leaGetBInaryCode(Function* func , const char* labelFileName  )
{
	char* opcode = "0110";
	char* firstArg ;
	char* secondArg ;
    char*	firstArgAdd ;
   char*	secondArgAdd ;
   char* output;
	

		/*determined first argument*/
		firstArg = "011";
		if(getNumberFromWord(labelFileName , func->arguments[0]) == -1)
		{
			firstArgAdd = "000000000001";
		}
		else
		{
			firstArgAdd = binaryCodeWithinSize(getNumberFromWord(labelFileName , func->arguments[0]), 10);
			firstArgAdd = concatenateStrings(firstArgAdd , "10\n");
		}
		
		
		
	
	   /*determined second argument*/
		if (isRegister(func->arguments[1]) == 1)
		{
			
			secondArg = "101";
		    secondArgAdd = binaryCodeWithinSize(isRegister(func->arguments[1]) , 5);
			secondArgAdd = concatenateStrings(secondArgAdd , "0000000\n");
		}
		else
		{
			secondArg = "011";
			if(getNumberFromWord(labelFileName , func->arguments[1]) == -1)
			{
				secondArgAdd = "000000000001";
			}
			else
			{
				secondArgAdd = binaryCodeWithinSize(getNumberFromWord(labelFileName , func->arguments[1]), 10);
				secondArgAdd = concatenateStrings(secondArgAdd , "10\n");
			}
		}
		
		
		
	
	
	    output = concatenateStrings(firstArg , opcode);
		output = concatenateStrings(output , secondArg);
		output = concatenateStrings(output , "00\n");
		output = concatenateStrings(output , firstArgAdd);
		output = concatenateStrings(output , secondArgAdd);

	return output;
}



char* prnGetBInaryCode(Function* func , const char* labelFileName  )
{
	char* opcode = "0110";
	char* firstArg = "000" ;
	char* secondArg ;
    char*	firstArgAdd ;
   char* output;
	

		/*determined first argument*/
		if (isRegister(func->arguments[0])>-1)
		{
			secondArg = "101";
		    firstArgAdd = binaryCodeWithinSize(isRegister(func->arguments[0]) , 5);
			firstArgAdd = concatenateStrings(firstArgAdd , "0000000\n");
		}
		else if(isNumber(func->arguments[0]))
		{
				secondArg = "001";
				firstArgAdd = binaryCodeWithinSize(atoi(func->arguments[0]), 5);
				firstArgAdd = concatenateStrings(firstArgAdd , "0000000\n");
		}
		else
		{
			secondArg = "011";
			if(getNumberFromWord(labelFileName , func->arguments[0]) == -1)
			{
				firstArgAdd = "000000000001";
			}
			else
			{
				firstArgAdd = binaryCodeWithinSize(getNumberFromWord(labelFileName , func->arguments[0]), 10);
				firstArgAdd = concatenateStrings(firstArgAdd , "10\n");
			}
		
		
	}
	
	    output = concatenateStrings(firstArg , opcode);
		output = concatenateStrings(output , secondArg);
		output = concatenateStrings(output , "00\n");
		output = concatenateStrings(output , firstArgAdd);


	return output;
}

char* rtsGetBInaryCode(Function* func )
{return "000111000000\n";}

char* stopGetBInaryCode(Function* func )
{return "000111100000\n";}


char* strGetBInaryCode(Function* func )
{

	char* output = "";
	char*temp;
	int i = 0 ;
	for (i  = 1 ; i < func->instructionCount-1 ; i++)
	{

		temp = binaryCodeWithinSize(getAsciiFromChar(getCharAtIndex(func->arguments[0] , i)) , 12);
	    temp = concatenateStrings(temp , "\n");
		output = concatenateStrings(output , temp);

	}
	output = concatenateStrings(output , "000000000000\n");
	return output;
}



char* dataGetBInaryCode(Function* func )
{
	
	char* output = "";
	char*temp;
	int i = 0 ;
	for (i  = 0 ; i < func->argumentCount ; i++)
	{
		temp = binaryCodeWithinSize(atoi(func->arguments[i])  , 12);
	    temp = concatenateStrings(temp , "\n");
		output = concatenateStrings(output , temp);
	}
	
	return output;
}

char* functionBinaryCode(Function* function , const char* labelFileName)
{

	switch(nameOfOpCode(function->name) )
	{
		case 1: 
			return movGetBInaryCode(function , labelFileName);
			break;
		case 2: 
			return cmpGetBInaryCode(function , labelFileName);
			break;
		case 3: 
			return addGetBInaryCode(function , labelFileName);
			break;
		case 4: 
			return subGetBInaryCode(function , labelFileName);
			break;
		case 5: 
			return notGetBInaryCode(function , labelFileName);
			break;
		case 6: 
			return clrGetBInaryCode(function , labelFileName);
			break;
		case 7: 
			return leaGetBInaryCode(function , labelFileName);
			break;
		case 8: 
			return incGetBInaryCode(function , labelFileName);
			break;
		case 9: 
			return decGetBInaryCode(function , labelFileName);
			break;
		case 10: 
			return jmpGetBInaryCode(function , labelFileName);
			break;
		case 11: 
			return bneGetBInaryCode(function , labelFileName);
			break;
		case 12: 
			return redGetBInaryCode(function , labelFileName);
			break;
		case 13: 
			return prnGetBInaryCode(function , labelFileName);
			break;
		case 14: 
			return jsrGetBInaryCode(function , labelFileName);
			break;
		case 15: 
			return rtsGetBInaryCode(function );
			break;
		case 16: 
			return stopGetBInaryCode(function );
			break;
		case 17: 
			return dataGetBInaryCode(function);
			break;
		case 18: 
			return strGetBInaryCode(function);
			break;
	    default:
			printf("%s -no such function\n" , function->name);
			return NULL;
			break;
	}
	
	return 0 ;
}


int listGetBInary(Function *head , const char* labelFileName , const char* objFileName )
{
	

	Function* current = head;
	while (current != NULL) {
		appendStringToFile(objFileName , functionBinaryCode(current , labelFileName));
        current = current->next;
	}	
	return 0;
}


