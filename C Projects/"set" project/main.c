#include "stdio.h"
#include "set.h"
#include "string.h"
#include "stdlib.h"

int main()
{
	char flag = 'y';
	do	
	{
	int size_of_container;
	int* array = (int*)malloc(SIZE * sizeof(int));
	printf("\n\nEnter the numbers you want \n\n");
	size_of_container = get_set(array , SIZE);
	
	printf("\nThe printed array is:\n\n");
	print_set(array , size_of_container);
	printf ("\n\n");
	free(array);
	
	printf("\nDo you want to start over? (y/n):\n");
	scanf(" %c" , &flag);
	
	}

	while (flag == 'y');
	printf("\n bye-bye \n");
	return 1;
}


