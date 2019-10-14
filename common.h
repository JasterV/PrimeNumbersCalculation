#include <stdio.h>
#include <stdlib.h>
#include <sys/types.h>
#include <sys/wait.h>
#include <unistd.h>
#include <string.h>

#define PIPE_NOMBRES_READ 10
#define PIPE_NOMBRES_WRITE 11
#define PIPE_RESPOSTES_READ 20
#define PIPE_RESPOSTES_WRITE 21

#define MAXNUMSIZE 10

typedef struct{
    int pid;
    int nombre;
    char primer;
} t_infoNombre;