#include "stdio.h"
#include "set.h"
#include "string.h"
#include "stdlib.h"


/* 
A function that receives input from the user and builds a sorted array
*/

int get_set(int * pointerSet , int inSize) 
{
	char ch; int temp = 1; int flag = 0; int count = 0; int tempValue = 0;
	int val;
	while(EOF != (ch = getchar()))
		{
		if ( ch == ' ' || ch == '\n' || ch == '\t') /* Checks whether the user typed 'space' or 'line drop' or 'tab' */
		{
			val = tempValue * temp;
			if (flag && !inside_the_array (pointerSet , count , val ))
				{
					*(pointerSet+count)  = val;
					count ++ ;
				}
				tempValue = 0;
				flag = 0;
				temp = 1;
		}
				
		else
		{
			if ( ch == '-') /*Checks if the received number is negative*/
				temp = -1;
			else 
			{
				tempValue = tempValue * 10 + atoi(&ch);
				flag = 1;
			}
		}
			
		if (count == inSize)
		{
			pointerSet = (int*)realloc(pointerSet, (ENLARGE_SIZE + count) * sizeof(int)); /*Adds the option to absorb more numbers in an array*/
			if (pointerSet == NULL)
			{
				printf("You get ERROR \n");
				return 0;
			}
			
			inSize = ENLARGE_SIZE + count;
		}
				
	}
	val = tempValue * temp;	
	if (flag && ! inside_the_array (pointerSet , count ,  val))
		{
			*(pointerSet+count)  = val;
			count ++ ;
		}
					
	return count;
}
/*
A function that checks whether a number received from the user is inside the array
*/
int inside_the_array(int* count , int inSize , int val)
{
	int i;
	for ( i = 0 ; i < inSize ; i++)
		if (val == *(count + i) ) 
			return 1;
	return 0 ;
}

/*
A function that prints the array numbers. If the number appears several times, the function does not print the same number twice, but only once.
*/
void print_set(int * pointerSet , int containerSize )
{
	int i;
	for ( i = 0 ; i < containerSize ; i++)
		printf("%d  " , *(pointerSet + i ) );
}
