from z3 import *

#funzione che ci restituisce il risultato per la torre di hanoi

def hanoi(n):
    solver = Solver()

    #limite superiore delle mosse nel gioco dell'hanoi

    max_mosse = 2**n - 1

    #dichiarazioni variabili usate dal solver in formato lista
    moves = [Int(f'move_{step}') for step in range(max_mosse)]
    fromTower = [Int(f'fromT_{step}') for step in range(max_mosse)]
    toTower = [Int(f'toT_{step}') for step in range(max_mosse)]


    #vincoli di z3, ogni elemento della lista viene vincolato secondo il problema di hanoi
    for i in range(max_mosse):
        # non si può muovere per un numero maggiore di n volte
        solver.add(And(moves[i] >= 1, moves[i] <= n))
        # non si può spostare da una torre <1 a una >3, abbiamo 3 torri a disposizione
        # DAFARE: rendere più modulare il tutto
        solver.add(And(fromTower[i] >= 1, fromTower[i] <= 3))
        solver.add(And(toTower[i] >= 1, toTower[i] <= 3))

    #gestiore algoritmica, metodo ricorsivo? da implementare
    def hanoi_algorithm():
        raise NotImplementedError

    hanoi_algorithm()

    #risoluzione del problema
    if solver.check() == sat:
        #risolvi e fornisci il modello
    else:
        print("Non c'e' un modello che soddisfa il problema")

hanoi(4)