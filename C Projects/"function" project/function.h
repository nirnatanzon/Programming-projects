#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <ctype.h>
#include "myfile.h"

typedef struct Function {
    char* name;
    char** arguments;
    size_t argumentCount;
    unsigned int instructionCount;
    struct Function* next;
} Function;


Function* createFunction(char* name, char** arguments, size_t argumentCount, unsigned int instructionCount);

void insertFunctionEnd(Function** head, Function* newFunction) ;

void printFunctions(Function* head) ;

void freeFunctionsList(Function* head) ;

void freeFunction(Function* function) ;

int disassembleAssembly(const char* assemblyLine, Function** func) ;

Function* copyFunction(const Function* source) ;

Function* createFunctionListFromFile(const char* fileName) ;

void freeFunctionList(Function* head) ;

int listValidator(Function *head, const char* labelFileName);

int functionValidator(Function* function , const char* labelFileName);

int movValidator(Function* function, const char* labelFileName);

int  cmpValidator(Function* function, const char* labelFileName);

int addValidator(Function* function, const char* labelFileName);

int subValidator(Function* function, const char* labelFileName);

int notValidator(Function* function, const char* labelFileName);

int clrValidator(Function* function, const char* labelFileName);

int leaValidator(Function* function, const char* labelFileName);

int incValidator(Function* function, const char* labelFileName);

int decValidator(Function* function, const char* labelFileName);

int jmpValidator(Function* function, const char* labelFileName);

int bneValidator(Function* function, const char* labelFileName);

int redValidator(Function* function, const char* labelFileName);

int jsrValidator(Function* function, const char* labelFileName);

int prnValidator(Function* function, const char* labelFileName);

int rtsValidator(Function* function);

int stopValidator(Function* function);

int dataValidator(Function* function);

int stringValidator(Function* function, const char* labelFileName);

int MachineCodeType1(Function* function) ;

int MachineCodeType2(Function* function) ;

int MachineCodeType3(Function* function) ;

int MachineCodeType4(Function* function) ;

int MachineCodeType5(Function* function) ;

int  machineCodeString(Function* function);

int functionCounter(Function* function );

int listInstructionCount(Function *head , char * instruction , char *  data);

unsigned int sumInstructionCount(Function* head, size_t index) ;

char* binaryCodeWithinSize(int num1, int num2) ;

char* movGetBInaryCode(Function* func , const char* labelFileName  );

char* cmpGetBInaryCode(Function* func , const char* labelFileName  );

char* addGetBInaryCode(Function* func , const char* labelFileName  );

char* subGetBInaryCode(Function* func , const char* labelFileName  );

char* notGetBInaryCode(Function* func , const char* labelFileName  );

char* clrGetBInaryCode(Function* func , const char* labelFileName  );

char* incGetBInaryCode(Function* func , const char* labelFileName  );

char* decGetBInaryCode(Function* func , const char* labelFileName  );

char* jmpGetBInaryCode(Function* func , const char* labelFileName  );

char* bneGetBInaryCode(Function* func , const char* labelFileName  );

char* redGetBInaryCode(Function* func , const char* labelFileName  );

char* jsrGetBInaryCode(Function* func , const char* labelFileName  );

char* leaGetBInaryCode(Function* func , const char* labelFileName  );

char* prnGetBInaryCode(Function* func , const char* labelFileName  );

char* rtsGetBInaryCode(Function* func );

char* stopGetBInaryCode(Function* func );

char* strGetBInaryCode(Function* func );

char* dataGetBInaryCode(Function* func );

char* functionBinaryCode(Function* function , const char* labelFileName);

int listGetBInary(Function *head , const char* labelFileName , const char* objFileName );
