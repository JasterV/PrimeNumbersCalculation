/*-------------------------------------------------------------------------
PRA1: Pralab2
Codi font: calculador.c

Víctor Martínez Montané
--------------------------------------------------------------------------*/

#include "common.h"

int isprime(long);
void sighandler(int);
int numprimes = 0;

/*Reads numbers from the numbers pipe, 
    calculates if those numbers are primes 
    and send the result to the response pipe*/
int main(int argc, char *argv[]) 
{
    int number;
    t_infoNumber results;
    results.pid = getpid();
    /*Reads every number, calculates and write the results*/
    while(read(PIPE_NUMBERS_READ, &number, sizeof(int)) > 0) 
    {
        results.number = number;
        if(isprime(number))
        {
            ++numprimes;
            results.prime = 'Y';
        }
        else results.prime = 'N';

        if(write(PIPE_RESPONSES_WRITE, &results, sizeof(t_infoNumber)) == -1)
        {
            perror("Error writing into the pipe: ");
            exit(EXIT_FAILURE);
        }
    }
    /*Close the pipes and wait for SIGTERM signal*/
    close(PIPE_RESPONSES_WRITE);
    close(PIPE_NUMBERS_READ);
    if(signal(SIGTERM, sighandler) == SIG_ERR)
    {
        perror("Signal: ");
        exit(EXIT_FAILURE);
    }
    pause();
}

/*Handles the signal*/
void sighandler(int signum)
{
    exit(numprimes);
}

/*Calculates if n is prime*/
int isprime(long n)
{
    for(int i = 2; i < n; ++i) 
    {
        if(n%i == 0) return false;
    }
    return true;
}