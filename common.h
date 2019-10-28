#include <stdio.h>
#include <stdlib.h>
#include <sys/types.h>
#include <sys/wait.h>
#include <unistd.h>
#include <string.h>

#define PIPE_NUMBERS_READ 10
#define PIPE_NUMBERS_WRITE 11
#define PIPE_RESPONSES_READ 20
#define PIPE_RESPONSES_WRITE 21

#define MAXNUMSIZE 10

enum boolean {false, true};

typedef struct{
    int pid;
    int number;
    char prime;
} t_infoNumber;