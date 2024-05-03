#include <stdio.h>
#include <string.h>

int my_strcmp(const char *str1, const char *str2);
int my_strncmp(const char *s1, const char *s2, size_t n);
int my_strchr (const char *str , int c);

#define MAX_L 80
int main() {

    char str1[MAX_L];
    char str2[MAX_L];
    char chr;
    char degel = 'y';
    int select;
    int result;
    int result1;
    int result2;
    int x;
    do{
		printf ("Enter the number of the function you want to use: \n");	
		printf("1: my_strcmp\n");
		printf("2: my_strncmp\n");
		printf("3: my_strchr\n");
		printf("\n");
		scanf("\n%d",&select);
		printf("\n");
		
		if(select == 1){
			printf("\nEnter the first string: \n"); 
			scanf("%s",str1);
			printf("\nEnter the second string: \n");
			scanf("%s",str2);
			result = my_strcmp(str1, str2);
			printf("\nmy_strcmp result: %d\n" , result);
			printf("\n");

			if (result > 0) {
				printf("%s comes after %s in lexicographic order.\n", str1, str2);
			} else if (result < 0) {
				printf("%s comes before %s in lexicographic order.\n", str1, str2);
			} else {
				printf("%s and %s are identical.\n", str1, str2);
				}
			}

		
		if(select == 2){
			printf("Enter the first string: \n"); 
			scanf("%s",str1);
			printf("\nEnter the second string: \n");
			scanf("%s",str2);
			printf("\nEnter the number of characters for comparison: \n");
			scanf("%d",&x);
			result1 = my_strncmp(str1, str2, x);
			printf("\nmy_strcmp result: %d\n" , result1);
			printf("\n");
			if (result1 < 0) {
			printf("The first 7 characters of str1 are less than the first 7 characters of str2.\n");
			}
			else if (result1 > 0) {
			printf("The first 7 characters of str1 are greater than the first 7 characters of str2.\n");
			}
			else {
			printf("The first 7 characters of str1 and str2 are equal. \n");
			}
			}

		if(select == 3){		   
			printf("Enter string:\n");
			scanf("%s" , str1);
			printf("\n\nEnter char:\n");
			scanf(" %c" , &chr);
			result2 =  my_strchr(str1 , chr);
			printf("\nmy_strchr: %d\n" , result2); 
			}
		printf("\nDo you want to start over ? [y/n]:\n");
		scanf(" %c" , &degel);

	}while (degel == 'y'); /* A while loop that repeats the entire program */

	printf("\n bye-bye \n");
	return 0;
    
}


/*
This function gets 2 string 'str1' and 'str2'.
A function that compares two strings and returns :
0 - if they are equal.
Positive sum – if the first string is greater than the second.
Negative sum – if the first string is less than the second. 
*/
int my_strcmp(const char* str1, const char* str2) {
    int i = 0;
    while (str1[i] != '\0' && str2[i] != '\0') {
        if (str1[i] != str2[i]) {
            return str1[i] - str2[i];
        }
        i++;
    }
    return str1[i] - str2[i];
}


/*
This function gets 2 string (str1 and str2) and number, compers between n chars of each str and return:
0 - if they are equal.
Positive sum – if the first string is greater than the second.
Negative sum – if the first string is less than the second. 
*/
int my_strncmp(const char *str1, const char *str2, size_t n)
{
	int i = 0;
    for (i = 0; i < n && str1[i] != '\0' && str2[i] != '\0'; i++) 
	{
        if (str1[i] != str2[i]) {
            return str1[i] - str2[i];
        }
    }
    if (n > 0) {
        return str1[n-1] - str2[n-1];
    }
    return 0;
}

/*
This function gets string 'str' and char 'ch' and return:
returns -1 if the char does not appear in stirng  or the index of the first show of the char in the string.

*/
int my_strchr (const char *str , int c)
{
	int i = 0;
	while(str[i] != '\0')
	{
		if(str[i] == c)
			return i ;
		i++;
	}
	return -1;
}
