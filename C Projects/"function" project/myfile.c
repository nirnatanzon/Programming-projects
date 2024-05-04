#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <ctype.h>
#include "myfile.h"




/**
 * Replaces occurrences of a word in a file with a specified string.
 *
 * @param filename    The name of the input file.
 * @param word        The word to be replaced.
 * @param string      The string to replace the word with.
 */
void replaceWordInFile(const char* filename, const char* word, const char* string) {
    FILE* file;
    FILE* tempFile;
    char buffer[1024];
    char tempBuffer[1024];
    char* foundWord;
    /*long currentPosition;*/
    int wordLength = strlen(word);

    file = fopen(filename, "r+"); /* Open the file in read and write mode */
    if (file == NULL) {
        if (DEBUG >= 3)
            printf("Failed to open the file.\n"); /* Print an error message if the file opening fails */
        return;
    }

    tempFile = fopen("tempfile.txt", "w+"); /* Open a temporary file in read and write mode */
    if (tempFile == NULL) {
        if (DEBUG >= 3)
            printf("Failed to create a temporary file.\n"); /* Print an error message if the temporary file creation fails */
        fclose(file);
        return;
    }

    /* Read each line from the file */
    while (fgets(buffer, sizeof(buffer), file) != NULL) {
        /*currentPosition = ftell(file) - strlen(buffer);  */

        /* Replace occurrences of the word with the specified string */
        while ((foundWord = strstr(buffer, word)) != NULL) {
            strncpy(tempBuffer, buffer, foundWord - buffer); /* Copy the content before the found word */
            tempBuffer[foundWord - buffer] = '\0';
            strcat(tempBuffer, string); /* Append the replacement string */
            strcat(tempBuffer, foundWord + wordLength); /* Append the remaining content after the found word */
            strcpy(buffer, tempBuffer); /* Copy the modified line back to the buffer */
        }

        fputs(buffer, tempFile); /* Write the modified line to the temporary file */
    }

    rewind(tempFile); /* Reset the position in the temporary file */
    rewind(file); /* Reset the position in the original file */

    /* Copy the content from the temporary file back to the original file */
    while (fgets(buffer, sizeof(buffer), tempFile) != NULL) {
        fputs(buffer, file);
    }
	fclose(tempFile);
    fclose(file); /* Close the original file */
    if (DEBUG >= 3)
        printf("Content replaced in the original file '%s'.\n", filename); /* Print a success message */
	
	remove("tempfile.txt");
}




/**
 * Cuts the string between specified start and stop words in a file.
 *
 * @param filename    The name of the input file.
 * @param outFilename The name of the output file.
 * @param startWord   The word marking the start of the string to be cut.
 * @param stopWord    The word marking the end of the string to be cut.
 * @return            The cut string if successful, or NULL if an error occurs.
 */
char* cutStringBetweenWords(const char* filename, const char* outFilename, const char* startWord, const char* stopWord) {
    FILE* file;
    FILE* newFile;
    char* fileContent;
    char* cutString;
    char* startPtr;
    char* stopPtr;
    long fileSize;
    size_t bytesRead;
    size_t cutSize;

    file = fopen(filename, "r"); /* Open the input file in read mode */
    if (file == NULL) {
        if (DEBUG >= 3)
            printf("Error opening file %s\n", filename); /* Print an error message if the file opening fails */
        return NULL;
    }

    fseek(file, 0L, SEEK_END); /* Move the file pointer to the end of the file */
    fileSize = ftell(file); /* Get the current position of the file pointer, which gives the file size */
    rewind(file); /* Move the file pointer back to the beginning of the file */

    fileContent = (char*)malloc(fileSize + 1); /* Allocate memory to store the file content */
    if (fileContent == NULL) {
        if (DEBUG >= 3)
            printf("cutStringBetweenWords Memory allocation failed\n"); /* Print an error message if memory allocation fails */
        fclose(file);
        return NULL;
    }

    bytesRead = fread(fileContent, 1, fileSize, file); /* Read the file content into the allocated memory */
    fileContent[bytesRead] = '\0'; /* Null-terminate the file content string */

    fclose(file); /* Close the input file */

    startPtr = strstr(fileContent, startWord); /* Find the position of the start word in the file content */
    stopPtr = strstr(fileContent, stopWord); /* Find the position of the stop word in the file content */
    if (startPtr == NULL || stopPtr == NULL) {
        if (DEBUG >= 2)
            printf("Start or stop word not found\n"); /* Print an error message if either the start or stop word is not found */
        free(fileContent);
        return NULL;
    }

    cutSize = stopPtr - (startPtr + strlen(startWord)); /* Calculate the size of the cut string */

    cutString = (char*)malloc(cutSize + 1); /* Allocate memory for the cut string */
    if (cutString == NULL) {
        if (DEBUG >= 3)
            printf("cutStringBetweenWords Memory allocation failed\n"); /* Print an error message if memory allocation fails */
        free(fileContent);
        return NULL;
    }

    strncpy(cutString, startPtr + strlen(startWord), cutSize); /* Copy the cut string into the allocated memory */
    cutString[cutSize] = '\0'; /* Null-terminate the cut string */

    newFile = fopen(outFilename, "w"); /* Open the output file in write mode */
    if (newFile == NULL) {
        if (DEBUG >= 3)
            printf("Error creating file %s\n", outFilename); /* Print an error message if the output file creation fails */
        free(fileContent);
        free(cutString);
        return NULL;
    }

    fwrite(fileContent, 1, startPtr - fileContent, newFile); /* Write the content before the start word to the output file */
    fwrite(stopPtr + strlen(stopWord), 1, fileContent + bytesRead - (stopPtr + strlen(stopWord)), newFile); /* Write the content after the stop word to the output file */

    fclose(newFile); /* Close the output file */
    free(fileContent); /* Free the memory allocated for file content */
    if (DEBUG >= 2)
        printf("Modified text saved to %s\n", outFilename); /* Print a success message */
    return cutString; /* Return the cut string*/
}



