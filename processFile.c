#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <unistd.h>

#include "processFile.h"

// structure used to define the parameters when creeating threads
struct Parameters
{
    accounts *accounts;
    char *transactions;
};

// functions used to count the number of clients and accounts in the input file and set the same data members in the accounts and clients parameters
void countClientsAndAccounts(FILE *fp, accounts *accounts, clients *clients)
{
	int numAccounts = 0;
	int numClients = 0;
	char c;

	// loop through the entire file
	for (c = getc(fp); c != EOF; c = getc(fp))
	{
		// Only account lines have the b characer, so the number of accounts is equal to the number of the character 'b'
		if (c == 'b') 
			numAccounts+=1;
		// Only the client lines have the 'c' character, so the number of clients is equal to the number of the character 'c'
		if (c == 'c') 
			numClients+=1;
	}

	// Set the number of clients and accounts into the parameters
	accounts->nAccounts = numAccounts;
	clients->nClients = numClients;

	// reset the file pointer
	fseek(fp, 0, SEEK_SET);
}

// function that reads through the file and processes all the transactions
int processFile(char *filename, accounts *accounts, clients *clients)
{
	FILE *fp = fopen(filename, "r");
	if (fp == NULL)
	{
		perror(filename);
		exit(1);
	}
	// get the number of clients and accounts
	countClientsAndAccounts(fp, accounts, clients); 

	// allocate memory for the balances and client transactions
	int *balances = malloc(sizeof(int) * accounts->nAccounts);
	clients->clientTransactions = malloc(sizeof(char *) * clients->nClients); 

	int accountNum;
	int clientNum = 0;

	const char delim[2] = " ";
	char *buffer = NULL;
	size_t len = 0;
	size_t line;
	char *tok;


	// go through the entire file to set account balances and client transactions
	while ((line = getline(&buffer, &len, fp)) != -1)
	{
		// if the line is a client transaction line, then allocate memory for the transactions and push them to the data member
		if (buffer[0] == 'c')
		{

			clients->clientTransactions[clientNum] = (char*)malloc(line + 1);
			strncpy(clients->clientTransactions[clientNum], buffer, line);

			clientNum++;
		}

		tok = strtok(buffer, delim);
		while (tok != NULL)
		{
			if (tok[0] == 'a') 
			{
				// set the account number to the second character of the token converted into an int
				accountNum = tok[1] - '0';

				// get next token
				tok = strtok(NULL, delim);

				// if the token is 'b' then get the next token and set that to be the balance
				if (tok[0] == 'b')
				{ 
					tok = strtok(NULL, delim);
					balances[accountNum - 1] = atoi(tok);
					
				}
				else
				{
					tok = strtok(NULL, delim);
				}
			}
			
			tok = strtok(NULL, delim);
		}
	}
	
	accounts->balance = balances;
	accounts->locks = malloc(sizeof(pthread_mutex_t) * accounts->nAccounts);

	// set mutex locks for the accounts
	for (int i = 0; i < accounts->nAccounts; i++)
	{
		pthread_mutex_init(&(accounts->locks[i]), NULL);
	
	}

	fclose(fp);
	free(buffer);

	// set up variables for threads
	pthread_t threads[clients->nClients];
	struct Parameters parameters[clients->nClients];

	// Create a thread for each client
	for (int i = 0; i < clients->nClients; i++)
	{
		parameters[i].accounts = accounts;
		parameters[i].transactions = clients->clientTransactions[i];

		// Create each thread using the parameters
		pthread_create(&threads[i], NULL, doTransactions, &parameters[i]);

	}

	// Join threads
	for (int i = 0; i < clients->nClients; i++)
		pthread_join(threads[i], NULL);
}

// function that frees the data members for accounts and clients 
void freeMemory(accounts *accounts, clients *clients)
{
	// free balance data member
	free(accounts->balance);

	// free all locks 
	for (int i = 0; i < accounts->nAccounts; i++)
	{
		pthread_mutex_destroy(&(accounts->locks[i]));
	}
	free(accounts->locks);

	// free client transactions data member
	free(clients->clientTransactions);
}

// function that processes the transactions in the threads
void *doTransactions(void *parameters)
{
    struct Parameters *params = parameters;
    accounts *accounts = params->accounts;
    char *transactions = params->transactions;

    char *tok;
    char *buffer = transactions;

    const char delim[2] = " ";

    int account1;
    int account2;
    int amount;

    // parse through all the client transactions
    tok = strtok_r(transactions, delim, &buffer);
    while (tok != NULL)
    {
		// if the transaction is w (withdrawal)
		if (tok[0] == 'w')
		{
			// then get the account number
			tok = strtok_r(NULL, delim, &buffer);
			account1 = tok[1] - '0';

			// get the amount
			tok = strtok_r(NULL, delim, &buffer);
			amount = atoi(tok);

			// perform the withdrawal
			withdraw(account1 - 1, amount, accounts);
		}

		// if the transaction is a deposit
        else if (tok[0] == 'd')
        {
			// then get the account number
            tok = strtok_r(NULL, delim, &buffer);
            account1 = tok[1] - '0';

			// get the amount
            tok = strtok_r(NULL, delim, &buffer);
            amount = atoi(tok);

			// perform the deposit
            deposit(account1 - 1, amount, accounts);
        }
       
		// if the transaction is a transfer
        else if (tok[0] == 't')
        {
			// then get the first account
            tok = strtok_r(NULL, delim, &buffer);
            account1 = tok[1] - '0';

			// get the second account
            tok = strtok_r(NULL, delim, &buffer);
            account2 = tok[1] - '0';

			// get the amount 
            tok = strtok_r(NULL, delim, &buffer);
            amount = atoi(tok);

			// do the transfer
            transfer(account1 - 1, account2 - 1, amount, accounts);
        }

		// if none of the above then go to the next token
        tok = strtok_r(NULL, delim, &buffer);
    }
}

// function that does the withdrawal transaction with the given account number and amount
void withdraw(int account1, int amount, accounts *accounts)
{
	// only occurs if the amount requested is less than the balance of the account
    if (amount <= accounts->balance[account1])
    {
		// lock account 
		pthread_mutex_lock(&(accounts->locks[account1]));

		// critical region. Do the withdrawal transaction
        accounts->balance[account1] -= amount;

		// unlock account
		pthread_mutex_unlock(&(accounts->locks[account1]));
    }
    
	
}

// function that does the deposit transaction with the given account number and amount
void deposit(int account1, int amount, accounts *accounts)
{
    // lock account
    pthread_mutex_lock(&(accounts->locks[account1])); 

	// critical section. Do the deposit transaction
    accounts->balance[account1] += amount;

    //unlock account
    pthread_mutex_unlock(&(accounts->locks[account1]));
}

// function that does the transfer transaction with the given account numbers and amount
void transfer(int account1, int account2, int amount, accounts *accounts)
{
	// only happens when the amount requested is less than the balance in account 1
    if (amount <= accounts->balance[account1])
    {
		// lock the two accounts associated with the transfer
		pthread_mutex_lock(&(accounts->locks[account1]));
		pthread_mutex_lock(&(accounts->locks[account2]));

		// critical region. Do the transfer transaction
        accounts->balance[account1] -= amount;
        accounts->balance[account2] += amount;

		// unlock the two accounts associated with the transfer
		pthread_mutex_unlock(&(accounts->locks[account1]));
		pthread_mutex_unlock(&(accounts->locks[account2]));
    }
}