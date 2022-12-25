grid = [
    [57,33,132,268,492,732],
    [81,123,240,443,353,508],
    [186,42,195,704,452,228],
    [-7,2,357,452,317,395],
    [5,23,-4,592,445,620],
    [0,77,32,403,337,452]
]

class Face:
    def __init__(self):
        self.value = None
        self.neighbours = []
        self.topPtr = 0
        self.leftPtr = 1
        self.opposite = None
    
    def tipUp(self):
        return self.neighbours[(self.topPtr+2)%4]
    def tipDown(self):
        return self.neighbours[self.topPtr]
    def tipRight(self):
        return self.neighbours[self.leftPtr]
    def tipLeft(self):
        return self.neighbours[(self.leftPtr+2)%4]


class Die:
    def __init__(self):
        self.faces = [Face() for i in range(6)]
        self.faceUp = self.faces[0]

        for i in range(6):
            for j in range(3):
                if not(i == j or 5 - i == j):
                    self.adj(i,j)
            
            for j in range(3):
                if not(i == j or 5 - i == j):
                    self.adj(i,5-j)
            
            self.faces[i].opposite = self.faces[5 - i]

    def adj(self, a, b):
        self.faces[a].neighbours.append(self.faces[b])
    
    def tipLeft(self): # return the top face's value
        oldUp = self.faceUp
        self.faceUp = self.faceUp.tipLeft()
        self.faceUp.topPtr = self.faceUp.neighbours.index(oldUp.neighbours[oldUp.topPtr])
        self.faceUp.leftPtr = self.faceUp.neighbours.index(oldUp)

    def tipRight(self):
        oldUp = self.faceUp
        self.faceUp = self.faceUp.tipRight()
        self.faceUp.topPtr = self.faceUp.neighbours.index(oldUp.neighbours[oldUp.topPtr])
        self.faceUp.leftPtr = self.faceUp.neighbours.index(oldUp.opposite)

    def tipUp(self):
        oldUp = self.faceUp
        self.faceUp = self.faceUp.tipUp()
        self.faceUp.topPtr = self.faceUp.neighbours.index(oldUp)
        self.faceUp.leftPtr = self.faceUp.neighbours.index(oldUp.neighbours[oldUp.leftPtr])

    def tipDown(self):
        oldUp = self.faceUp
        self.faceUp = self.faceUp.tipDown()
        self.faceUp.topPtr = self.faceUp.neighbours.index(oldUp.opposite)
        self.faceUp.leftPtr = self.faceUp.neighbours.index(oldUp.neighbours[oldUp.leftPtr])

# allow negative numbers on the die

def doDirection(row, col, N, score, dr, dc, tipToCall, untip):
    global visited
    global theDie

    if col + dc >= 0 and col + dc < len(grid[0]) and row + dr >= 0 and row + dr < len(grid):
        tipToCall()
        print(theDie.faceUp.value)
        print(*visited,sep='\n',end='\n\n')
        print("Here" + tipToCall.__repr__()) 
        print(theDie.faceUp.value)
        print(f"The values on the die are: {list(map(lambda it: it.value,theDie.faces))}.")


        wasUnset = False
        if theDie.faceUp.value == None and (grid[row+dr][col+dc] - score) % N == 0:
            theDie.faceUp.value = (grid[row+dr][col+dc] - score) // N
            wasUnset = True
            
        if theDie.faceUp.value != None and N * theDie.faceUp.value + score == grid[row+dr][col+dc]:
            if visited[row+dr][col+dc] != -1:
                visited[row+dr][col+dc] = str(visited[row+dr][col+dc]) + "," + str(N)
            else: 
                visited[row+dr][col+dc] = N
                
            if findPath(row+dr,col+dc,N+1):
                return True
            else:
                visited[row+dr][col+dc] = -1 # not on the path any more
                if (wasUnset):
                    theDie.faceUp.value = None
                print("untip")
                untip()
                return False
        else:
            untip()
            return False

    return False


def findPath(row,col,N):
    global theDie


    if (row == 0 and col == len(grid[0])-1):
        return True

    score = grid[row][col]
    return doDirection(row, col, N, score, 0, -1, theDie.tipLeft, theDie.tipRight) or doDirection(row, col, N, score, 0, 1, theDie.tipRight,theDie.tipLeft) or doDirection(row, col, N, score, 1, 0, theDie.tipDown,theDie.tipUp) or doDirection(row, col, N, score, -1, 0, theDie.tipUp,theDie.tipDown)


def score():
    scoreOut = 0
    for r in range(len(grid)):
        for c in range(len(grid[0])):
            if visited[r][c] == -1:
                scoreOut += grid[r][c]
    return scoreOut

visited = list(map(lambda l : list(map(lambda _ : -1, l)),grid))
theDie = Die()

visited[len(grid)-1][0] = 0
findPath(len(grid)-1,0,1)
print(f"The values on the die are: {list(map(lambda it: it.value,theDie.faces))}.")
print(visited)
print(f"The die score is: {score()}.")



# to try:
    # check you are still in the grid
    # check if the new upper face exists
    # if it does
    #   check if this causes failure on N - if yes then this case fails and we need to try the other directions
    #   if no failure: continue()
    # if it does not
    #   set it so that it works for this location in the grid and continue()

# to continue
    # set the current square as visited
    # recurse on the new row and col position with N + 1
    # if the recursive call returns True (i.e. success)
    #   return True and break out; don't unset the visited
    # if it does not:
    #   unset the current square as visited
    #   keep trying the other neighbours

# try and tip left
# try and tip right
# try and tip up
# try and tip down