#include "common.h"

int main(int argc, char *argv[]) {
    int cp[2], gp[2];
    int numCalculators = atoi(argv[1]);
    int nombresPrimersExit = 0, nombresPrimersPipe = 0;
    char number[MAXNUMSIZE + 1];
    pid_t genpid, contpids[numCalculators];

    /*Creates the pipes*/
    if (pipe(gp) == -1) {
        perror("Creating pipe:");
        exit(-1);
    }
    /* Writing to prompt*/
    if (write(1, "Calcular nombres primers de 2 a: ", 33) == -1) {
        perror("Error writing standard out: ");
        exit(-1);
    }

    /*Reading the number*/
    if (read(0, number, MAXNUMSIZE) == -1) {
        perror("Error reading standard int: ");
        exit(-1);
    }

    /*Print error if the given number is < 2*/
    if (atoi(number) < 2) {
        if (write(1, "Error number < 2\n", 16) == -1) {
            perror("Error writing standard out: ");
        }
        exit(-1);
    }

    /*Creates the generator*/
     if((genpid = fork()) == 0){
            dup2(gp[1], PIPE_NOMBRES_WRITE);
            close(gp[0]);
            close(gp[1]);
            if(execlp("./generador", "generador", number, NULL) == -1){
                perror("Error running the generator.");
                exit(-1);
            };
            exit(0);
        }else if(genpid == -1){
            perror("Error de creación del proceso hijo generador.");
                exit(-1);
        }

    if (pipe(cp) == -1) {
        perror("Creating pipe:");
        exit(-1);
    }
        
    /*Creates the calculators*/
    for (int i = 0; i < numCalculators; ++i) {
        if((contpids[i] = fork()) == 0){
                dup2(gp[0], PIPE_NOMBRES_READ);
                dup2(cp[1], PIPE_RESPOSTES_WRITE);
                close(gp[1]);
                close(gp[0]);
                close(cp[1]);
                close(cp[0]);
                if(execlp("./calculador", "calculador", NULL) == -1){
                    perror("Error running the calculator");
                    exit(-1);
                }
                exit(0);
        }else if(contpids[i] == -1){
            perror("Error de creación del proceso hijo calculador.");
                exit(-1);
        }
    }


    /*Duplicates the calculators pipe entry and closes the remaining descriptors*/
    dup2(cp[0], PIPE_RESPOSTES_READ);
    close(gp[0]);
    close(gp[1]);
    close(cp[0]);
    close(cp[1]);

    /*Treat the results*/
    t_infoNombre infon;
    char buffer[100];
    for(int i = 0; read(PIPE_RESPOSTES_READ, &infon, sizeof(t_infoNombre)) != 0; ++i) {
        sprintf(buffer, "Pid del procés calculador: %5d, Nombre a calcular: %4d, Es primer? %2c\n", infon.pid, infon.nombre, infon.primer);
        write(1, buffer, strlen(buffer));
        if(infon.primer == 'S'){
            nombresPrimersPipe += 1;
        }
        memset(buffer, '\0', sizeof(buffer));
    }
    
    int status = 0;
    /*Send the signals*/
    if(kill(genpid, SIGTERM) == -1){
        perror("Error closing generator: ");
        exit(-1);
    }
    wait(NULL);
    for(int i = 0; i < numCalculators;++i) {
        if (kill(contpids[i], SIGTERM) == -1) {
            perror("Error closing calculator: ");
            exit(-1);
        }
        wait(&status);
        nombresPrimersExit += WEXITSTATUS(status);
    }

    sprintf(buffer, "Nombres primers pipe: %d, Nombres primers exit: %d\n", nombresPrimersPipe, nombresPrimersExit);
    write(1, buffer, strlen(buffer));

    exit(0);
}