#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
#include <sys/stat.h> //necessario per stat
#include <sys/wait.h> //necessario per wait
#include <dirent.h> // per listare contenuto di una directory
#include <string.h>
#include <stdbool.h>
#include <pwd.h> // per utenti
#include <grp.h> // per gruppi

//Definizione delle costanti per i dati
#define MAXPROCESSES 256
#define MAXCMD 256
#define MAXL 256

/*

terza esercitazione di laboratorio di sistemi operativi

*/

// rapido check del numero di argomenti del programma
int checkArg(int argvNumber)
{
    int check = 1;
    //controllo che ci siano 4 argomenti
    if(argvNumber != 4){
        printf("Errore!, bisogna inserire 3 argomenti\n");
        check = 0;
    }
    return check;
}


int main(int argc, char *argv[])
{
    //se da errore esco, messaggio già gestito dalla funzione
    if(!checkArg(argc)){
        exit(1);
    }
    
    //numero dei processi da eseguire in parallelo
    int num_processi = *argv[2];
    
    //LETTURA DEL FILE args.txt
    FILE *file;
    file = fopen(argv[1],"r");

    //gestione erroe in caso di fail
    if(file == NULL){
        printf("errore, non è presente il file elencato...\n");
        return 1;
    }
    
    //DATI
    char buffer_file [MAXCMD]; // usato per leggere le righe del file
    char fincomando[MAXCMD]; // parte finale del comando
    char argomento[MAXCMD];
    char bufferCommands [MAXCMD][MAXL]; // insieme dei comandi

    //sezionamento del comando
    
    // tolgo il carattere speciale!!
    strcpy(argomento, argv[3]);
    char *porzioneComando = strstr(argomento, "%"); // è il puntatore alla riga
    if(porzioneComando == NULL){
        printf("Errore, nel terzo argomento va inserito il carattere speciale! \n");
        strcpy(porzioneComando, buffer_file);
        return 1;
    }

    //porzioneComando
    strcpy(porzioneComando, porzioneComando+1);
    strcpy(fincomando, porzioneComando);
    //ora porzione fin comando è una stringa col valore della stringa finale del comando!!
    
    
    //QUANTI PROGRAMMI HO?
    int counter = 0;
    
    while(fgets(buffer_file, MAXCMD, file) != NULL)
    {
        buffer_file[strlen(buffer_file)-1] = '\0';
        //puntatori!!!
        strcat(buffer_file, fincomando);
        // IN QUESTA PARTE porzioneComando fa da puntatore, copiando qui, copio nella zona di memoria dopo il carattere speciale
        strcpy(porzioneComando, buffer_file);
        //printf("%s\n", argomento);
        strcpy(bufferCommands[counter], argomento);
        
        /*
            WORKING !!!
        */
        counter++;
    }

    
    // gestione processi
    int MATRpipe[MAXPROCESSES][2];

    int numProcessi = *argv[2];

    for(int i = 0; i < num_processi; i++)
    {
        int *flagPipe = MATRpipe[i];
        pipe(flagPipe);

        //code here
        if(fork()){
            close(flagPipe[0]);
            //iterazione per num_processi volte
            for(int h = i; h < counter; h++)
            {
                write(flagPipe[1], bufferCommands[h], MAXCMD);
            }
            close(flagPipe[1]);
            continue;
            //padre
            //terminare quando finisce di esegure il comando


        }else{
            char comando[MAXCMD];
            //VANNO CHIUSE LE PIPE APERTE,
            
            for(int k = 0; k <= i; k++){
                close(MATRpipe[k][1]);
            }

            while(read(flagPipe[0], comando, MAXCMD) > 0){
                system(comando);
            }
            exit(0);
            //figlio


        }
    }

    while(wait(NULL) > 0){};

    return 0;
}











