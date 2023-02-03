MAXN = 10000000
n = 0

def solve(a,b,c,d):
    if (a == b and b == c and c == d and d == 0):
        return 1
    else:
        return 1+solve(abs(a-b),abs(b-c),abs(c-d),abs(d-a))

def iterate(a,b,c,d):
    global n
    a1 = abs(a-b)
    b1 = abs(b-c)
    c1 = abs(c-d)
    d1 = abs(d-a)

    if (n % 3 == 0):
        a1 *= 2
        b1 *= 2
        c1 *= 2
        d1 *= 2
    n += 1

    for a2 in range(a1+1, MAXN):
        b2 = a2 - a1
        c2 = b2 + b1
        d2 = c2 + c1
        # print(a2, b2, c2, d2)
        if (a2 == d2 + d1):
            a3 = a2
            b3 = 0
            c3 = b2
            d3 = d1
            if (d3 - c3 == c2): 
                return (a3,b3,c3,d3)

if __name__ == "__main__":
    curr = (13, 0, 2, 6)

    while curr != None:
        curr = iterate(*curr)
        print(curr)

