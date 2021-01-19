#ifndef IO_H
#define IO_H

#include <stdio.h>
#include "types.h"

// see the .c file for detailed explanation of the following methods

void countClientsAndAccounts(FILE* fp, accounts *accounts, clients *clients);
int processFile(char *filename, accounts *accounts, clients *clients);
void freeMemory(accounts *accounts, clients *clients);
void *doTransactions(void *parameters);
void withdraw(int account1, int amount, accounts *accounts);
void deposit(int account1, int amount, accounts *accounts);
void transfer(int account1, int account2, int amount, accounts *accounts);
#endif