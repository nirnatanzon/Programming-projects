#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <ctype.h>
#include "Opcode.h"
#include "function.h"
#include "myfile.h"



/**
 * Processes a macro block in the input file by extracting the macro name, content, and replacing occurrences in the output file.
 *
 * @param input_filename   The name of the input file.
 * @param output_filename  The name of the output file.
 */
int macro_block(const char* input_filename, const char* output_filename) ;
int entry_block(const char* input_filename, const char* output_filename ) ;
int extren_block(const char* input_filename, const char* output_filename) ;


	

int main(int argc, char *argv[]) {
	int i ;
	int j;
	 char *fileName;
	char *amFile;
	char *extFile ;
	char *entFile ;
	char *lbFile ;
	char *objFile;
	char error = 0;
	char  instruction[20] ;
	char  data[20]; 
	int line = 0;
    int count = 0 ;
	char * lbl ;
	Function* head = NULL;
	
    if (argc < 2) {
        printf("Usage: %s FILE_NAME1 FILE_NAME2 FILE_NAME3 ...\n", argv[0]);
        return 1;
    }
	
	
	for (i = 1; i < argc; i++) {
        fileName = argv[i];
		/*create files names"*/
		fileName =  concatenateStrings(fileName , ".as");
        amFile = getFileName(fileName ) ;
		amFile = concatenateStrings(amFile , ".am");
		extFile = getFileName(fileName ) ;
		extFile = concatenateStrings(extFile , ".ext");
		entFile = getFileName(fileName ) ;
		entFile = concatenateStrings(entFile , ".ent");
		lbFile = getFileName(fileName ) ;
		lbFile = concatenateStrings(lbFile , ".lb");
		objFile = getFileName(fileName ) ;
		objFile = concatenateStrings(objFile , ".ob");
		
		/*make sure the files are empty"*/
		clearFile(amFile);
		clearFile(extFile);
		clearFile(entFile);
		clearFile(lbFile);
		clearFile(objFile);
	
		
	

		
		/*make sure the file is excit"*/
		error = isExcited(fileName);
		if (error  != NO_ERROR)
		{
			remove(amFile);
			remove(entFile);
			remove(extFile);
			remove(lbFile);
			remove(objFile);
			printError(error);
			return -1;
		}
		

		/*macro"*/
		error = macro_block( fileName,amFile);
		if (error  != NO_ERROR)
		{
			printError(error);
			remove(amFile);
			remove(entFile);
			remove(extFile);
			remove(lbFile);
			remove(objFile);
			return -1;
		}
			

		/*entry"*/
		error=entry_block(amFile , entFile );
		if (error  != NO_ERROR)
		{
			printError(error);
			remove(amFile);
			remove(entFile);
			remove(extFile);
			remove(lbFile);
			remove(objFile);
			return -1;
		}
		
		
		/*extern"*/
		error=extren_block(amFile , extFile );
		if (error  != NO_ERROR)
		{
			printError(error);
			remove(amFile);
			remove(entFile);
			remove(extFile);
			remove(lbFile);
			remove(objFile);
			return -1;
		}
		
		/*label"*/
		printFileLabels(amFile , lbFile);
		

		/*beautify .am&.lb files"*/
		beautifyAssembly(amFile);
		deleteEmptyLines(amFile );
        mergeFiles(extFile  ,lbFile);
		replaceTabsWithSpaces(lbFile);
		beautifyAssembly(lbFile);
		deleteEmptyLines(lbFile);
		
		/*create linked list of the functions*/
		head = createFunctionListFromFile(amFile);
		
		/*make sure the list is valid and add no errors*/
		error = listValidator(head ,lbFile);
		if (error  != NO_ERROR)
		{
			remove(amFile);
			remove(entFile);
			remove(extFile);
			remove(lbFile);
			remove(objFile);
			freeFunctionList(head);
			return -1;
		}
		
		/*calculate the amount of machin oredr for every line*/
		listInstructionCount(head , instruction , data);
		
		
		
		if(DEBUG>=2)
			printFunctions(head);
	
		/*update .ext , .ent and lb file */
		 j = 0;
		 
		 lbl = getLabelFromLine(lbFile , j) ;
		 while(lbl !=NULL)
		 {
			line  = getNumberFromWord(lbFile , lbl)-1;
			if(line == -1)
				update_number_in_file(lbFile , lbl , 100);
			else
			{
				count  = sumInstructionCount(head , line);
				if (count > 0)
				{
					update_number_in_file(lbFile , lbl , count+100);
					update_number_in_file(entFile , lbl , count+100);

				}
			}
			j++;
			lbl = getLabelFromLine(lbFile , j) ;
		 }
		 
		j = 0;
        lbl = getLabelFromLine(extFile , j) ;
		 while(lbl !=NULL)
		 {
			line  = getNumberFromWord(extFile , lbl)-1;
			if(line == -1)
				update_number_in_file(lbFile , lbl , 100);
			else
			{
				count  = sumInstructionCount(head , line);
				if (count > 0)
				{
					update_number_in_file(extFile , lbl , count+100);
				}
			}
			j++;
			lbl = getLabelFromLine(lbFile , j) ;
		 }
        
		/* convert the linke list into a binary code file*/
        listGetBInary( head , lbFile , objFile);
		beautifyAssembly(objFile);
	    deleteEmptyLines(objFile);
		remove(lbFile);
		/* convert the binary code file into base64 file*/
		convert_binary_file_to_base64(objFile , instruction , data);

		
		freeFunctionList(head);
			
	}
   remove(lbFile);
   printf("\n\nCompiling was done successfully!\n\n");
	return 1;
}





