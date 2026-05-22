"""
Tarea 1
A - Problem A

Nombre: Isabel Sofía Adrada Arboleda, Juan Luis Guevara Bustamante
Código de estudiante: 8977586,

"""

#implementación con una máscara de bits para el mapa completo

from sys import stdin
from heapq import heappop, heappush

def verBit(n, pos, bits):
    return bool(n & (1 << bits - pos))

def swap(n, pos, bits):
    return n ^ (1 << bits - pos)

def assign(n, flag, pos, bits):
    if flag:
        ans = n | (1 << bits - pos)
    else:
        ans = n & ~(1 << bits - pos)
    return ans

def fn(r, c, C):
    return r * C + c

def f(k, C):
    r = floor(k/C)
    c = k - r * C
    return (r, c)

def move(d, rc, R, C):
    #0 = north, 1 = east, 2 = south, 3 = west
    dire = ((-1, 0), (0, 1), (1, 0), (0, -1))
    
    nrc = (rc[0] + dire[d][0], rc[1] + dire[d][1])
    flag = nrc[0] >= 0 and nrc[0] < R and nrc[1] >= 0 and nrc[1] < C
    return (flag, nrc)

def roll(d, c):
    #0 = north, 1 = east, 2 = south, 3 = west
    G = ((3, 5, 1, 4), (0, 1, 2, 1), (1, 4, 3, 5), (2, 3, 0, 3), (4, 0, 4, 2), (5, 2, 5, 0))
    #0 = vertical, 1 = horizontal
    move = ((0, 0), (1, 2), (2, 4), (3, 5))

    nc = c
    m = d % 2
    for i in move:
        pos = i[m]
        nc = assign(nc, verBit(c, G[pos][d], 5), pos, 5)

    return nc

def checkGold(nrc, c, gm, R, C):
    #0 = cara de abajo
    ans = False
    flag = verBit(c, 0, 5)
    if verBit(gm, fn(nrc[0], nrc[1], C), R * C - 1):
        if not flag:
            c = swap(c, 0, 5)
            gm = swap(gm, fn(nrc[0], nrc[1], C), R * C - 1)
            ans = True
    else:
        if flag:
            c = swap(c, 0, 5)
            gm = swap(gm, fn(nrc[0], nrc[1], C), R * C - 1)
    return (ans, c, gm)

def dijkstra(rc, gold, R, C, A, B):
    #estado: ((row, col), cube, goldMap)
    
    act = (0, rc, 0, gold)
    vis = {(rc, 0, gold): 0}
    flag = False
    cost = 0
    q = []
    heappush(q, act)

    while not flag and len(q) != 0:
      co, rc, c, gm = heappop(q)
      i = 0
      while not flag and i < 4 and vis[(rc, c, gm)] == co:
        nflag, nrc = move(i, rc, R, C)
        ac = A
            
        if nflag:
            nc = roll(i, c)
            gflag, nc, ngm = checkGold(nrc, nc, gm, R, C)
            if gflag:
                ac = B
            ns = (nrc, nc, ngm)
            cost = co + ac
            
            if ns not in vis or vis[ns] > cost:
                #63 = 111111
                if nc == 63:
                    flag = True
                else:
                    vis[ns] = cost
                    heappush(q, (cost, nrc, nc, ngm))  
        i += 1
        
    return (flag, cost)
        

def main():
    T = int(stdin.readline())
    for i in range(T):
        R, C, A, B = list(map(int, stdin.readline().split()))
        gold = 0
        gCount = 0
        rc = (-1, -1)
        flag = False
        
        for j in range(R):
            aux = stdin.readline()
            k = 0
            while not flag and k < C:
                if aux[k] == 'G':
                    gold = swap(gold, fn(j, k, C), R * C - 1)
                    gCount += 1
                elif aux[k] == 'S':
                    rc = (j, k)
                flag = gCount == 6 and rc != (-1, -1)
                k += 1

        flag, cost = dijkstra(rc, gold, R, C, A, B)
        
        if flag:
            print("Screw you guys, I got all the gold for", cost, "cost!")
        else:
            print("Oh my God, they killed Kenny!")        

main()
