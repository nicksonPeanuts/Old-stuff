import random
from z3 import *


def generate_n_queens(N, num_fixed_queens):
    #   Genera una scacchiera N x N con alcune regine fissate casualmente.
    board = [[0 for _ in range(N)] for _ in range(N)]
    #print(board)
    fixed_queens = []

    for _ in range(num_fixed_queens):
        while True:
            row, col = random.randint(0, N - 1), random.randint(0, N - 1)
            if board[row][col] == 0:  # Se la casella è libera
                board[row][col] = 1
                fixed_queens.append((row, col))
                break
    return board, fixed_queens


def solve_n_queens(N, fixed_queens):
    """Risolvi il problema delle N-Regine con Z3."""
    solver = Solver()

    # Creazione delle variabili: q[i] = colonna della regina nella riga i
    queens = [Int(f'q_{i}') for i in range(N)]

    # Vincoli:
    # 1. Ogni regina è in una colonna valida (0 <= q[i] < N)
    for q in queens:
        solver.add(0 <= q, q < N)

    # 2. Regine fissate (input utente)
    for row, col in fixed_queens:
        solver.add(queens[row] == col)

    # 3. Nessuna regina può attaccarsi:
    #    - Stessa colonna: q[i] != q[j]
    #    - Stessa diagonale: |q[i] - q[j]| != |i - j|
    for i in range(N):
        for j in range(i + 1, N):
            solver.add(queens[i] != queens[j])
            solver.add(queens[i] - queens[j] != i - j)
            solver.add(queens[i] - queens[j] != j - i)

    # Verifica se esiste una soluzione
    if solver.check() == sat:
        model = solver.model()
        solution = [[0 for _ in range(N)] for _ in range(N)]

        # Aggiungi le regine fissate
        for row, col in fixed_queens:
            solution[row][col] = 1

        # Aggiungi le regine trovate da Z3
        for i in range(N):
            col = model.evaluate(queens[i]).as_long()
            if solution[i][col] == 0:  # Se non è una regina fissata
                solution[i][col] = 2  # Contrassegna le nuove regine

        print("SAT: Soluzione trovata!")
        print_board(solution, fixed_queens)
        return True
    else:
        print("UNSAT: Nessuna soluzione possibile.")
        return False


def print_board(board, fixed_queens):
    """Stampa la scacchiera con le regine."""
    N = len(board)
    for i in range(N):
        for j in range(N):
            if (i, j) in fixed_queens:
                print("Q", end=" ")  # Regine fissate
            elif board[i][j] == 2:
                print("q", end=" ")  # Regine trovate da Z3
            else:
                print(".", end=" ")  # Cella vuota
        print()


# Configurazione
# Genera e risolvi

#FARE IN MODO CHE UTENTE DECIDA SE FAR GENERARE CASUALMENTE OPPURE SE SCEGLIERE LA POSIZIONE DELLE REGINE

while True:
    # 1 SCELTA DELL'UTENTE

    # 2 CASUALMENTE
    print("inserisci le dimensioni della scacchiera N*N: ")

    # Dimensione della scacchiera
    N = int(input("inserisci la dimensione della scacchiera: "))

    # Numero di regine fissate casualmente
    num_fixed_queens = int(input("inserisci il numero di regine fissate casualmente: "))
    #print(f"Scacchiera {N}x{N} con {num_fixed_queens} regine fissate casualmente:")
    tentativi = 0
    while True:
        tentativi += 1
        board, fixed_queens = generate_n_queens(N, num_fixed_queens)

        #print("Regine fissate:", fixed_queens)
        if solve_n_queens(N, fixed_queens) == True:
            #print_board(board, fixed_queens)
            print(f"Numero di tentativi: {tentativi}")
            break
        if tentativi > 100:
            print("ci sono voluti più di 100 tentativi, riprova...")
            break

    #richiesta all'utente per continuare o meno il gioco
    if input("vuoi continuare il gioco? Y/N") == "N":
        print("terminazione...")
        break
