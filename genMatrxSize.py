import math

for f in range(1, 9):
    print("{", sep="", end="")
    
    for c in range(1, 9):
        N = f * c
        total_mascaras = sum(math.comb(N, k) for k in range(0, min(6, N) + 1))
        
        print(total_mascaras, sep="", end="")
        
        if c < 8:
            print(", ", sep="", end="")
            
    print("}", sep="", end="")
    
    if f < 8:
        print(", ", sep="", end="")
print()
