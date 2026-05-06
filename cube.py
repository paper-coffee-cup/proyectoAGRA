"""
Tarea 1
A - Problem A

Nombre: Isabel Sofía Adrada Arboleda, Juan Luis Guevara Bustamante
Código de estudiante: 8977586,

"""

from sys import stdin

def main():
    T = int(stdin.readline())
    for i in range(T):
        R, C, A, B = list(map(int, stdin.readline().split()))
        m = [[[] for _ in range(C)] for _ in range(R)]
        for j in range(R):
            aux = stdin.readline().strip()
            for k in range(C):
                m[j][k] = aux[k]

        print(m)

main()
