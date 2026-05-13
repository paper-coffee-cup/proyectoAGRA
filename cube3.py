"""
Tarea 1
A - Problem A

Nombre: Isabel Sofía Adrada Arboleda, Juan Luis Guevara Bustamante
Código de estudiante: 8977586,

"""

from sys import stdin
from heapq import heappop, heappush

def verBit(n, pos):
    return bool(n & (1 << 5 - pos))

def swap(n, pos):
    return n ^ (1 << 5 - pos)

def move(d, rc, pos, gm, g, R, C):
    #0 = north, 1 = east, 2 = south, 3 = west
    op = [2, 3, 0, 1, 5, 4]
    
    if d == 0:
        ans = (pos[1], op[pos[0]], pos[2])
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
    
    if flag and nrc in gm:
        if not verBit(g, ans[0]):
            g = swap(g, ans[0])
            gm.remove(nrc)
    elif flag:
        if verBit(g, ans[0]):
            g = swap(g, ans[0])
            gm.add(nrc)

    return (flag, nrc, ans, g, gm)

def dijkstra(rc, gold, R, C, A, B):
    #estado: ((row, col), (floor, north, east), g, goldMap)
    
    act = (0, rc, (0, 1, 4), 0, gold.copy())
    vis = {(rc, (0, 1, 4), 0, frozenset(gold)): 0}
    flag = False
    cost = 0
    q = []
    heappush(q, act)

    while not flag and len(q) > 0:
      c, rc, pos, g, gm = heappop(q)
      i = 0
      while vis[(rc, pos, g, frozenset(gm))] == c and not flag and i < 4:
        nflag, nrc, npos, ng, ngm = move(i, rc, pos, gm.copy(), g, R, C)
        ngm = ngm.copy()
        ac = A
        if ng > g:
            ac = B
            
        if nflag:
            fngm = frozenset(ngm)
            if ((nrc, npos, ng, fngm) not in vis or vis[(nrc, npos, ng, fngm)] > c + ac):
                vis[(nrc, npos, ng, fngm)] = c + ac
            
                #63 = 111111
                if ng == 63:
                    flag = True
                    cost = c + ac
                else:
                    heappush(q, (c + ac, nrc, npos, ng, ngm))
        i += 1
    return (flag, cost)
        

def main():
    T = int(stdin.readline())
    for i in range(T):
        R, C, A, B = list(map(int, stdin.readline().split()))
        gold = set()
        for j in range(R):
            aux = stdin.readline()
            for k in range(C):
                if aux[k] == 'G':
                    gold.add((j, k))
                elif aux[k] == 'S':
                    rc = (j, k)

        flag, cost = dijkstra(rc, gold, R, C, A, B)
        
        if flag:
            print("Screw you guys, I got all the gold for", cost, "cost!")
        else:
            print("Oh my God, they killed Kenny!")        

main()
