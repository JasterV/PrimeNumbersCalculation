/*-------------------------------------------------------------------------
PRA1: Pralab2
Codi font: controlador.c

Víctor Martínez Montané
--------------------------------------------------------------------------*/

#include "common.h"

/*Creates one generator thread and multiple calculator threads.
    Reads the reponses pipe and print the results*/
int main(int argc, char *argv[]) 
{
    int cp[2], gp[2]; //cp: Responses pipe, gp: Numbers pipe
    int num_calculators = atoi(argv[1]); //Num of calculators to create
    int prime_numbers_exit = 0, prime_numbers_pipe = 0;
    char number[MAXNUMSIZE + 1];
    pid_t genpid, contpids[num_calculators];

    /* Writing to prompt*/
    if (write(1, "Calculate prime numbers from 2 to: ", 36) == -1) 
    {
        perror("Error writing standard out: ");
        exit(EXIT_FAILURE);
    }

    /*Reading the number*/
    if (read(0, number, MAXNUMSIZE) == -1) 
    {
        perror("Error reading standard int: ");
        exit(EXIT_FAILURE);
    }

    /*Print error if the given number is < 2*/
    if (atoi(number) < 2) 
    {
        if (write(1, "Error number < 2\n", 16) == -1) 
            perror("Error writing standard out: ");
        exit(EXIT_FAILURE);
    }

    /*Creates the pipes*/
    if (pipe(gp) == -1) 
    {
        perror("Error creating generator pipe:");
        exit(EXIT_FAILURE);
    }

    /*Creates the generator*/
     if((genpid = fork()) == 0)
     {
            dup2(gp[1], PIPE_NUMBERS_WRITE);
            close(gp[0]);
            close(gp[1]);
            execlp("./generador", "generador", number, NULL);
            perror("Error running the generator.");
            exit(EXIT_FAILURE);
        }
        else if(genpid == -1)
        {
            perror("Fork error on generator.");
            exit(EXIT_FAILURE);
        }

    /*Creates the responses pipe*/
    if (pipe(cp) == -1) 
    {
        perror("Creating numbers pipe:");
        exit(EXIT_FAILURE);
    }

    /*Creates the calculators*/
    for (int i = 0; i < num_calculators; ++i) 
    {
        if((contpids[i] = fork()) == 0)
        {
                dup2(gp[0], PIPE_NUMBERS_READ);
                dup2(cp[1], PIPE_RESPONSES_WRITE);
                close(gp[1]);
                close(gp[0]);
                close(cp[1]);
                close(cp[0]);
                execlp("./calculador", "calculador", NULL);
                perror("Error running the calculator");
                exit(EXIT_FAILURE);
        }
        else if(contpids[i] == -1)
        {
            perror("Fork error on calculator.");
            exit(EXIT_FAILURE);
        }
    }

    /*Duplicates the numbers pipe entry and closes the remaining descriptors*/
    dup2(cp[0], PIPE_RESPONSES_READ);
    close(gp[0]);
    close(gp[1]);
    close(cp[0]);
    close(cp[1]);

    /*Treat the results*/
    t_infoNumber infon;
    char buffer[100];
    while(read(PIPE_RESPONSES_READ, &infon, sizeof(t_infoNumber)) > 0) 
    {
        sprintf(buffer, "Calculator PID: %5d, \t Number: %4d, Is prime? %2c\n", infon.pid, infon.number, infon.prime);
        if(write(1, buffer, strlen(buffer)) == -1)
        {
            perror("Error writing standart out: ");
            exit(EXIT_FAILURE);
        }
        if(infon.prime == 'Y') ++prime_numbers_pipe;
    }

    int status = 0;
    /*Send the signals*/
    if(kill(genpid, SIGTERM) == -1)
    {
        perror("Error closing generator: ");
        exit(EXIT_FAILURE);
    }
    wait(NULL);

    for(int i = 0; i < num_calculators; ++i) 
    {
        if (kill(contpids[i], SIGTERM) == -1) 
        {
            perror("Error closing calculator: ");
            exit(EXIT_FAILURE);
        }
        wait(&status);
        if(WIFEXITED(status))
        {
            prime_numbers_exit += WEXITSTATUS(status);
        }
    }

    sprintf(buffer, "\nPrime numbers pipe: %d, \t Prime numbers exit: %d\n", prime_numbers_pipe, prime_numbers_exit);
    if(write(1, buffer, strlen(buffer)) == -1)
    {
        perror("Error writing standart out: ");
        exit(EXIT_FAILURE);
    }

    exit(EXIT_SUCCESS);
}