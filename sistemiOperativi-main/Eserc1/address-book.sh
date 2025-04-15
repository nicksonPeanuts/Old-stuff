#bin\bash\

#controllo degli argomenti del comando
if [[ $# == "0" ]]
then 
    echo "serve almeno un argomento"
    else
        #per variabili ricorda di scrivere attaccato, senno' lo conta come comando
        #se check = 1 continua il programma senno' termina
        check=1
fi


#controllo degll'argomento argomenti e indirizzazione al comando da eseguire
#dataset = address-book-database.csv

#inizio del programma vero e proprio
if [[ $check == 1 ]]
then
    if [[ $1 = "view" ]]
    then
         
        #stampa file ordinato per mail
        rows_file=$(wc -l < address-book-database.csv)
        
        #numero file -1 perchè non contato l'header
        rows_file=$(($rows_file - 1)) 

        #stampa l'header incolonnato
        cat address-book-database.csv | head -n 1 | column -s "," -o " " -t 

        #stampa il file incolonnato e ordianto
        cat address-book-database.csv | tail -n $rows_file | column -s "," -o " " -t | sort -k 4 

        elif [[ $1 == "search" ]]
        
        then  
            #controlla se ci sono 2 argomenti, senno' avvisa l'utente
            if [[ "$#" == "2" ]]
            then
                
                #inizializza variabile con dentro l'input dell'utente, il secondo parametro del comando search
                string=$2

                #numero di righe del match, non puo' essere maggiore delle righe del database, non c'è richiesta tuttavia 
                #da parte dell'esercizio
                matches=$(grep -c $string address-book-database.csv)
                rows_file=$(wc -l < address-book-database.csv)
                rows_file=$(($rows_file - 1)) 

                #controllo il numero di match nel file
                if (( $matches != 0 ))
                then
                    IFS=$'\n'
                    
                    #navigo nelle righe del file per trovare corrispondenza
                    for line in $(cat address-book-database.csv | tail -n $rows_file)
                    do
                        check_match=$(echo $line | grep -c $string )
                        #se c'e' il match nella riga...eseguo la stampa formattata bene
                        if (( $check_match != 0 ))
                        then
                            #usare echo e non cat!!
                            line_matched=$(echo $line | grep $string)

                            #for per eseguire la 
                            for (( i = 1; i <= 6; i++ ));
                            do
                                #cutto lungo l'i-esimo campo (field)
                                indice=$(head -1 address-book-database.csv | cut -d "," -f $i)
                                campo=$(echo $line_matched | cut -d "," -f $i)
                                echo ${indice^}: $campo
                                
                            done
                            #escape, vado a capo ogni volta
                            echo -e
                        fi

                    done

                    else
                        echo "Not found"
                fi

                else
                    echo "va inserita anche una stringa" 
            fi
            
            elif [[ $1 = "insert" ]]
            then
                #idea: costruisco piano piano la riga da inserire, aggiungendo la virgola alla fine ogni volta
                #inizializzo la variabile line, la nostra linea e la variabile in caso di errore
                line=""
                error=0
                indice=$(head -1 address-book-database.csv | cut -d "," -f 1)

                echo -n ${indice^}: " "
                read input
                line="$input"


                for (( i = 2 ; i <= 7 ; i++));
                do
                    indice=$(head -1 address-book-database.csv | cut -d "," -f $i)
                    #campo=$(echo $line_matched | cut -d "," -f $i)

                    if (( $i < 7 ))
                    then
                        #caso della mail, è il campo 4, 
                        if (( $i == 4 ))
                        then

                            echo -n ${indice^}: " "
                            read input
                            #check della mail nell'intero file
                            matches=$(grep -c $input address-book-database.csv)
                            if (( $matches == 0 ))
                            then
                                line="$line,$input"
                                else
                                    error=1
                            fi
                            #se non è la mail, processa il resto!
                            else 
                                echo -n ${indice^}: " "
                                read input
                                line="$line,$input"
                        fi
                    fi
                    
                done

                #se non ci sono errori, procedere all'aggiornamento database
                if (( $error != 1 ))
                then
                    echo $line >> address-book-database.csv
                    echo "Added"
                    else
                        echo "Errore!, mail gia' presente nella lista"
                        echo "aggiornamento non effettuato"
                fi

                elif [[ $1 = "delete" ]]
                then
                if [[ "$#" == 2 ]]
                then
                    IFS=$'\n'

                    mail=$2
                    check_error=0
                    search=$(grep -c $mail address-book-database.csv)
                    if (( $search != 0 ))
                    then
                        counter=1
                        for line in $(cat address-book-database.csv)
                        do
                            check=$(echo $line | grep -c $mail)
                            if (( $check != 0 ))
                            then
                                sed -i "${counter}d" address-book-database.csv
                                echo "Deleted"
                            fi
                            counter=$(( counter + 1 ))
                        done

                        else
                            echo "Cannot find any record"
                    fi

                    else
                        echo "bisogna inserire un'altro argomento!"
                fi
    fi
fi