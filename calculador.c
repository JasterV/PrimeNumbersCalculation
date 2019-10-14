#include "common.h"

int isprime(long);
void sighandler(int);
int numprimes = 0;

int main(int argc, char *argv[]) {
    int number;
    t_infoNombre results;
    results.pid = getpid();
    
    /*Reads every number, calculates and write the results*/
    while(read(PIPE_NOMBRES_READ, &number, sizeof(int)) != 0) {
        results.nombre = number;
        if(isprime(number)){
            numprimes += 1;
            results.primer = 'S';
        }else{
            results.primer = 'N';
        }
        if(write(PIPE_RESPOSTES_WRITE, &results, sizeof(t_infoNombre)) == -1){
            perror("Error writing into the pipe: ");
            exit(-1);
        }
    }
    close(PIPE_RESPOSTES_WRITE);
    close(PIPE_NOMBRES_READ);
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