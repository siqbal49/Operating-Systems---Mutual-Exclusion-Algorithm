#define FILENAME_OUTPUT "assignment_6_output_file.txt"

#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
#include "processFile.h"
#include "types.h"

int main(int argc, char *argv[])
{
	// if incorrect number of arguements, then throw message
	if (argc != 2)
	{
		printf("Usage: %s input.txt\n", argv[0]);
		exit(1);
	}

	// setup accounts and clients objects to pass into the process file  function 
	accounts accounts;
	clients clients;

	// process the file and store all information into accounts and clients
	processFile(argv[1], &accounts, &clients);


	// Remove the file if it already exists
	if (access(FILENAME_OUTPUT, F_OK) != -1)
	{
		remove(FILENAME_OUTPUT);
	}
	
	// open the file for writing
	FILE *fp = fopen(FILENAME_OUTPUT, "w");
	if (fp == NULL)
	{
		printf("Error opening file\n");
		exit(1);
	}

	// write the account balances into the output file and also print it to the screen
	for (int i = 0; i < accounts.nAccounts; i++)
	{
		fprintf(fp, "a%d b %d\n", i+1, accounts.balance[i]); 
		printf("a%d b %d\n", i+1, accounts.balance[i]);	  
	}

	fclose(fp);
	freeMemory(&accounts, &clients);
	return 0;
}