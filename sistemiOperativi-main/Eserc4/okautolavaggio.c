#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
#include <sys/stat.h> // necessario per stat
#include <string.h>
#include <stdbool.h>
#include <pthread.h>
#include <semaphore.h>
#include <fcntl.h>

/*
    AUTONOLLEGGIO
    gestione da parte di più operatori che lavorano in maniera concorrente
*/

// Definizione delle costanti per i dati
#define MAXCMD 100
#define MAXLN 256
#define MAXCAR 256

// comandi che da a disposizione il programma
char comandiDisponibili[4][MAXCMD] = {
    "view",
    "lock",
    "release",
    "quit"
};

// funzione che indirizza il comando
int indirizzatore(char *command) {
    for (int i = 0; i < 4; i++) {
        if (strcmp(command, comandiDisponibili[i]) == 0) {
            return i + 1;
        }
    }
    return 5; // default
}

int whereIs(char vettura[100], char elenco[MAXCAR][MAXLN], int dim) {
    for (int i = 0; i < dim; i++)
    {
        if (strcmp(vettura, elenco[i]) == 0)
        {
            return i;
        }
    }
    return -1;
}

int main()
{
    // lettura da file e inserimento dei dati in un array di stringhe
    FILE *file = fopen("catalog.txt", "r");

    // gestione errore in caso di fail
    if (file == NULL)
    {
        printf("Errore, non è presente il file elencato...\n");
        return 1;
    }

    // gestione semafori e file
    char buffer[MAXLN]; // buffer
    char nomi_semafori[MAXCAR][MAXLN];
    sem_t *semafori[MAXCAR];
    int indice = 0;

    while (fgets(buffer, MAXLN, file) != NULL)
    {
        buffer[strlen(buffer) - 1] = '\0'; // Rimuove il newline

        sem_t *sem = sem_open(buffer, O_CREAT | O_EXCL, S_IRUSR | S_IWUSR, 1);
        if (sem == SEM_FAILED) {
            // il semaforo esiste già, rimediamo
            sem = sem_open(buffer, 0);
            if (sem == SEM_FAILED) {
                printf("attenzione");
                continue;
            }
        }
        semafori[indice] = sem;
        strcpy(nomi_semafori[indice], buffer);
        indice++;
    }
    fclose(file);

    // ciclo infinito, sezione utente
    while (true)
    {
        char comando[MAXLN];
        char argomento1[MAXLN];
        char argomento2[MAXLN];

        printf("Command: ");
        fgets(comando, sizeof(comando), stdin);
        comando[strlen(comando) - 1] = '\0'; // Rimuove il newline
        sscanf(comando, "%s %s", argomento1, argomento2);

        int scelta = indirizzatore(argomento1);

        if (scelta == 1) { // view
            for (int i = 0; i < indice; i++) 
            {
                char stato[MAXLN];
                int valoreSemaforo;
                sem_getvalue(semafori[i], &valoreSemaforo);
                if (valoreSemaforo > 0) {
                    strcpy(stato, "free");
                } else {
                    strcpy(stato, "busy");
                }
                printf("Car: %s, status: %s\n", nomi_semafori[i], stato);
            }
        } else if (scelta == 2) { // lock
            int index = whereIs(argomento2, nomi_semafori, indice);
            if (index > -1) {
                if (sem_trywait(semafori[index]) == 0) {
                    printf("Car: %s is now locked\n", argomento2);
                } else {
                    printf("Car: %s is alreay locked\n", argomento2);
                }
            } else {
                printf("Cannot find car %s\n", argomento2);
            }
        } else if (scelta == 3) { // release
            int index = whereIs(argomento2, nomi_semafori, indice);
            if (index > -1) 
            {
                int valoreSemaforo;
                sem_getvalue(semafori[index], &valoreSemaforo);
                if (valoreSemaforo == 0) {
                    if (sem_post(semafori[index]) == 0) {
                        printf("Car: %s is now free\n", argomento2);
                    }
                } else {
                    printf("Error. Car %s is already free\n", argomento2);
                }
            } else {
                printf("Cannot find car %s\n", argomento2);
            }
        } else if (scelta == 4) { // quit
            break;
        } else {
            printf("Unknown Command\n");
        }
    }

    // chiusura finale dei semafori
    for (int i = 0; i < indice; i++) {
        sem_close(semafori[i]);
        sem_unlink(nomi_semafori[i]);
    }

    return 0;
}
