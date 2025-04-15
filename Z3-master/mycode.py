from z3 import *

def hanoi_z3(n, num_towers):
    solver = Solver()

    # Numero di mosse necessarie
    num_moves = 2**n - 1

    # Variabili: per ogni mossa e ogni disco, indica su quale torre si trova
    # struttura dati FONDAMENTALE: tower è lista di liste e rappresenta la torre in cui si trova il dico disk alla mossa move
    # essenziale per modellare il problema

    tower = [[Int(f'tower_{move}_{disk}') for disk in range(n)] for move in range(num_moves + 1)]
    #print(tower)

    # Vincolo: le torri devono essere sempre 1, 2 o 3
    for move in range(num_moves + 1):
        for disk in range(n):
            solver.add(And(tower[move][disk] >= 1, tower[move][disk] <= num_towers))

    # Vincolo iniziale: tutti i dischi sono sulla torre 1 alla prima mossa (mossa 0)
    for disk in range(n):
        solver.add(tower[0][disk] == 1)

    # Vincolo finale: tutti i dischi sono sulla torre 3 alla fine delle mosse, quindi idealmente a num_moves mosse 2^n - 1
    for disk in range(n):
        solver.add(tower[num_moves][disk] == num_towers)

    # Vincoli di validità delle mosse
    # per ogni mossa nel range num_moves, i dischi devono avere dei vincoli
    #dichiariamo is_top, AND logico TRUE se il disco sta in cima FALSE altrimenti
    for move in range(num_moves):
        for disk in range(n):
            # Vincolo: un disco può essere spostato solo se è in cima alla sua torre
            #il for itera sui dischi piu piccoli di 'disk' e verifica che il disco sia in cima
            is_top = And([tower[move][i] != tower[move][disk] for i in range(disk)])

            #aggiungiamo una implicazione, se il disco viene spostato:
            #1: il disco deve stare in cima alla torre
            #2: non si può spostare un disco su un disco piu piccolo

            solver.add(Implies(
                tower[move][disk] != tower[move + 1][disk],  # Se il disco viene spostato
                And(
                    is_top,  # Il disco deve essere in cima alla sua torre
                    # Non si può spostare un disco su un disco più piccolo
                    And([tower[move + 1][i] != tower[move + 1][disk] for i in range(disk)])
                )
            ))

    # Vincolo: solo un disco può essere spostato per mossa
    # vincolo di somma: IDEA: creo una lista booleana che mi dice i dischi mossi per mossa, e impongo che la somma totale sia uguale a 1
    for move in range(num_moves):
        moved_disks = [tower[move][disk] != tower[move + 1][disk] for disk in range(n)]
        solver.add(Sum([If(moved, 1, 0) for moved in moved_disks]) == 1)


    # Risoluzione del problema
    if solver.check() == sat:
        model = solver.model()
        # lista delle mosse della torre di Hanoi

        for move in range(num_moves + 1):
            print(f'Move {move}:')
            for disk in range(n):
                print(f'  Disk {disk + 1} is on Tower {model[tower[move][disk]]}')
    else:
        print("No solution found")


def main():
    #INPUT UTENTE
    print("TORRE DI HANOI")
    n = 0
    t = 0
<<<<<<< HEAD
    while n == 0:
=======
    while n == 0 and t == 0:
>>>>>>> 0a8dcf2 (primo commit da linux)
        print("Inserisci quanti dischi vuoi avere: ")
        n = int(input())
        print("Inserisci quante torri vuoi avere: ")
        t = int(input())
        if n == 0 or t == 0:
            print("Non ci possono essere zero dischi")
        else:
            hanoi_z3(n, t)

<<<<<<< HEAD
=======
#IDEA: IMPLEMENTARE ANIMAZIONI CHE MOSTRANO GLI STATI, MAGARI CON PYGAME
>>>>>>> 0a8dcf2 (primo commit da linux)
#ENTRY POINT
if __name__ == '__main__':
    main()
