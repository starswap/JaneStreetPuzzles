// What we have here is some code which can:
// - manage the grid and solve individual sub-24/7s at particular offsets
// - verify that the starting and ending numbers are correct
// - calculate the answer in terms of the product of orthogonally adjacent empty blocks.
// However the code doesn't check the sum conditions and furthermore it will only solve one grid at a time.
// It also needs a helping hand to get started with the recursive backtracking.
// I solved the problem in conjunction with the computer by:
// 1. Making observations from the grid about some extra numbers we can put in based on the sum conditions (which tell us about the middle two squares in each row - key observation)
// 2. Repeatedly:
//    3. Getting the computer to solve a sub-puzzle (starting at the bottom left and always proceeding with the puzzle containing the most numbers)
//    4. Checking the solution on paper (in particular the sum conditions)
//    5. Adding any other numbers I could infer given the solution
//    6. Updating the input grid in this code, using -1 to represent a square which is known to have to contain a zero from a previous puzzle solve.
// 7. Grabbing the final numerical answer at the end.

#include <bits/stdc++.h>
using namespace std;


// vector<int> compoundColSums = {0, 36, 30, 34, 27, 0, 40, 27, 0, 0, 0, 0, 0};
// vector<int> compoundRowSums = {0, 0, 0, 33, 29, 0, 40, 28, 0, 0, 36, 0};

vector<int> firstInRow = {5, 7, 7, 0, 0, 2, 0, 0, 0, 0, 0, 0};
vector<int> firstInCol = {6, 0, 0, 0, 0, 3, 0, 0, 0, 0, 7, 0};
vector<int> lastInRow = {4, 0, 0, 1, 0, 0, 0, 0, 0, 0, 0, 7};
vector<int> lastInCol = {6, 0, 0, 4, 0, 0, 0, 0, 0, 0, 0, 5};


// Neighbour check; non-diagonal
int dr[] = {1, 0, 0,-1};
int dc[] = {0, 1,-1, 0};

// Print out the puzzle
ostream& operator << (ostream & o, vector<vector<int>> v) {
    // Top:
    o << "#";
    for (int i = 0; i < 2 * v.size() - 1; ++i) {
        o << "-";
    }
    o << "#" << '\n';

    // Numbers:
    for (int j = 0; j < v.size(); j ++) {
        o << "|" << v[j][0];
        for (int i = 1; i < v[j].size(); ++i) {
            o << " " << v[j][i];
        }
        o << "|" << "\n";
    }

    // Bottom:
    o << "#";
    for (int i = 0; i < 2 * v.size() - 1; ++i) {
        o << "-";
    }
    o << "#" << '\n';

    return o;
}

// Print out other vectors of integers
ostream& operator << (ostream & o, vector<int> v) {
    o << "[";
    for (int i = 0; i < v.size(); ++i) {
        o << v[i] << ",";
    }
    o << "]";
    return o;
}

// Bounds check
bool inGrid(vector<vector<int>> &grid, int r, int c) {
    return r >= 0 && r < 7 && c >= 0 && c < 7;
}

// Bounds check and value non zero check
bool exists(vector<vector<int>> &grid, int r, int c, int rd, int cd) {
    return inGrid(grid, r, c) && grid[r + rd][c + cd] > 0;
}

// Flood fill all the non zero elements back to 0 - to see if all non-zero
//   elements are connected
void floodfill(vector<vector<int>> &grid, int r, int c, int rd, int cd) {
    grid[r + rd][c + cd] = 0;
    for (int i = 0; i < 4; ++i) {
        if (exists(grid, r + dr[i], c + dc[i], rd, cd)) {
            floodfill(grid, r + dr[i], c + dc[i], rd, cd);
        }
    }
}

// Check solution meets the contiguity constraint
bool contigCheck(vector<vector<int>> grid, int rd, int cd) {
    // We search through the grid for the first non-zero element
    //    then flood fill from there. After that point, we shouldn't find any 
    //    more non-zero elements, iff the solution is correct.
    bool doneOne = false;
    for (int r = 0; r < 7; ++r) {
        for (int c = 0; c < 7; ++c) {
            if (grid[r + rd][c + cd] > 0 && !doneOne) {
                floodfill(grid, r, c, rd, cd);
                doneOne = true;
            }
            else if (grid[r + rd][c + cd] > 0 && doneOne) {
                return false;
            }
        }
    }
    return true;
}

