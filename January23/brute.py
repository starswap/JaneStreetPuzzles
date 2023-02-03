MAXN = 10000000

def solve(a,b,c,d):
    if (a == b and b == c and c == d and d == 0):
        return 1
    else:
        return 1+solve(abs(a-b),abs(b-c),abs(c-d),abs(d-a))

def printOut(a,b,c,d):
    if a == b and b == c and c == d and d == 0:
        print((0,0,0,0))
    else:
        print(f"({a},{b},{c},{d})", end = " -> ")
        printOut(abs(a-b),abs(b-c),abs(c-d),abs(d-a))

if __name__ == "__main__":
    bestScore = 0
    bests = []
    prevBest = -1
    for n in range(1,700):
        i = n
        j = 0
        # for j in range(i+1):
        for k in range(i+1):
            for l in range(i+1):
                curr = solve(i,j,k,l)
                if (curr > bestScore):
                    bests = [(i,j,k,l)]
                    bestScore = curr
                    printOut(i,j,k,l)
                elif (curr == bestScore):
                    bests.append((i,j,k,l))

        # print(f"With max n = {n}, the best score is {bestScore}")
        if bestScore != prevBest:
            print(f"{n,bestScore}")
            print(f"This can be achieved with {bests}")
            print(f"The best sum is = {min(map(sum, bests))}")
        prevBest = bestScore