/**
 * Extracts the first word (macro name) from a given input string.
 *
 * @param input       The input string.
 * @return            The extracted macro name if successful, or NULL if the input is empty or memory allocation fails.
 */
char* extract_mcro_name(const char* input) {
    const char* start;
    const char* end;
    size_t length;
    char* firstWord;

    if (input == NULL || strlen(input) == 0)
        return NULL;

    start = input;
    while (*start != '\0' && (*start == ' ' || *start == '\t'))
        start++;

    end = start;
    while (*end != '\0' && *end != ' ' && *end != '\t')
        end++;

    length = (size_t)(end - start);

    firstWord = (char*)malloc(length + 1);
    if (firstWord == NULL) {
        /* Memory allocation failed, print an error message */
        printf("extract_mcro_name Memory allocation failed\n");
        return NULL;
    }

    strncpy(firstWord, start, length);
    firstWord[length] = '\0';

    return firstWord;
}




/**
 * Extracts the content (text after the macro name) from a given input string.
 *
 * @param input       The input string.
 * @return            The extracted macro content if successful, or NULL if the input is empty or memory allocation fails.
 */
char* extract_macro_content(const char* input) {
    const char* start;
    const char* end;
    size_t length;
    char* content;

    if (input == NULL || strlen(input) == 0)
        return NULL;

    start = input;
    while (*start != '\0' && (*start == ' ' || *start == '\t'))
        start++;

    end = start;
    while (*end != '\0' && *end != ' ' && *end != '\t')
        end++;

    while (*end != '\0' && (*end == ' ' || *end == '\t'))
        end++;

    length = strlen(end);

    content = (char*)malloc(length + 1);
    if (content == NULL) {
        /* Memory allocation failed, print an error message if DEBUG is enabled */
        if (DEBUG>=3)
            printf("extract_macro_content Memory allocation failed\n");
        return NULL;
    }

    strcpy(content, end);

    return content;
}



/**
 * Beautifies an assembly file by removing redundant whitespace.
 *
 * @param filename    The name of the input assembly file.
 */
void beautifyAssembly(const char* filename) {
    FILE* inputFile;
    FILE* tempFile;
    FILE* outputFile;
    int previousChar = ' ';
    int currentChar;
    int emptyLine = 1;

    inputFile = fopen(filename, "r");
    if (inputFile == NULL) {
        /* Failed to open the file, print an error message if DEBUG is enabled */
        if (DEBUG >= 3)
            printf("beautifyAssembly Failed to open the file.\n");
        return;
    }

    tempFile = tmpfile();
    if (tempFile == NULL) {
        /* Failed to create a temporary file, print an error message if DEBUG is enabled */
        if (DEBUG >= 3)
            printf("beautifyAssembly Failed to create a temporary file.\n");
        fclose(inputFile);
        return;
    }

    while ((currentChar = fgetc(inputFile)) != EOF) {
        if (isspace(currentChar)) {
            if (currentChar == '\n') {
                emptyLine = 1;
            }
            if (previousChar == '\n' || previousChar == ' ' || previousChar == '\t' ) {
                continue;
            }
        } else {
            if (emptyLine) {
                fputc('\n', tempFile);
                emptyLine = 0;
            }
        }

        fputc(currentChar, tempFile);
        previousChar = currentChar;
    }

    fclose(inputFile);
    rewind(tempFile);

    outputFile = fopen(filename, "w");
    if (outputFile == NULL) {
        /* Failed to open the file for writing, print an error message if DEBUG is enabled */
        if (DEBUG >= 3)
            printf("beautifyAssembly Failed to open the file for writing.\n");
        fclose(tempFile);
        return;
    }

    while ((currentChar = fgetc(tempFile)) != EOF) {
        fputc(currentChar, outputFile);
    }

    fclose(outputFile);
    fclose(tempFile);

    /* Beautification completed successfully, print a message if DEBUG is enabled */
    if (DEBUG >= 2)
        printf("Beautification completed successfully.\n");
}




