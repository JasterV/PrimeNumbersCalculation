#include "common.h"

int main(int argc, char *argv[]) {
    int number = atoi(argv[1]);
    for (int i = 2; i <= number; ++i) {
        if(write(PIPE_NOMBRES_WRITE, &i, sizeof(int)) == -1){
            perror("Error writing the generator numbers.");
            exit(-1);
        }
    }
    close(PIPE_NOMBRES_WRITE);
    signal(SIGTERM, exit);
    pause();
}

