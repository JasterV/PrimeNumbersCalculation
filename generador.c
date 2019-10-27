/*-------------------------------------------------------------------------
PRA1: Pralab2
Codi font: generador.c

Víctor Martínez Montané
--------------------------------------------------------------------------*/

#include "common.h"

/*Writes numbers from 2 to n into the numbers pipe*/
int main(int argc, char *argv[]) 
{
    int number = atoi(argv[1]);
    /*Generate and write the numbers*/
    for (int i = 2; i <= number; ++i) 
    {
        if(write(PIPE_NUMBERS_WRITE, &i, sizeof(int)) == -1)
        {
            perror("Error writing the generator numbers.");
            exit(EXIT_FAILURE);
        }
    }
    close(PIPE_NUMBERS_WRITE);
    /*Wait for SIGTERM signal and exit*/
    if(signal(SIGTERM, SIG_DFL) == SIG_ERR)
    {
        perror("Signal: ");
        exit(EXIT_FAILURE);
    }
    pause();
}