/**
 * Adds a tab character at the end of a file.
 *
 * @param file    The file to which the tab character should be added.
 */
void addTabToEndOfFile(FILE *file) {
    fseek(file, 0, SEEK_END); /* Move the file pointer to the end of the file */

    fprintf(file, "\n\t"); /* Write a newline character followed by a tab character to the end of the file */

    rewind(file); /* Reset the file pointer to the beginning of the file */
}



/**
 * Opens a file in read mode.
 *
 * @param filename    The name of the file to be opened.
 * @return            A pointer to the opened file if successful, or NULL if the file opening fails.
 */
FILE* openFile(const char* filename) {
    FILE* file = fopen(filename, "r");
    if (file == NULL) {
		if (DEBUG >=3)
			printf("openFile Failed to open the file.\n"); /* Print an error message if the file opening fails */
        return NULL;
    }
    return file;
}



/**
 * Deletes lines from a file that do not start with a semicolon (';') or whitespace.
 *
 * @param filename    The name of the input file.
 */
void deleteLinesWithNonSemicolonFirstChar(const char* filename) {
    FILE* file;
    FILE* temp;
    char line[MAX_LINE_LENGTH];

    file = fopen(filename, "r");
    if (file == NULL) {
        /* Failed to open the file, print an error message if DEBUG is enabled */
        if (DEBUG >= 3)
            printf("deleteLinesWithNonSemicolonFirstChar Failed to open the file.\n");
        return;
    }

    temp = fopen("temp.txt", "w");
    if (temp == NULL) {
        /* Failed to create a temporary file, print an error message if DEBUG is enabled */
        if (DEBUG >= 3)
            printf("deleteLinesWithNonSemicolonFirstChar Failed to create a temporary file.\n");
        fclose(file);
        return;
    }

    while (fgets(line, sizeof(line), file)) {
        if (line[0] != ';' && !isspace((unsigned char)line[0])) {
            fputs(line, temp); /* Write the line to the temporary file if it does not start with ';' or whitespace */
        }
    }

    fclose(file);
    fclose(temp);

    if (remove(filename) != 0) {
        /* Failed to delete the original file, print an error message if DEBUG is enabled */
        if (DEBUG >= 3)
            printf("deleteLinesWithNonSemicolonFirstChar Failed to delete the original file.\n");
        return;
    }

    if (rename("temp.txt", filename) != 0) {
        /* Failed to rename the temporary file, print an error message if DEBUG is enabled */
        if (DEBUG >= 3)
            printf("deleteLinesWithNonSemicolonFirstChar Failed to rename the temporary file.\n");
        return;
    }

    /* Lines starting with ';' have been deleted successfully, print a message if DEBUG is enabled */
    if (DEBUG >= 2)
        printf("Lines starting with ';' have been deleted successfully.\n");
}


/**
 * Checks if a file is excited by attempting to open it.
 *
 * @param filename  The name of the file to check.
 * @return          Returns 1 if the file is excited (successfully opened), or 0 otherwise.
 */
int isExcited(const char* filename) {
    FILE* inputFile = fopen(filename, "r");
    if (inputFile == NULL) {
        return 2; /* Return 2 to indicate that the file is not excited */
    }

    fclose(inputFile);
    return 0; /* Return 0 to indicate that the file is excited */
}




/**
 * Retrieves the substring of a given string before the specified character.
 *
 * @param str  The input string.
 * @param ch   The character to search for.
 * @return     Returns the substring before the specified character, or NULL if the character is not found.
 */