int macro_block(const char* input_filename, const char* output_filename) {
    const char* startWord = "mcro";
    const char* stopWord = "endmcro";
    char* result = NULL;
    char* mcro_name = NULL;
    char* content = NULL;
	clearFile(output_filename);
    result = cutStringBetweenWords(input_filename, output_filename, startWord, stopWord);
    mcro_name = extract_mcro_name(result); /* Extract the macro name from the result */
	if( findStringInFile(input_filename , concatenateStrings("mcro " , mcro_name)) >  findStringInFile(input_filename , mcro_name))
			return MACRO_USED_BEFORE_DECLERATION;
	if (nameOfOpCode(mcro_name))
			return NOT_VALID_MACRO;
    content = extract_macro_content(result); /* Extract the macro content from the result */
    replaceWordInFile(output_filename, mcro_name, content); /* Replace occurrences of the macro name with the macro content in the output file */
    
    result = cutStringBetweenWords(output_filename, output_filename, startWord, stopWord);
    while (result != NULL) {
        mcro_name = extract_mcro_name(result); /* Extract the macro name from the result */
		if( findStringInFile(input_filename , concatenateStrings("mcro " , mcro_name)) >  findStringInFile(input_filename , mcro_name))
			return MACRO_USED_BEFORE_DECLERATION;
		if (nameOfOpCode(mcro_name))
			return NOT_VALID_MACRO;
		
        content = extract_macro_content(result); /* Extract the macro content from the result */
        replaceWordInFile(output_filename, mcro_name, content); /* Replace occurrences of the macro name with the macro content in the output file */
        result = NULL;

    }

		beautifyAssembly(output_filename); /* Beautify the assembly code in the output file */
		deleteLinesWithNonSemicolonFirstChar(output_filename); /* Delete lines in the output file that do not start with ';' or whitespace */
		
	return NO_ERROR;
}

int entry_block(const char* input_filename, const char* output_filename ) {
	 char* str ;
	str = findWordInFile(input_filename, ".entry");
    
	if(str != NULL)
		clearFile(output_filename);
     /* Loop while there are occurrences of ".entry" in the file*/
    while (str != NULL) {

         /* Delete the line starting with the word ".entry" */
        deleteLineStartingWithWord(input_filename, ".entry");

         /* Check if the line contains more than one word */
        if (countWords(str) > 1) {
             /* If there are multiple words, return an illegal label error code  */
            return ILLEGAL_LABEL;
        } else {
             /* If there is only one word, append it to the "main.ent" file */
            appendStringToFile(output_filename, concatenateStrings(getStringBeforeChar(str,'\n') , ":  -1"));

        }

         /* Find the next occurrence of ".entry" in the output file */
        str = findWordInFile(input_filename, ".entry");
        
    }

     /*Beautify the assembly code in the output file*/
    beautifyAssembly(output_filename);
	deleteEmptyLines(output_filename);



	return NO_ERROR;
}


int extren_block(const char* input_filename, const char* output_filename ) {
	char* str;
    char *tempStr;
	str= findWordInFile(input_filename, ".extern ");
	if(str != NULL)
		clearFile(output_filename);
    /* Loop while there are occurrences of ".entry" in the file */
    while (str != NULL) {
        /* Delete the line starting with the word ".entry" */
        deleteLineStartingWithWord(input_filename, ".extern ");

        /* Check if the line contains more than one word */
        if (countWords(str) > 1) {
            /* If there are multiple words, return an illegal label error code */
            return ILLEGAL_LABEL;
        } else {
            /* If there is only one word, append it to the "main.ent" file */
			tempStr = intToString(findStringInFile(input_filename , str));
			tempStr = concatenateStrings(":  " , tempStr);
            appendStringToFile(output_filename, concatenateStrings(getStringBeforeChar(str,'\n') , tempStr));
        }

        /* Find the next occurrence of ".extern" in the output file */
        str = findWordInFile(input_filename, ".extern ");
       
    }

    /* Beautify the assembly code in the output file */\
    beautifyAssembly(output_filename);
    deleteEmptyLines(output_filename);

	return NO_ERROR;
}