bool firstLastCheck(vector<vector<int>> grid, int rd, int cd) {
    int p;
    if (rd == 0) {
        for (int c = 0; c < 7; ++c) {
            if (firstInCol[cd + c] != 0) {
                for (p = 0; p < 7, grid[rd + p][cd + c] == 0; p++) {}
                if (!(p == 7 || grid[rd + p][cd + c] == firstInCol[cd + c])) return false;
            }
        }
    }
    else {
        for (int c = 0; c < 7; ++c) {
            if (lastInCol[cd + c] != 0) {
                for (p = 6; p >= 0, grid[rd + p][cd + c] == 0; p--) {}
                if (!(p == -1 || grid[rd + p][cd + c] == lastInCol[cd + c])) return false;
            }
        }
    }

    if (cd == 0) {
        for (int r = 0; r < 7; ++r) {
            if (firstInRow[rd + r] != 0) {
                for (p = 0; p < 7, grid[rd + r][cd + p] == 0; p++) {}
                if (!(p == 7 || grid[rd + r][cd + p] == firstInRow[r + rd])) return false;
            }
        }
    }
    else {
        for (int r = 0; r < 7; ++r) {
            if (lastInRow[rd + r] != 0) {
                for (p = 6; p >= 0, grid[rd + r][cd + p] == 0; p--) {}
                if (!(p == -1 || grid[rd + r][cd + p] == lastInRow[r + rd])) return false;
            }
        }
    }
    
    return true;
}

int placed = 0;

// Recursive helper to solve 24-7 puzzle
bool solveRecursive(vector<vector<int>> &grid, vector<int>& canPlace, int r, int c, int maxAvail, int rd, int cd) {

    // cout << placed << endl;
    // cout << grid << endl;

    // Check if we have no numbers left to place
    bool noNumbersLeft = true;
    for (int i = 0; i < 7; ++i) {
        if (canPlace[i] > 0) {
            noNumbersLeft = false; break;
        }
    }

    if (noNumbersLeft) {
        return contigCheck(grid, rd, cd) && firstLastCheck(grid, rd, cd); // either we have a contiguous solution (success!) or not (failure)
    }

    for (; r < 7; ++r) { // reuse existing r from the recursive call
        for (; c < 7; ++c) { // reuse existing c from the recursive call
            bool squareOk = ! (
                (exists(grid, r, c-1, rd, cd) && exists(grid, r-1, c-1, rd, cd) && exists(grid, r-1, c, rd, cd)) ||
                (exists(grid, r, c+1, rd, cd) && exists(grid, r+1, c+1, rd, cd) && exists(grid, r+1, c, rd, cd)) ||
                (exists(grid, r, c-1, rd, cd) && exists(grid, r+1, c-1, rd, cd) && exists(grid, r+1, c, rd, cd)) ||
                (exists(grid, r, c+1, rd, cd) && exists(grid, r-1, c+1, rd, cd) && exists(grid, r-1, c, rd, cd))
            );

            if (grid[r + rd][c + cd] == 0 && squareOk) { // no issues with 2x2 squares, and the current square is empty. (could check current square empty first)
                for (int i = 0; i < 7; ++i) {
                    if (canPlace[i] > 0) {
                        // try to place i + 1
                        int cCount = 0, cSum = 0, rCount = 0, rSum = 0;
                        
                        for (int p = 0; p < 7; p++) {
                            if (grid[p+rd][c+cd] > 0) {
                                cSum += grid[p + rd][c + cd];
                                cCount++;
                            }
                            if (grid[r + rd][p + cd] > 0) {
                                rSum += grid[r + rd][p + cd];
                                rCount++;
                            }
                        }
                        
                        // Assuming we were to place i + 1 here.
                        cSum += i + 1; rSum += i + 1;
                        cCount++; rCount++;

                        // Check 4 and 20 constraints are not hopeless.
                        if (cCount <= 4 && rCount <= 4 && cSum <= 20 && rSum <= 20 && (20 - rSum <= (4 - rCount) * (maxAvail + 1)) && (20 - cSum <= (4 - cCount) * (maxAvail + 1))) {
                            grid[r + rd][c + cd] = i + 1; // do placement
                            canPlace[i]--; // reduce amount available
                            int oma = maxAvail; // save for later
                            while (canPlace[maxAvail] == 0) maxAvail--;
                            placed += 1;
                            
                            if (solveRecursive(grid, canPlace, r, c, maxAvail, rd, cd)) return true;
                            
                            // reset if we failed, so we can try again
                            placed -= 1;
                            maxAvail = oma;
                            canPlace[i]++;
                            grid[r + rd][c + cd] = 0;
                        }
                        
                    }
                }
            }
        }

        // We are about to move on from this row, never to come back.
        // If we didn't place enough numbers here to meet the 4 and 20 constraint, we should give up.
        int sum = 0;
        int count = 0;
        for (int c = 0; c < 7; ++c) {
            if (grid[r + rd][c + cd] > 0) {
                sum += grid[r + rd][c + cd];
                count++;
            } 
        }
        if (count < 4 || count == 4 && sum != 20) {return false;}
    
        // // Similarly do the 1st and last checks for this row.
        // if (firstInRow[rd + r] != 0) {
        // for (int c = 0; c < 7, grid[rd + r][cd + c] == 0; c++) {}
        //     if (!(c == 7 || grid[rd + r][cd + c] == firstInRow[r + rd])) return false;
        // }

        // if (lastInRow[rd + r] != 0) {
        //     for (int c = 6; c >= 0, grid[rd + r][cd + c] == 0; c--) {}
        //     if (!(c == -1 || grid[rd + r][cd + c] == lastInRow[r + rd])) return false;
        // }
     
        c = 0;
    }
    return false;
}