char* getStringBeforeChar(const char* str, char ch) {
    char* result = NULL;
    char* charPtr = strchr(str, ch);
    size_t length;
    if (charPtr != NULL) {
        length = charPtr - str;
        result = (char*)malloc(length + 1);
        if (result != NULL) {
            memcpy(result, str, length);
            result[length] = '\0';
        }
    }
    return result; /* Returns the substring before the specified character or NULL if the character is not found */
}



/**
 * Extracts labels from a file and prints them along with their line numbers.
 *
 * @param filename      The name of the input file.
 * @param outFilename   The name of the output file to write the labels and line numbers.
 */
void printFileLabels(const char* filename, const char* outFilename) {
    int counter = 0;
    FILE* file = fopen(filename, "r");
	FILE* newFile = fopen(outFilename, "w+");
	char buffer[MAX_LINE_LENGTH];
	char* temp = "sdddfsf";
    if (file == NULL) {
		if (DEBUG >=3)
			printf("printFileLabels Failed to open the file.\n");
        return;
    }
	
    
    if (newFile == NULL) {
		if (DEBUG >=3)
			printf("printFileLabels Failed to open the file.\n");
        fclose(file);
        return;
    }
	if(DEBUG >=2)
		printf("Labels - ");
    
    
	
    while (fgets(buffer, sizeof(buffer), file) != NULL) {
        temp = getStringBeforeChar(buffer, ':');
        if (temp != NULL) {
			strcat(temp , ":");
			if(DEBUG >=2)
				printf("%s   " , temp);
	
	
			fprintf(newFile, "%s\t%d\n", temp, counter);
			
			deleteStringFromFile(filename , temp );
            temp = NULL;
        }
        counter++;
    }

    fclose(file);
    fclose(newFile);
	
}




/* Function to concatenate two strings */
char* concatenateStrings(char* str1, char* str2) {
    size_t len1 = strlen(str1);
    size_t len2 = strlen(str2);

    /* Allocate memory for the concatenated string */
    char* concatenated = (char*)malloc(len1 + len2 + 1);
    if (concatenated == NULL) {
        printf("Memory allocation failed\n");
        return NULL;
    }

    strcpy(concatenated, str1);
    strcat(concatenated, str2);

    return concatenated;
}

/* Function to beautify a string (remove redundant spaces) */
char* beautifyString(const char* input) {
    int i = 0;
    size_t inputLength;
    char* beautifiedString;
    size_t currentIndex = 0;
    int previousChar = ' ';
    char currentChar;

    if (input == NULL) {
        return NULL;
    }

    inputLength = strlen(input);
    beautifiedString = (char*)malloc((inputLength * 2) + 1);
    if (beautifiedString == NULL) {
        return NULL;
    }

    for (i = 0; i < inputLength; i++) {
        currentChar = input[i];

        if (isspace(previousChar) && isspace(currentChar)) {
            continue;
        }

        beautifiedString[currentIndex] = currentChar;
        currentIndex++;
        previousChar = currentChar;
    }

    beautifiedString[currentIndex] = '\0';

    return beautifiedString;
}

/* Function to find a string in a file and return its line number */
int findStringInFile(const char* filename, const char* searchString) {
    char line[MAX_LINE_LENGTH];
    int lineNumber = 1;
    FILE* file = fopen(filename, "r");
    if (file == NULL) {
        printf("Failed to open the file.\n");
        return -1;
    }

    while (fgets(line, sizeof(line), file)) {
        if (strstr(beautifyString(line), beautifyString(searchString)) != NULL) {
            fclose(file);
            return lineNumber;
        }
        lineNumber++;
    }

    fclose(file);
    return -1;
}

/* Function similar to strdup */
char* strdup(const char* str) {
    size_t len = strlen(str) + 1;
    char* new_str = (char*)malloc(len);
    if (new_str != NULL) {
        strcpy(new_str, str);
    }
    return new_str;
}

/* Function to remove a word from a string */
char* removeWordFromString(const char* line, const char* word) {
    char* result = strdup(line);
    char* match = strstr(result, word);

    if (match != NULL) {
        memmove(match, match + strlen(word), strlen(match + strlen(word)) + 1);
        result = realloc(result, strlen(result) - strlen(word) + 1);
    }

    return result;
}

