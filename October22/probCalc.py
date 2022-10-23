from random import shuffle

# ---- Calculate Answer Exactly ------
prob = ( (12/13) * (11/12) * (10/11) * (9/10) * (8/9)
       * (12/13) * (11/12) * (10/11) * (9/10)
       * (12/13) * (11/12) * (10/11)
       * (12/13)
       ) # We can clearly see this reduces to (8/13 * 9/13 * 10/13 * 12/13).
       #   Initially I made a mistake in the calculation and it didn't match the Monte Carlo Method - caught it phew
 
print("By Exact Method, " + str(prob))


# ----- Calculate via Monte Carlo -----
cards = [(num,suit) for num in range(1,14) for suit in range(0,4)]

NUM_TRIALS = 1000000

nShape = 0
shapeNNoAce = 0
for t in range(NUM_TRIALS):
    if (t % ((NUM_TRIALS)/10) == 0):
        print(f"{int(t/(NUM_TRIALS/10))}/10")
    shuffle(cards)
    hand = cards[:13]
    shape = [0,0,0,0]
    hasAce = False
    for (num,suit) in hand:
        shape[suit] += 1
        if num == 1:
            hasAce = True

    shape.sort()
    if shape == [1,3,4,5]:
        if not(hasAce):
            shapeNNoAce += 1
        nShape += 1

print(f"By Monte Carlo Method: {shapeNNoAce}/{nShape}")
print(f"...which equals {shapeNNoAce/nShape}")