// might be non contig
// middle 2 sum.


bool solve247(vector<vector<int>>& grid, int rd, int cd) {
    vector<int> canPlace(7, 0);
    for (int i = 0; i < 7; ++i) {
        canPlace[i] = i + 1;
    }
    for (int r = 0; r < 7; ++r) {
        for (int c = 0; c < 7; ++c) {
            if (grid[rd+r][cd+c] > 0) {
                canPlace[grid[rd+r][cd+c] - 1]--;
            }
        }
    }

    int maxi;
    for (maxi = 7; maxi > 0, canPlace[maxi-1] == 0; maxi--);

    return solveRecursive(grid, canPlace, 0, 0, maxi, rd, cd);
}

  vector<vector<int>> grid = {
        {0, 5, 0, 6, 0, 3, 6, 0, 0, 0, 7, 4},
        {0, 7, 7, 1, 0, -1, 5, 0, 4, 6, 5, 0},
        {0, 0, 0, 7, 5, 3, 5, 0, 6, 0, 6, 0},
        {6, 4, 3, 0, 0, 7, -1, 5, 7, 1, 0, 0},
        {7, 0, 0, 0, 2, 7, 4, 2, 0, 0, 0, 7},
        {2, -1, 6, 6, 6, -1, -1, 6, 3, 7, -1, 4},
        {5, 4, 4, -1, 7, -1, -1, 7, -1, 6, 2, 5},
        {7, 0, 0, 0, 1, 5, 7, 0, 0, 0, 0, 0},
        {0, 5, 3, 7, 0, 5, -1, 0, 0, 0, 0, 0},
        {6, 5, 0, 0, 0, 7, 2, 0, 0, 0, 0, 0},
        {0, 6, 7, 3, 0, -1, 4, 0, 0, 0, 0, 0},
        {0, 0, 0, 4, 6, 3, 7, 0, 0, 0, 0, 0}
    };


int regionScore(int i, int j) {
    if (grid[i][j] > 0) return 0;
    else {
        int total = 1;
        grid[i][j] = 100;
        for (int d = 0; d < 4; ++d) {
            if (i + dr[d] >= 0 && i + dr[d] < 12 && j + dc[d] >= 0 && j + dc[d] < 12)
                total += regionScore(i + dr[d], j + dc[d]);
        }
        return total;
    }
}

int main() {

  


    //     vector<vector<int>> grid = {
    //     {0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0},
    //     {0, 0, 0, 0, 1, 0, 0, 0, 0, 6, 5, 0},
    //     {0, 0, 0, 0, 0, 3, 0, 0, 0, 0, 6, 0},
    //     {0, 4, 0, 0, 0, 0, 0, 0, 7, 0, 0, 0},
    //     {0, 0, 0, 0, 2, 0, 0, 0, 0, 0, 0, 7},
    //     {2, 0, 6, 0, 0, 0, -1, 0, 3, 7, 0, 0},
    //     {0, 0, 4, 0, 0, 0, -1, 0, 0, 0, 0, 0},
    //     {0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0},
    //     {0, 0, 0, 7, 0, 5, 0, 0, 0, 0, 0, 0},
    //     {0, 5, 0, 0, 0, 7, 0, 0, 0, 0, 0, 0},
    //     {0, 6, 7, 0, 0, 0, 0, 0, 0, 0, 0, 0},
    //     {0, 0, 0, 0, 6, 0, 0, 0, 0, 0, 0, 0}
    // };


    // can we encode forced 0s?

    // vector<vector<int>> grid = {
    //     {10, 14, 10, 10, 10, 10, 10},
    //     {10, 0, 4, 0, 0, 0, 0, 0},
    //     {10, 0, 0, 6, 3, 0, 0, 6},
    //     {10, 0, 0, 0, 0, 0, 5, 5},
    //     {10, 0, 0, 0, 4, 0, 0, 0},
    //     {10, 4, 7, 0, 0, 0, 0, 0},
    //     {10, 2, 0, 0, 7, 4, 0, 0},
    //     {10, 0, 0, 0, 0, 0, 1, 0}
    // };

    cout << "Starting to Solve:" << endl;
    bool couldSolve = solve247(grid, 5, 5);
    cout << (couldSolve ? "SOLVED" : "FAILED TO SOLVE") << endl;
    if (couldSolve) {
        cout << grid << endl;
    }

    long long score = 1;
    for (int i = 0; i < 12; ++i) {
        for (int j = 0;j < 12;++j) {
            if (grid[i][j] <= 0) {
                score *= regionScore(i, j);
            }
        }
    }

    cout << score << endl;

    cout << grid << endl;
    return 0;
}


// Whole thing needs to be contiguous