/* Function to find a word in a file and return the rest of the line */
char* findWordInFile(const char* filename, const char* word) {
    char line[MAX_LINE_LENGTH];
    char* result = NULL;
    FILE* file = fopen(filename, "r");
    size_t wordLength;
    size_t lineLength;
    size_t restLength;
    char* found;

    if (file == NULL) {
        printf("Unable to open the file.\n");
        return NULL;
    }

    while (fgets(line, sizeof(line), file) != NULL) {
        found = strstr(line, word);
        if (found != NULL) {
            wordLength = strlen(word);
            lineLength = strlen(line);
            restLength = lineLength - (found - line) - wordLength;

            result = malloc((restLength + 1) * sizeof(char));
            strncpy(result, found + wordLength, restLength);
            result[restLength] = '\0';

            break;
        }
    }

    fclose(file);
    return result;
}




char* removeWhitespace(const char* str) {
	int i;
	size_t len;
	char* result;
	 size_t j = 0; 
    if (str == NULL)
        return NULL;

    len = strlen(str);
    result = malloc((len + 1) * sizeof(char));

    if (result == NULL)
        return NULL;

    for ( i = 0; i < len; i++) {
        if (!isspace(str[i])) {
            result[j] = str[i];
            j++;
        }
    }

    result[j] = '\0'; 

    return result;
}



int countWords(const char* str) {
	int i;
	int wordCount = 0;
    int inWord = 0;
    if (str == NULL)
        return 0;

    

    for ( i = 0; str[i] != '\0'; i++) {
        if (str[i] == ' ' || str[i] == '\t' || str[i] == '\n') {
            if (inWord) {
                wordCount++;
                inWord = 0;
            }
        } else {
            inWord = 1;
        }
    }

    if (inWord) {
        wordCount++;
    }

    return wordCount;
}


char* getFirstWord(const char* str) {
	size_t wordLength;
	char* firstWord;
	if (str == NULL || *str == '\0') {
        return NULL;
    }

    while (isspace(*str)) {
        str++;
    }

    wordLength = strcspn(str, " \t\n\r");

    firstWord = (char*)malloc((wordLength + 1) * sizeof(char));
    if (firstWord == NULL) {
        printf("Memory allocation failed.\n");
        return NULL;
    }

    strncpy(firstWord, str, wordLength);
    firstWord[wordLength] = '\0';

    return firstWord;
}



void appendStringToFile(const char* filename, const char* str) {
    FILE* file = fopen(filename, "a");
    if (file == NULL) {
        printf("Unable to open the file.\n");
        return;
    }
    
    fprintf(file, "%s\n", str);
    
    fclose(file);
}




void deleteLineStartingWithWord(const char* filename, const char* word) {
    FILE* file = fopen(filename, "r");
	FILE* tempFile = tmpfile();
	char line[MAX_LINE_LENGTH];
    int deleted = 0;
	int c;
    if (file == NULL) {
        printf("Failed to open the file: %s\n", filename);
        return;
    }
    
    
    if (tempFile == NULL) {
        printf("Failed to create a temporary file.\n");
        fclose(file);
        return;
    }

    while (fgets(line, sizeof(line), file) != NULL) {
        if (strncmp(line, word, strlen(word)) == 0) {
            if (!deleted) {
                deleted = 1;
                continue;
            }
        }
        
        fputs(line, tempFile);
    }
    
    fclose(file);
    
    rewind(tempFile);
    
    file = fopen(filename, "w");
    if (file == NULL) {
        printf("Failed to open the file for writing: %s\n", filename);
        fclose(tempFile);
        return;
    }
    
  
    while ((c = fgetc(tempFile)) != EOF) {
        fputc(c, file);
    }
    
    fclose(file);
    fclose(tempFile);
    
}




void copyFile(const char* sourceFilename, const char* destinationFilename) {
    FILE* sourceFile = fopen(sourceFilename, "rb");
	FILE* destinationFile = fopen(destinationFilename, "wb");
	char buffer[MAX_FILE_SIZE];
    size_t bytesRead;
    if (sourceFile == NULL) {
        printf("Failed to open the source file.\n");
        return;
    }
    if (destinationFile == NULL) {
        destinationFile = fopen(destinationFilename, "w+b");
        if (destinationFile == NULL) {
            printf("Failed to create the destination file.\n");
            fclose(sourceFile);
            return;
        }
    }
    while ((bytesRead = fread(buffer, 1, sizeof(buffer), sourceFile)) > 0) {
        fwrite(buffer, 1, bytesRead, destinationFile);
    }

    fclose(sourceFile);
    fclose(destinationFile);
	if(DEBUG >=2)
		printf("File copied successfully.\n");
}



void closeFile(FILE* file) {
    if (file != NULL) {
        fclose(file);
    }
}

