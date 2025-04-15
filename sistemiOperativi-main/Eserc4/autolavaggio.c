#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
#include <sys/stat.h> //necessario per stat
#include <sys/wait.h> //necessario per wait
#include <dirent.h> // per listare contenuto di una directory
#include <string.h>
#include <stdbool.h>
#include <sys/stat.h>


#include <pthread.h>
#include <semaphore.h>
#include <fcntl.h>

/*
    AUTONOLLEGGIO

    gestione da parte di più operatori che lavorano in maniera concorrente

*/


//Definizione delle costanti per i dati
#define MAXCMD 100
#define MAXLN 256
#define MAXCAR 256

//comandi che da a disposizione il programma
char comandiDisponibili[4][MAXCMD] = {
    {"view\0"},
    {"lock\0"},
    {"release\0"},
    {"quit\0"}
};


//definizione dei semafori, 
//TODO: farli named, uno per ogni vettura del file
//funzione che indirizza il comando!
int indirizzatore(char *command)
{
    //default
    int caso = 1;

    if(strcmp(command, comandiDisponibili[0]) == 0){
        caso = 1;
    }else if(strcmp(command, comandiDisponibili[1]) == 0){
        caso = 2;
    }else if(strcmp(command, comandiDisponibili[2]) == 0){
        caso = 3;
    }else if(strcmp(command, comandiDisponibili[3]) == 0){
        caso = 4;
    }else{
        //default
        caso = 5;
    }

    return caso;
}


int whereIs(char vettura[100], char elenco[MAXCAR][MAXLN], int dim)
{
    int check = -1;

    for(int i = 0; i < dim; i++){
        int comparazione = strcmp(vettura, elenco[i]);
        if(comparazione == 0){
            check = i;
            break;
        }
    }

    return check;
}


//TODO: gestire bene il comando dell'utente

int main()
{
    //lettura da file e inserimento dei dati in un array di stringhe
    FILE *file;
    file = fopen("catalog.txt","r");

    //gestione erroe in caso di fail
    if(file == NULL){
        printf("errore, non è presente il file elencato...\n");
        return 1;
    }

    //gestione semafori e file
    char buffer[MAXLN]; //buffer
    char nomi_semafori[MAXCAR][MAXLN];
    sem_t *semafori[MAXCAR];
    int indice = 0;

    while(fgets(buffer, MAXLN, file) != NULL)
    {
        buffer[strlen(buffer)-1] = '\0';

        sem_t * sem = sem_open(buffer, O_CREAT | O_EXCL , __S_IREAD | __S_IWRITE, 1);
        if(sem == SEM_FAILED)
        {
            // il semaforo esiste già, rimediamo...
            sem_t *semaforo = sem_open(buffer, O_CREAT);
            semafori[indice] = semaforo;
            if(semaforo == NULL)
            {
                printf("Attenzione\n");
                continue;
            }
        }else{
            semafori[indice] = sem;
        }
        strcpy(nomi_semafori[indice], buffer);
        indice++;
    }
    fclose(file);
   

    //ciclo infinito, sezione utente

    //TODO, FINIRE GESTIONE SEMAFORI E COMANDI!
    while(true)
    {
        //chiediamo all'utente un comando e lo conserviamo
        //variabile temporanea e funzioni su strighe
        /*
        char temp[100];
        char argument[100];
        int count = 0;
        printf("\nComando: ");
        fgets(temp, sizeof(temp), stdin);

        char *argomento = strtok(temp, " ");
        char comando[100];
        //comando iniziale
        strcpy(comando, argomento);
        comando[strlen(comando)] = '\0';
        
        //se sono due, argomento != NULL quindi si apre il ciclo
        //DONE, FUNZIONA a volte
        while(argomento != NULL) {
            strcpy(argument, argomento);
            argument[strlen(argument)-1] = '\0';
            argomento = strtok(NULL, " ");
        }

        */
        char comando[MAXLN];
		char argomento1[MAXLN];
		char argomento2[MAXLN];

		printf("Command: ");
		fgets(comando, sizeof(comando), stdin);
		// Parses command in arg1, arg2
		comando[strlen(comando)-1] = '\0';
		sscanf(comando, "%s %s", argomento1, argomento2);
        //printf("%s", argument);

        //si utilizzano le variabili comando e argomento
        
        int scelta = (int)indirizzatore(argomento1);
        if(scelta == 1){ //DONE
            //view
            for(int i = 0; i < indice; i++)
            {
                char stato[MAXLN]; 
                int try;
                try = sem_trywait(semafori[i]);
                
                if(try == 0){
                  strcpy(stato, "free");
                  sem_post(semafori[i]);
                }else{
                    strcpy(stato, "busy");
                }
                //printf("semaforo, %s\n", nomi_semafori[i]);
                printf("Car: %s, status: %s\n", nomi_semafori[i], stato);
            }

            continue;

        }else if(scelta == 2){
            //lock FACENDO UNA WAIT AL SEMAFORO GIUSTO
            int index = whereIs(argomento2, nomi_semafori, indice);
            //printf("%d", index);
            if(index > -1){
                int try;
                try = sem_trywait(semafori[index]);
                //printf("%d", try);
                if(try == 0){
                    sem_wait(semafori[index]);
                    printf("Car: %s is now locked\n", argomento2);
                }else{
                    printf("Car %s is already locked\n", argomento2);
                }
            }else{
                printf("Cannot find car %s\n", argomento2);
            }
            continue;

        }else if(scelta == 3){
            //release

            int index = whereIs(argomento2, nomi_semafori, indice);
            
            if(index < 0){
                printf("Cannot find car %s\n", argomento2);
            }else{
                int valoreSemaforo;
                sem_getvalue(semafori[index], &valoreSemaforo);
                if(valoreSemaforo < 1){
                    sem_post(semafori[index]);
                    printf("Car: %s is now free\n", argomento2);
                }else{
                    printf("Error. Car %s already free\n", argomento2);
                }
            }
            
        }else if(scelta == 4){
            //quit
            //chiusura semafori e chiusura programma
            break;

        }else{
            //comando non conosciuto! errore dell'utente
            printf("Unknow Command\n");  
        }
    }
    //chiusura finale dei semafori
    for(int i = 0; i < indice; i++){
        sem_close(semafori[i]);
    }
    
    return 0;
}