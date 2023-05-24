#include <bits/stdc++.h>
using namespace std;

int opCount = 0;
int dr[] = {1, 0, 0,-1};
int dc[] = {0, 1,-1, 0};

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

ostream& operator << (ostream & o, vector<int> v) {
    o << "[";
    for (int i = 0; i < v.size(); ++i) {
        o << v[i] << ",";
    }
    o << "]";
    return o;
}

bool inGrid(vector<vector<int>> &grid, int r, int c) {
    return r >= 0 && r < grid.size() && c >= 0 && c < grid[0].size();
}

bool exists(vector<vector<int>> &grid, int r, int c) {
    return inGrid(grid, r, c) && grid[r][c] != 0;
}

void floodfill(vector<vector<int>> &grid, int r, int c) {
    grid[r][c] = 0;
    for (int i = 0; i < 4; ++i) {
        if (exists(grid, r + dr[i], c + dc[i])) {
            floodfill(grid, r + dr[i], c + dc[i]);
        }
    }
}

bool floodcheck(vector<vector<int>> grid) {
    bool doneOne = false;
    for (int r = 0; r < 7; ++r) {
        for (int c = 0; c < 7; ++c) {
            if (grid[r][c] != 0 && !doneOne) {
                floodfill(grid, r, c);
                doneOne = true;
            }
            else if (grid[r][c] != 0 && doneOne) {
                return false;
            }
        }
    }
    return true;
}

bool solveRecursive(vector<vector<int>> &grid, vector<int>& canPlace, int r, int c, int maxAvail) {
    opCount++;
    // if (grid[0][2] == 3 && grid[2][0] == 0 && grid[2][1] == 0 && grid[2][2] == 5 && grid[3][0] == 0) {cout << grid << endl; cout << canPlace << endl;} //opCount % 100000 == 0 
    int oldR = r;
    int oldC = c;
    // cout << grid << endl;
    
    bool done = true;
    for (int i = 0; i < 7; ++i) {
        if (canPlace[i] > 0) {
            done = false;
        }
    }

    if (done) {
        // if (grid[0][2] == 3 && grid[2][0] == 0 && grid[2][1] == 0 && grid[2][2] == 5 && grid[3][0] == 0) {
            // cout << grid << endl;
            // cout << canPlace << endl;
        // }
        return floodcheck(grid);
    }

    while (true) {
        while (c < 7) {
            bool squareOk = ! (
                (exists(grid, r, c-1) && exists(grid, r-1, c-1) && exists(grid, r-1, c)) ||
                (exists(grid, r, c+1) && exists(grid, r-1, c+1) && exists(grid, r-1, c)) ||
                (exists(grid, r, c-1) && exists(grid, r+1, c-1) && exists(grid, r+1, c)) ||
                (exists(grid, r, c+1) && exists(grid, r+1, c+1) && exists(grid, r+1, c))
            );

            if (grid[r][c] == 0 //&& (exists(grid, r, c-1) || exists(grid, r- 1, c) || exists(grid, r + 1, c) || exists(grid, r, c + 1)) 
                && squareOk) { // empty and contiguous and contains free in 2x2
                for (int i = 6; i >= 0; --i) {
                    if (canPlace[i] > 0) {
                        
                        // try to place i + 1
                        int cCount = 0;
                        int cSum = 0;
                        int rCount = 0;
                        int rSum = 0;
                        for (int p = 0; p < grid.size(); p++) {
                            cSum += grid[p][c];
                            cCount += grid[p][c] > 0;
                            rSum += grid[r][p];
                            rCount += grid[r][p] > 0;
                        }
                        cSum += i + 1;
                        rSum += i + 1;
                        cCount++;
                        rCount++;

                        // if (grid[0][2] == 3 && grid[2][0] == 0 && grid[2][1] == 0 && grid[2][2] == 5 && grid[3][0] == 0 && r == 3 && c == 1 && i == 2) {
                        //     cout << rSum << " " << rCount << " " << cSum << " " << cCount << endl;
                        //     cout << maxAvail << endl;
                        // }

                        if (cCount <= 4 && rCount <= 4 && cSum <= 20 && rSum <= 20 && (20 - rSum <= (4 - rCount) * (maxAvail + 1)) && (20 - cSum <= (4 - cCount) * (maxAvail + 1))) {
                            grid[r][c] = i + 1;
                            canPlace[i]--;
                            int oma = maxAvail;
                            while (canPlace[maxAvail] == 0) maxAvail--;
                            if (solveRecursive(grid, canPlace, r, c, maxAvail)) return true;
                            maxAvail = oma;
                            canPlace[i]++;
                            grid[r][c] = 0;
                        }
                        
                    }
                }
            }
            c++;
            if (r == 6 && c == 7) {
                return false;
            }
        }
        int sum = 0;
        int count = 0;
        for (int c = 0; c < 7; ++c) {
            sum += grid[r][c];
            count += grid[r][c] > 0;
        }
        if (count < 4 || count == 4 && sum != 20) {return false;}
        c = 0;
        r++;
    }
    return false;
}

bool solve247(vector<vector<int>>& grid) {
    vector<int> canPlace(7, 0);
    for (int i = 0; i < 7; ++i) {
        canPlace[i] = i + 1;
    }
    for (int r = 0; r < 7; ++r) {
        for (int c = 0; c < 7; ++c) {
            if (grid[r][c] != 0) {
                canPlace[grid[r][c] - 1]--;
            }
        }
    }
    // cout << "a" << canPlace << endl; 
    int maxi;
    for (maxi = 7; maxi > 0, canPlace[maxi-1] == 0; maxi--);

    return solveRecursive(grid, canPlace, 0, 0, maxi);
}

int main() {
    vector<vector<int>> grid = {
        {0, 4, 0, 0, 0, 0, 0},
        {0, 0, 6, 3, 0, 0, 6},
        {0, 0, 0, 0, 0, 5, 5},
        {0, 0, 0, 4, 0, 0, 0},
        {4, 7, 0, 0, 0, 0, 0},
        {2, 0, 0, 7, 4, 0, 0},
        {0, 0, 0, 0, 0, 1, 0}
    };
    // vector<vector<int>> grid ={ 
    //     {0, 0, 0, 0, 0, 0, 0},
    //     {0, 0, 0, 0, 1, 0, 0},
    //     {0, 0, 0, 0, 0, 3, 0},
    //     {0, 4, 0, 0, 0, 0, 0},
    //     {0, 0, 0, 0, 2, 0, 0},
    //     {0, 0, 6, 0, 0, 0, 0},
    //     {0, 0, 0, 0, 0, 0, 0}
    // };

    cout << "Beginning search" << endl;
    bool couldSolve = solve247(grid);
    cout << (couldSolve ? "SOLVED" : "FAILED TO SOLVE") << endl;
    if (couldSolve) {
        cout << grid << endl;
    }
    // cout << grid << endl;


    return 0;
}
