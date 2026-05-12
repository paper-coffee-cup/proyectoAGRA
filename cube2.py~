"""
Tarea 1
A - Problem A

Nombre: Isabel Sofía Adrada Arboleda, Juan Luis Guevara Bustamante
Código de estudiante: 8977586,

"""

from sys import stdin
from heapq import heappop, heappush

def move(d, rc, pos, m, g, cg, R, C):
    #0 = north, 1 = east, 2 = south, 3 = west
    op = {0: 2, 1: 3, 2: 0, 3: 1, 4: 5, 5: 4}
    
    if d == 0:
        ans = (pos[1], op[pos[0]], pos[0])
        flag = rc[0] - 1 >= 0
        nrc = (rc[0] - 1, rc[1])
    elif d == 1:
        ans = (pos[2], pos[1], op[pos[0]])
        flag = rc[1] + 1 < C
        nrc = (rc[0], rc[1] + 1)
    elif d == 2:
        ans = (op[pos[1]], pos[0], pos[2])
        flag = rc[0] + 1 < R
        nrc = (rc[0] + 1, rc[1])
    else:
        ans = (op[pos[2]], pos[1], pos[0])
        flag = rc[1] - 1 >= 0
        nrc = (rc[0], rc[1] - 1)

    if m[nrc[0]][nrc[1]] == 'G':
        if g[ans[0]]:
            cg += 1
        g[ans[0]] = True
    
    return (flag, nrc, ans, g, cg)

def dijkstra(m, rc, R, C, A, B):
    #estado: ((row, col), (floor, north, east), gold, cgold)
    
    act = (0, rc, (0, 1, 4), [False, False, False, False, False, False], 0)
    vis = {(rc, (0, 1, 4), [False, False, False, False, False, False], 0): 0}
    flag = False
    cost = 0
    ans = 0
    q = []
    heappush(q, act)

    while not flag and len(queue) > 0:
      c, rc, pos, g, cg = heappop(q)
      i = 0
      while not flag and i < 4:
          nflag, nrc, npos, ng, ncg = move(i, rc, pos, m, g, cg, R, C)
          
          ac = A
          if ncg > cg:
              ac = B
          
          if nflag and ((nrc, npos, ng, ncg) not in vis or vis[(nrc, npos, ng, ncg)] > c + ac):
              vis[(nrc, npos, ng, ncg)] = c + ac
              
              if ncg == 6:
                  flag = True
                  cost = c + ac
              else:
                  heappush(q, (c + ac, nrc, npos, ng, ncg))

    return (flag, cost)
        

def main():
    T = int(stdin.readline())
    for i in range(T):
        R, C, A, B = list(map(int, stdin.readline().split()))
        m = [[[] for _ in range(C)] for _ in range(R)]
        for j in range(R):
            aux = stdin.readline().strip()
            for k in range(C):
                m[j][k] = aux[k]
                if aux[k] == 'S':
                    rc = (j, k)
                
        flag, cost = dijkstra(m, rc, R, C, A, B)
        
        if flag:
            print("Screw you guys, I got all the gold for", cost, "cost!")
        else:
            print("Oh my God, they killed Kenny!")        

main()
