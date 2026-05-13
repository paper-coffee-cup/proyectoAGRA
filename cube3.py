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

def move(d, rc, R, C):
    #0 = north, 1 = east, 2 = south, 3 = west
    dire = ((-1, 0), (0, 1), (1, 0), (0, -1))
    
    nrc = (rc[0] + dire[d][0], rc[1] + dire[d][1])
    flag = nrc[0] >= 0 and nrc[0] < R and nrc[1] >= 0 and nrc[1] < C
    return (flag, nrc)

def roll(d, nrc, pos):
    #0 = north, 1 = east, 2 = south, 3 = west
    op = (2, 3, 0, 1, 5, 4)
    
    if d == 0:
        npos = (pos[1], op[pos[0]], pos[2])
    elif d == 1:
        npos = (pos[2], pos[1], op[pos[0]])
    elif d == 2:
        npos = (op[pos[1]], pos[0], pos[2])
    else:
        npos = (op[pos[2]], pos[1], pos[0])
    return npos

def checkGold(nrc, npos, g, gm):
    if nrc in gm:
        if not verBit(g, npos[0]):
            g = swap(g, npos[0])
            gm = gm.copy()
            gm.remove(nrc)
    else:
        if verBit(g, npos[0]):
            g = swap(g, npos[0])
            gm = gm.copy()
            gm.add(nrc)
    return (g, gm)

def dijkstra(rc, gold, R, C, A, B):
    #estado: ((row, col), (floor, north, east), g, goldMap)
    
    act = (0, rc, (0, 1, 4), 0, gold)
    vis = {(rc, (0, 1, 4), 0, frozenset(gold)): 0}
    flag = False
    cost = 0
    q = []
    heappush(q, act)

    while not flag and len(q) > 0:
      c, rc, pos, g, gm = heappop(q)
      i = 0
      while not flag and i < 4 and vis[(rc, pos, g, frozenset(gm))] == c:
        nflag, nrc = move(i, rc, R, C)
        ac = A
            
        if nflag:
            npos = roll(i, nrc, pos)
            ng, ngm = checkGold(nrc, npos, g, gm)
            fngm = frozenset(ngm)
            if ng > g:
                ac = B
            ns = (nrc, npos, ng, fngm)
            cost = c + ac
            
            if ns not in vis or vis[ns] > cost:
                #63 = 111111
                
                flag = ng == 63
                if not flag:
                    vis[ns] = cost
                    heappush(q, (cost, nrc, npos, ng, ngm))
                    
        i += 1
    return (flag, cost)
        

def main():
    T = int(stdin.readline())
    for i in range(T):
        R, C, A, B = list(map(int, stdin.readline().split()))
        gold = set()
        rc = (-1, -1)
        flag = False
        
        for j in range(R):
            aux = stdin.readline()
            k = 0
            while not flag and k < C:
                if aux[k] == 'G':
                    gold.add((j, k))
                elif aux[k] == 'S':
                    rc = (j, k)
                flag = len(gold) == 6 and rc != (-1, -1)
                k += 1
                
        flag, cost = dijkstra(rc, gold, R, C, A, B)
        
        if flag:
            print("Screw you guys, I got all the gold for", cost, "cost!")
        else:
            print("Oh my God, they killed Kenny!")        

main()
