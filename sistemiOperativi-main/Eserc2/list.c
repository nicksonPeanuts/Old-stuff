#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
#include <sys/stat.h> //necessario per stat
#include <dirent.h> // per listare contenuto di una directory
#include <string.h>
#include <stdbool.h>
#include <pwd.h> // per utenti
#include <grp.h> // per gruppi



/*

file per la stampa dell'albero dei file in linux!
ricorda: il massimo per la stringa d_name e' 256 caratteri!


*/

//recursive function, stampa se trova una directoru
void stampaRicorsiva(char *path)
{

    //apro la directory
    DIR *directory;
    directory = opendir(path);

    //gestione errore
    //se non è una directory!
    if(directory == NULL)
    {
        printf("%s","Non è una directory!");
        exit(1);
    }

    struct stat buf; // dati del file

    //variabile per decidere se fare la ricorsione o meno
    bool ricorsione = false;

    //la struct con i dati della directory
    struct dirent *entrata;

    //leggi gli elementi della directory, uno ad uno
    //readdir ritorna una struct dirent*
    while((entrata = readdir(directory)) != NULL)
    {
        char percorso_file[1024];
        //printf("%s", "heyyy");
        if(strlen(percorso_file) + strlen(path) > 256){
            printf("%s","Errore, percorso del file troppo grande da gestire!");
            exit(1);
        }

        //gestione del path, concateno e creo il percorso
        //ci sarà sempre il nome dell "oggetto" che sto considerando
        strcpy(percorso_file, path);
        strcat(percorso_file, "/");
        strcat(percorso_file, entrata->d_name);
        char tipo[100];

        //gestione dell'errore nella lettura del symbolic link o file

        if(lstat(percorso_file, &buf) < 0){
            printf("%s","Errore: file illegibile\n");
            exit(1);
        }
        //stat(percorso_file, &buf);

        //controllo del tipo di file
        if(S_ISREG(buf.st_mode))
        {
            strcpy(tipo, "file");
        }
        else if(S_ISLNK(buf.st_mode))
        {
            strcpy(tipo, "symbolic link");
        }
        else if(S_ISDIR(buf.st_mode))
        {
            //CASO RICORSIVO!

            //eliminazione dei casi "." e ".."
            if(strcmp(".", entrata->d_name) == 0 || strcmp("..", entrata->d_name) == 0){
                continue;
            }
            ricorsione = true;
            strcpy(tipo, "directory");
        }
        else if(S_ISFIFO(buf.st_mode))
        {
            strcpy(tipo, "FIFO");

        }else{
            strcpy(tipo, "other");
        }

        //stampa delle informazioni,
        struct passwd *pwd = getpwuid(buf.st_uid);
        char *utente = pwd->pw_name;

        struct group *gruppo = getgrgid(buf.st_gid);
        char *grp = gruppo->gr_name;

        //printf("%s", "heyyy");
        printf("Node: %s \n\tInode: %ld \n\tType: %s \n\tSize: %ld \n\tOwner: %ld %s \n\tGroup: %ld %s \n",percorso_file ,(long)buf.st_ino ,tipo ,(long)buf.st_size ,(long)buf.st_uid ,utente ,(long)buf.st_gid, grp);
        /*
        printf("Node: %s", percorso_file);
        printf("\n\tInode: %ld", (long)buf.st_ino);
        printf("\n\tType: %s", tipo);
        printf("\n\tSize: %ld", (long)buf.st_size);
        printf("\n\tOwner: %d %s", buf.st_uid, utente);
        printf("\n\tGroup: %ld %s \n", (long)buf.st_gid, grp);
        */
        //in caso vado alla ricorsione
        if(ricorsione == true){
            //printf("%s", "heyy");
            stampaRicorsiva(percorso_file);
            //printf("%s", "heyy");
        }
    }
    closedir(directory);
}


int main(int argc, char *argv[])
{
    //inizializzo la struct buf, informazioni sul file/directory
    struct stat buf;


    //argv[1] stringa del primo argomento del programma
    if(argc != 2){
        printf("specifica un path\n");
        return 1;
    }

    if(stat(argv[1], &buf) < 0)
    {
        printf("impossibile leggere le informazioni del path immesso");
        exit(1);
    }
    else
    {
        //controllo se e' una directory
        if(!S_ISDIR(buf.st_mode))
        {
            printf("Attenzione, il path non contiene una directory");
            exit (1);
        }

        struct passwd *pwd = getpwuid(buf.st_uid);
        char *utente = pwd->pw_name;

        struct group *gruppo = getgrgid(buf.st_gid);
        char *grp = gruppo->gr_name;

        printf("Node: %s \n\tInode: %ld \n\tType: %s \n\tSize: %ld \n\tOwner: %ld %s \n\tGroup: %ld %s \n",argv[1], (long)buf.st_ino ,"Directory" ,(long)buf.st_size ,(long)buf.st_uid ,utente ,(long)buf.st_gid, grp);
        /*
        printf("Node: %s", argv[1]);
        printf("\n\tInode: %ld", (long)buf.st_ino);
        printf("\n\tType: %s", "Directory");
        printf("\n\tSize: %ld", (long)buf.st_size);
        printf("\n\tOwner: %d %s", buf.st_uid, utente);
        printf("\n\tGroup: %ld %s \n", (long)buf.st_gid, grp);
        */
        stampaRicorsiva(argv[1]);
    }
    return 0;
}