void deleteEmptyLines(const char* fileName ) {
    FILE *file = fopen(fileName, "r");
    FILE *tempFile = fopen("temp.txt", "w");
    int i;
	char buffer[MAX_FILE_SIZE];
    if (file == NULL) {
        perror("Error opening the file");
        return;
    }

   
    
    if (tempFile == NULL) {
        perror("Error creating temporary file");
        fclose(file);
        return;
    }

   
    while (fgets(buffer, sizeof(buffer), file)) {
        int emptyLine = 1;
        for ( i = 0; i < strlen(buffer); i++) {
            if (!isspace(buffer[i])) {
                emptyLine = 0;
                break;
            }
        }

        if (!emptyLine) {
            fputs(buffer, tempFile);
        }
    }

    fclose(file);
    fclose(tempFile);

    if (remove(fileName) != 0) {
        perror("Error removing the original file");
        return;
    }

    if (rename("temp.txt", fileName) != 0) {
        perror("Error renaming the temporary file");
        return;
    }
	if(DEBUG >=2)
		printf("Empty lines deleted successfully.\n");
}


void deleteStringFromFile(const char* filename, const char* stringToDelete) {
    FILE* file = fopen(filename, "r");
	FILE* tempFile = fopen("temp.txt", "w");
	char buffer[MAX_FILE_SIZE];
    size_t stringToDeleteLength = strlen(stringToDelete);
    int modified = 0; 
	char* occurrence;
	size_t prefixLength;
    if (file == NULL) {
        printf("Failed to open the file.\n");
        return;
    }

    
    if (tempFile == NULL) {
        printf("Failed to create temporary file.\n");
        fclose(file);
        return;
    }
    while (fgets(buffer, sizeof(buffer), file) != NULL) {
        occurrence = strstr(buffer, stringToDelete);
        while (occurrence != NULL) {
            modified = 1;
            prefixLength = occurrence - buffer;
            fwrite(buffer, 1, prefixLength, tempFile);
            fwrite(occurrence + stringToDeleteLength, 1, strlen(occurrence + stringToDeleteLength), tempFile);
            occurrence = strstr(occurrence + stringToDeleteLength, stringToDelete);
        }
        if (!modified) {
            fputs(buffer, tempFile);
        }
        modified = 0;
    }

    fclose(file);
    fclose(tempFile);

    if (remove(filename) != 0) {
        printf("Failed to remove the original file.\n");
        return;
    }
    if (rename("temp.txt", filename) != 0) {
        printf("Failed to rename the temporary file.\n");
        return;
    }

}

int countCharOccurrences(const char* str, char c) {
    int count = 0;

    while (*str != '\0') {
        if (*str == c)
            count++;

        str++;
    }

    return count;
}



int isWordInFile(const char* filename, const char* word) {
    FILE* file = fopen(filename, "r");
	char* delimiters = " ,\t\n"; 
    char line[MAX_LINE_LENGTH];
    char* token;
    if (file == NULL) {
        printf("Error opening the file %s\n", filename);
        return 0;
    }

    

    while (fgets(line, sizeof(line), file)) {
        token = strtok(line, delimiters);
        while (token != NULL) {
            if (strcmp(token, word) == 0) {
                fclose(file);
                return 1; 
            }
            token = strtok(NULL, delimiters);
        }
    }

    fclose(file);
    return 0; 
}



int getNumberByLabel(const char* filename, const char* label) {
    FILE* file = fopen(filename, "r");
	char* delimiters = " \t\n"; 
    char line[MAX_LINE_LENGTH];
    char* token;
	int number ;
    if (file == NULL) {
        printf("Error opening the file %s\n", filename);
        return -1;
    }

   

    while (fgets(line, sizeof(line), file)) {
        token = strtok(line, delimiters);
        if (token == NULL)
            continue;

        if (strcmp(token, label) == 0) {
            token = strtok(NULL, delimiters);
            if (token != NULL) {
                number = atoi(token);
                fclose(file);
                return number; 
            }
        }
    }

    fclose(file);
    return -1; 
}


int isNumber(const char* str) {
    char* endptr;
    int errno = 0;
    strtod(str, &endptr);
    
    if (errno == 0 && endptr != str && *endptr == '\0') {
        return 1;
    }

    return 0; 
}


