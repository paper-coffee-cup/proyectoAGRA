"""
Tarea 1
A - Problem A

Nombre: Isabel Sofía Adrada Arboleda, Juan Luis Guevara Bustamante
Código de estudiante: 8977586,

"""

from sys import stdin
from heapq import heappop, heappush

def move(dir, pos):
    op = {0: 2, 1: 3, 2: 0, 3: 1, 4: 5, 5: 4}
    
    #0 = north, 1 = east, 2 = south, 3 = west
    if dir == 0:
        ans = (pos[1], op[pos[0]], pos[0])
    elif dir == 1:
        ans = (pos[2], pos[1], op[pos[0]])
    elif dir == 2:
        ans = (op[pos[1]], pos[0], pos[2])
    else:
        ans = (op[pos[2]], pos[1], pos[0])
    return ans

def dijkstra(m, rc, A, B):
    gold = [False for _ in range(6)]
    vis = set()
    count = 0
    cost = 0
    q = []
    act = (rc, (0, 1, 4), 0)

    while count < 6 and len(queue) > 0:
      rc, pos, count = heappop(q)
      
      #problema de mario
      if u == a + b: flag = True
      else:
        if vis[(u, l, k)] == c:
          # se coloca los zapatos especiales
          if l != L and k > 0:
            if (u, L, k - 1) not in vis or vis[(u, L, k - 1)] > c:              
              heappush(queue, (c, u, L, k - 1))
              vis[(u, L, k - 1)] = c
          for (v, p) in G[u]:
            if (v, 0, k) not in vis or vis[(v, 0, k)] > c + p:           
              # se mueve sin usar los zapatos
              heappush(queue, (c + p, v, 0, k))
              vis[(v, 0, k)] = c + p
            # se mueve usando los zapatos
            if l >= p and (u <= a or l == L):
              if (v, l - p, k) not in vis or vis[(v, l - p, k)] > c:
                heappush(queue, (c, v, l - p, k))
                vis[(v, l - p, k)] = c

    return (count == 6, cost)
        

def main():
    T = int(stdin.readline())
    for i in range(T):
        R, C, A, B = list(map(int, stdin.readline().split()))
        m = [[[] for _ in range(C)] for _ in range(R)]
        for j in range(R):
            aux = stdin.readline().strip()
            for k in range(C):
                m[j][k] = aux[k]
                if m[j][k] == 'S':
                    rc = (j, k)
                
        flag, cost = dijkstra(m, rc, A, B)
        
        if flag:
            print("Screw you guys, I got all the gold for", cost, "cost!")
        else:
            print("Oh my God, they killed Kenny!")        

main()
