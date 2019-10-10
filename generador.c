#include <stdio.h>
#include <stdlib.h>
#include <sys/wait.h>
#include <unistd.h>
#include <string.h>

void sighandler(int);

int main(int argc, char *argv[]) {
    int number = atoi(argv[1]);
    for (int i = 2; i <= number; ++i) {
        write(1, &i, sizeof(int));
    }

    close(1);
    signal(SIGTERM, sighandler);
    pause();
}

void sighandler(int signum){
    exit(0);
}