char* getFileName(const char *fullFileName) {
    char *fileName;
    char *dot = strrchr(fullFileName, '.');
    
    if (dot) {
        size_t dotPosition = dot - fullFileName;
        
        fileName = (char*)malloc((dotPosition + 1) * sizeof(char));
        if (fileName == NULL) {
            fprintf(stderr, "Memory allocation error\n");
            exit(EXIT_FAILURE);
        }
        
        strncpy(fileName, fullFileName, dotPosition);
        fileName[dotPosition] = '\0'; 
    } else {
        fileName = strdup(fullFileName);
        if (fileName == NULL) {
            fprintf(stderr, "Memory allocation error\n");
            exit(EXIT_FAILURE);
        }
    }

    return fileName;
}


void clearFile(const char *fileName) {
    FILE *file = fopen(fileName, "w");
    
    if (file == NULL) {
        perror("Error opening the file");
        return;
    }
    
    fclose(file);
}


int stringLength(const char* str)
{
    int length = 0;

    while (str[length] != '\0') {
        length++;
    }

    return length;
}

void replaceTabsWithSpaces(const char* filename) {
    FILE* file = fopen(filename, "r");
	FILE* tempFile = fopen("temp.txt", "w");
	char ch;
	
    if (file == NULL) {
        printf("Error opening the file.\n");
        return;
    }

    
    if (tempFile == NULL) {
        printf("Error creating temporary file.\n");
        fclose(file);
        return;
    }

    

    while ((ch = fgetc(file)) != EOF) {
        if (ch == '\t') {
            fputc(' ', tempFile);
        } else {
            fputc(ch, tempFile);
        }
    }

    fclose(file);
    fclose(tempFile);

    remove(filename);
    rename("temp.txt", filename);
}


int getNumberFromWord(const char* filename, const char* word) {
    FILE* file = fopen(filename, "r");
	char buffer[100];
	int number = -10; 
	char* token;
	char* numStr;
    if (file == NULL) {
        printf("Error opening the file.\n");
        return -10;
    }

    
    

    while (fgets(buffer, sizeof(buffer), file) != NULL) {
         token = strtok(buffer, ":");
        if (token != NULL) {
            if (strcmp(token, word) == 0) {
                numStr = strtok(NULL, " ");
                if (numStr != NULL) {
                    number = atoi(numStr);
                    break; 
                }
            }
        }
    }
    fclose(file);
    return number;
}


void update_number_in_file(const char *file_name, const char *word, int new_number) {
	char line[MAX_LINE_LENGTH];
    char updated_lines[1000] = ""; 
    FILE *file = fopen(file_name, "r");
    if (file == NULL) {
        printf("Error: File '%s' not found.\n", file_name);
        return;
    }

    
    while (fgets(line, sizeof(line), file)) {
        if (strncmp(line, word, strlen(word)) == 0 && line[strlen(word)] == ':') {
           
            snprintf(line, sizeof(line), "%s: %d\n", word, new_number);
        }
        strcat(updated_lines, line);
    }

    fclose(file);

    file = fopen(file_name, "w");
    if (file == NULL) {
        printf("Error: Unable to open file for writing.\n");
        return;
    }

    fputs(updated_lines, file);
    fclose(file);
	if(DEBUG > 2)
		printf("Number for %s updated to %d successfully.\n", word, new_number);
}


void mergeFiles(const char* sourceFileName, const char* destinationFileName) {
    FILE *sourceFile, *destinationFile;
    char ch;

    sourceFile = fopen(sourceFileName, "r");
    if (sourceFile == NULL) {
        printf("Error: Unable to open source file '%s' for reading.\n", sourceFileName);
        return;
    }

    destinationFile = fopen(destinationFileName, "a");
    if (destinationFile == NULL) {
        printf("Error: Unable to open destination file '%s' for appending.\n", destinationFileName);
        fclose(sourceFile);
        return;
    }

    while ((ch = fgetc(sourceFile)) != EOF) {
        fputc(ch, destinationFile);
    }

    fclose(sourceFile);
    fclose(destinationFile);
	if (DEBUG > 2)
		printf("Files merged successfully.\n");
}


char* getLabelFromLine(const char* file_name, int line_number) {
    FILE* file = fopen(file_name, "r");
	char line[100];
	int current_line = 0;
	char* colon_pos;
	size_t word_length;
	char* word;
    if (file == NULL) {
        printf("Error: File '%s' not found.\n", file_name);
        return NULL;
    }

    while (fgets(line, sizeof(line), file)) {
        if (current_line == line_number) {
            colon_pos = strchr(line, ':');
            if (colon_pos != NULL) {
                word_length = colon_pos - line;
                word = (char*)malloc(word_length + 1);
                strncpy(word, line, word_length);
                word[word_length] = '\0';
                fclose(file);
                return word;
            }
        }
        current_line++;
    }

    fclose(file);
	if(DEBUG >2)
		printf("Error: Line number %d is out of range.\n", line_number);
    return NULL;
}

