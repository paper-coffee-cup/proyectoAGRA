"""
Tarea 1
A - Problem A

Nombre: Isabel Sofía Adrada Arboleda, Juan Luis Guevara Bustamante
Código de estudiante: 8977586,

"""

#Unificación de posición y caras en un solo número

from sys import stdin
from heapq import heappop, heappush

def verBit(n, pos, bits):
    return bool(n & (1 << bits - pos))

def swap(n, pos, bits):
    return n ^ (1 << bits - pos)

def fn(r, c, C):
    return r * C + c

def f(k, C):
    r = floor(k/C)
    c = k - r * C
    return (r, c)

def move(d, rc, R, C):
    #0 = north, 1 = east, 2 = south, 3 = west
    dire = ((-1, 0), (0, 1), (1, 0), (0, -1))

    rc += dire[0] * 10000 + dire[1] * 1000    
    flag = rc > 9999 and rc < 10000 * R and  rc - (rc // (10 ** 4)) * 10000 < R * 10000 and nrc[1] >= 0 and nrc[1] < C
    return (flag, nrc)

def move2(d, rc, R, C):
    #0 = north, 1 = east, 2 = south, 3 = west
    dire = ((-1, 0), (0, 1), (1, 0), (0, -1))
    
    nrc = (rc[0] + dire[d][0], rc[1] + dire[d][1])
    if d == 0:
        flag = nrc[0] >= 0
    elif d == 1:
        flag = nrc[1] < C
    elif d == 2:
        flag = nrc[0] < R
    else:
        flag = nrc[1] >= 0
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

def checkGold(nrc, npos, g, gm, R, C):
    flag = verBit(g, npos[0], 5)
    if verBit(gm, fn(nrc[0], nrc[1], C), R * C - 1):
        if not flag:
            g = swap(g, npos[0], 5)
            gm = swap(gm, fn(nrc[0], nrc[1], C), R * C - 1)
    else:
        if flag:
            g = swap(g, npos[0], 5)
            gm = swap(gm, fn(nrc[0], nrc[1], C), R * C - 1)
    return (g, gm)

def dijkstra(rc, gold, R, C, A, B):
    #estado: (row * 10000 + col * 1000 + floor * 100 + north * 10 + east, g, goldMap)
    
    act = (0, rc, 0, gold)
    vis = {(rc, 0, gold): 0}
    flag = False
    cost = 0
    q = []
    heappush(q, act)

    while not flag and len(q) != 0:
      c, rc, g, gm = heappop(q)
      i = 0
      
      while not flag and i < 4 and vis[(rc, g, gm)] == c:
        nflag, nrc = move(i, rc, R, C)
        ac = A
            
        if nflag:
            npos = roll(i, nrc, pos)
            ng, ngm = checkGold(nrc, npos, g, gm, R, C)
            if ng > g:
                ac = B
            ns = (nrc, npos, ng, ngm)
            cost = c + ac
            
            if ns not in vis or vis[ns] > cost:
                #63 = 111111

                if ng == 63:
                    flag = True
                else:
                    vis[ns] = cost
                    heappush(q, (cost, nrc, npos, ng, ngm))  
        i += 1
        
    return (flag, cost)
        

def main():
    T = int(stdin.readline())
    for i in range(T):
        R, C, A, B = list(map(int, stdin.readline().split()))
        flag = False
        gold = 0
        gCount = 0
        rc = 14
        
        for j in range(R):
            aux = stdin.readline()
            k = 0
            while not flag and k < C:
                if aux[k] == 'G':
                    gold = swap(gold, fn(j, k, C), R * C - 1)
                    gCount += 1
                elif aux[k] == 'S':
                    rc += j * 10000 +  k * 1000
                flag = gCount == 6 and rc != 14
                k += 1

        flag, cost = dijkstra(rc, gold, R, C, A, B)
        
        if flag:
            print("Screw you guys, I got all the gold for", cost, "cost!")
        else:
            print("Oh my God, they killed Kenny!")        

main()
