#include <stdio.h>
#include <stdlib.h>
#include <sys/types.h>
#include <sys/wait.h>
#include <unistd.h>
#include <string.h>
#include "infonombre.h"
#define MAXNUMSIZE 10

int isprime(long);
void sighandler(int);
int numprimes = 0;

int main(int argc, char *argv[]) {
    int number;
    t_infoNombre results;
    results.pid = getpid();
    
    /*Reads every number, calculates and write the results*/
    while(read(0, &number, sizeof(int)) != 0) {
        results.nombre = number;
        if(isprime(number)){
            numprimes += 1;
            results.primer = 'S';
        }else{
            results.primer = 'N';
        }
        if(write(1, &results, sizeof(t_infoNombre)) == -1){
            perror("Error writing into the pipe: ");
            exit(-1);
        }
    }
    close(1);
    close(0);
    signal(SIGTERM, sighandler);
    pause();
}

void sighandler(int signum){
    exit(numprimes);
}

int isprime(long n){
    int isprime = 1;
    for(int i = 2; i < n; ++i) {
        if(n%i == 0){
            isprime = 0;
        }
    }
    return isprime;
}