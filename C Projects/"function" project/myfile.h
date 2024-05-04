#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <ctype.h>
#define DEBUG 1
#define MAX_LINE_LENGTH 80
#define MAX_FILE_NAME 256
#define MAX_FILE_SIZE 4096

#define LABEL_FILE "label.lb"

/**
 * Replaces occurrences of a word in a file with a specified string.
 *
 * @param filename    The name of the input file.
 * @param word        The word to be replaced.
 * @param string      The string to replace the word with.
 */
void replaceWordInFile(const char* filename, const char* word, const char* string) ;



/**
 * Cuts the string between specified start and stop words in a file.
 *
 * @param filename    The name of the input file.
 * @param outFilename The name of the output file.
 * @param startWord   The word marking the start of the string to be cut.
 * @param stopWord    The word marking the end of the string to be cut.
 * @return            The cut string if successful, or NULL if an error occurs.
 */
char* cutStringBetweenWords(const char* filename, const char* outFilename,  const char* startWord, const char* stopWord ) ;



/**
 * Extracts the first word (macro name) from a given input string.
 *
 * @param input       The input string.
 * @return            The extracted macro name if successful, or NULL if the input is empty or memory allocation fails.
 */
char* extract_mcro_name(const char* input) ;



/**
 * Extracts the content (text after the macro name) from a given input string.
 *
 * @param input       The input string.
 * @return            The extracted macro content if successful, or NULL if the input is empty or memory allocation fails.
 */
char* extract_macro_content(const char* input) ;



/**
 * Beautifies an assembly file by removing redundant whitespace.
 *
 * @param filename    The name of the input assembly file.
 */
void beautifyAssembly(const char* filename) ;



/**
 * Adds a tab character at the end of a file.
 *
 * @param file    The file to which the tab character should be added.
 */
void addTabToEndOfFile(FILE *file) ;



/**
 * Opens a file in read mode.
 *
 * @param filename    The name of the file to be opened.
 * @return            A pointer to the opened file if successful, or NULL if the file opening fails.
 */
FILE* openFile(const char* filename);


/**
 * Deletes lines from a file that do not start with a semicolon (';') or whitespace.
 *
 * @param filename    The name of the input file.
 */
void deleteLinesWithNonSemicolonFirstChar(const char* filename) ;




/**
 * Checks if a file is excited by attempting to open it.
 *
 * @param filename  The name of the file to check.
 * @return          Returns 1 if the file is excited (successfully opened), or 0 otherwise.
 */
int isExcited(const char* filename) ;



/**
 * Retrieves the substring of a given string before the specified character.
 *
 * @param str  The input string.
 * @param ch   The character to search for.
 * @return     Returns the substring before the specified character, or NULL if the character is not found.
 */
char* getStringBeforeChar(const char* str, char ch);



/**
 * Extracts labels from a file and prints them along with their line numbers.
 *
 * @param filename      The name of the input file.
 * @param outFilename   The name of the output file to write the labels and line numbers.
 */
void printFileLabels(const char* filename, const char* outFilename);


int countCharOccurrences(const char* str, char c) ;


void deleteEmptyLines(const char* filename ) ;

void appendStringToFile(const char* filename, const char* str) ;

int countWords(const char* str) ;

void deleteLineStartingWithWord(const char* filename, const char* word) ;

char* findWordInFile(const char* filename, const char* word);

void copyFile(const char* sourceFilename, const char* destinationFilename) ;

char* concatenateStrings( char* str1,  char* str2) ;

int findStringInFile(const char* filename, const char* searchString) ;

void deleteStringFromFile(const char* filename, const char* stringToDelete) ;

char* removeWordFromString(const char* line, const char* word) ;

char* strdup(const char* str) ;

int isNumber(const char* str) ;

int isWordInFile(const char* filename, const char* word) ;

char* getFileName(const char *fullFileName) ;

void mergeFiles(const char* sourceFileName, const char* destinationFileName) ;

void update_number_in_file(const char *file_name, const char *word, int new_number) ;

char* getLabelFromLine(const char* file_name, int line_number) ;

char getCharAtIndex(const char *str, int n) ;

int getAsciiFromChar(const char ch) ;

void insertStringAtBeginningOfFile(const char *filename, const char *string) ;

char* intToString(int num) ;

int getNumberFromWord(const char* filename, const char* word) ;

void replaceTabsWithSpaces(const char* filename) ;

int stringLength(const char* str);

void update_number_in_file(const char *file_name, const char *word, int new_number) ;

void cutLastNChars(char* str, size_t n) ;

void clearFile(const char *fileName) ;

char* binary_to_base64(const char* binary_string, int length) ;

char** read_binary_lines_from_file(const char* filename, int* num_lines) ;

void convert_binary_file_to_base64(const char* input_filename , char * instruction , char *  data) ;

void free_binary_lines(char** lines, int num_lines) ;

int snprintf(char* str, size_t size, const char* format, ...) ;