char getCharAtIndex(const char *str, int n) {
    if (n < 0 || n >= strlen(str)) {
        return '\0'; 
    }

    return str[n];
}


int getAsciiFromChar(const char ch) {
    return (int)ch;
}


void insertStringAtBeginningOfFile(const char *filename, const char *string) {
	size_t stringLength ;
	size_t fileSize;
	char *buffer;
    FILE *file = fopen(filename, "r+");
    if (file == NULL) {
        perror("Error opening file");
        return;
    }

    stringLength = strlen(string);
    fseek(file, 0, SEEK_END);
    fileSize = ftell(file);

    buffer = (char *)malloc(fileSize + 1 + stringLength);
    if (buffer == NULL) {
        perror("Memory allocation error");
        fclose(file);
        return;
    }

    strcpy(buffer, string);

    fseek(file, 0, SEEK_SET);
    fread(buffer + stringLength, 1, fileSize, file);

    fseek(file, 0, SEEK_SET);
    fwrite(buffer, 1, fileSize + stringLength, file);

    fclose(file);
    free(buffer);
}

char* intToString(int num) {
    int numLength = snprintf(NULL, 0, "%d", num);
    char *str = (char *)malloc(numLength + 1);
    if (str == NULL) {
        perror("Memory allocation error");
        return NULL;
    }

    snprintf(str, numLength + 1, "%d", num);

    return str;
}

void cutLastNChars(char* str, size_t n) {
    size_t length = strlen(str);
    
    if (n >= length) {
        str[0] = '\0';
    } else {
        str[length - n] = '\0';
    }
}

/* Function to convert a binary string to base64 */
char* binary_to_base64(const char* binary_string, int length) {
	char* base64_chars = "ABCDEFGHIJKLMNOPQRSTUVWXYZabcdefghijklmnopqrstuvwxyz0123456789+/";
	char* result = (char*)malloc((length * 4 / 3) + 4);
	int i;
	int j;
	int chunk = 0;
    int base64_index = 0;
    for ( i = 0; i < length; i += 6) {
        chunk = 0;
        for ( j = 0; j < 6; j++) {
            if (i + j < length) {
                chunk <<= 1;
                chunk |= binary_string[i + j] - '0';
            } else {
                chunk <<= 1;
            }
        }

        result[base64_index++] = base64_chars[chunk];
    }
    cutLastNChars(result , 1);
    result[base64_index] = '\0';

    return result;
}

/* Function to read binary lines from a file */
char** read_binary_lines_from_file(const char* filename, int* num_lines) {
    FILE* file = fopen(filename, "r");
	char line[1024];
	char** lines;
    if (!file) {
        perror("Error opening file");
        exit(1);
    }
    lines = (char**)malloc(sizeof(char*));
    *num_lines = 0;

    /* Read each line from the file */
    while (fgets(line, sizeof(line), file)) {
        *num_lines += 1;
        lines = (char**)realloc(lines, (*num_lines) * sizeof(char*));
        lines[*num_lines - 1] = strdup(line);
    }

    fclose(file);
    return lines;
}

/* Function to convert a binary file to base64 and write the output back to the file */
void convert_binary_file_to_base64(const char* input_filename , char * instruction , char *  data) {
    int num_lines;
    char** binary_lines = read_binary_lines_from_file(input_filename, &num_lines);
	int i;
	char* base64_output ;
    /* Open the file for writing */
    FILE* output_file = fopen(input_filename, "w");
    if (!output_file) {
        perror("Error opening output file");
        exit(1);
    }

    /* Convert and write each binary line in base64 */
	
    for ( i = 0; i < num_lines; i++) {
        base64_output = binary_to_base64(binary_lines[i], strlen(binary_lines[i]));
        fprintf(output_file, "%s\n", base64_output);
        free(base64_output);
    }

    /* Clean up memory and close the file */
    free_binary_lines(binary_lines, num_lines);
    fclose(output_file);
	
	insertStringAtBeginningOfFile(input_filename , "\n");
	
	insertStringAtBeginningOfFile(input_filename , instruction);
	insertStringAtBeginningOfFile(input_filename , " ");
	insertStringAtBeginningOfFile(input_filename , data);
}

/* Function to free memory allocated for binary lines */
void free_binary_lines(char** lines, int num_lines) {
	int i;
    for (i = 0; i < num_lines; i++) {
        free(lines[i]);
    }
    free(lines);
}