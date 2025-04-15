#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
#include <string.h>
#include <stdbool.h>

#include <pthread.h>
#include <semaphore.h>

sem_t semaforo1;
sem_t semaforo2;

int main(int argc, char *argv[])
{
    
    sem_init(&semaforo1, 0, 0);

    return 0;
}


