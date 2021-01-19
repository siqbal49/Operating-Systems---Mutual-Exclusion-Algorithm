#ifndef TYPES_H
#define TYPES_H

#include <pthread.h>

typedef struct accounts
{
	// number of accounts
	int nAccounts;

	// balance for each account
	int *balance;		

	// mutex locks for each account
	pthread_mutex_t *locks; 
} accounts;

typedef struct clients
{
	// number of clients
	int nClients;		

	// list of client transactions  
	char **clientTransactions; 
} clients;

#endif