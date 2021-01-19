Using C programming language, you will be developing a mutual exclusion algorithm for a process synchronization problem. 
You need to make sure that your mutual exclusion algorithm ensures that only one process can access the critical section portion of your code at a given point in time. 

You are allowed to use any mutual exclusion / semaphore related C systems calls.

a) Description of the problem is given below:
Assume that there are a set of n bank accounts (n ≥ 1) shared by a set of x clients (x ≥1). 
Clients can perform three different types of transactions with each bank account:deposit, withdraw or transfer funds. 
If a particular transaction results in a negative account balance, the transaction should be ignored (i.e. an account balance should never be less than 0).

b) Structure of the input file:
In the following example, there are two bank accounts (a1 and a2) shared by a total of ten clients (c1 to c10). 
The clients are allowed to deposit money into both the accounts, withdraw money from both the accounts, and transfer money between the two accounts.
The initial balances of the accounts are specified in the input file. 

An input file is provided below for illustrative purposes.

a1 b 5000
a2 b 3500
c1 d a1 100 w a2 500 t a1 a2 25
c2 w a1 2500 t a1 a2 150
...
...
c9 w a1 1000 w a2 500
c10 d a1 50 d a2 200

Illustration:
(i) a1 b 5000
The above line specifies the initial balance of account #1 as $5000

(ii) c1 d a1 100 w a2 500 t a1 a2 25
The above line specifies the operations performed by client #1. client #1 deposits $100 into Account #1, then withdraws $500 from Account #2, and then transfers $25 from Account#1 to Account #2

The input file name will be provided to the program as a command line argument. 
You must output the balances of each bank account after all the transactions have been performed. 
For each bank account, your output should display the account followed by the account balance. 

For example:
a1 b 500
a2 b 300

Your C program should output results to the screen and into a text file
“output_file.txt”.

c) Makefile:
You are also responsible for creating and submitting a Makefile. 
- When running make it should compile your program and build output to a6
- To run your program after compiling: ./a6 [Input filename]
- make clean should remove the output file generated and the compiled executable
