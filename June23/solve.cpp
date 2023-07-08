#include <bits/stdc++.h>
using namespace std;

const int EMPTY = 100; // Represents a cell whose digit has not yet been allocated.
                       // 0 represents a cell which has been allocated as no digit.
bool avail_internal[GRID_SIZE];
#ifdef TEST_MODE
    const int GRID_SIZE = 5;
    const int topNums[] = {5312, 1, 1, 5, 4};
    const int sideNums[] = {444, 5, 3, 123, 1};
    int grid[GRID_SIZE][GRID_SIZE] = {{EMPTY, 4, 4, 4, EMPTY}, {5, 5, EMPTY, 5, EMPTY}, {3, EMPTY, EMPTY, 5, 4}, {1, 2, 3, EMPTY, EMPTY}, {2, EMPTY, 3, 5, EMPTY}};
#else
    const int GRID_SIZE = 9;
    const int topNums[] = {5, 1, 6, 1, 8, 1, 22, 7, 8};
    const int sideNums[] = {55, 1, 6, 1, 24, 3, 6, 7, 2};
    int grid[GRID_SIZE][GRID_SIZE] = {
        {5,5,0,0,0,0,0,0,0},
        {0,7,7,7,7,7,7,0,8},
        {0,0,2,0,2,0,0,7,8},
        {0,0,0,3,3,0,0,0,8},
        {0,0,0,0,0,3,0,0,8},
        {0,0,0,0,0,0,0,0,8},
        {5,0,0,0,0,0,0,0,0},
        {5,0,4,4,4,4,0,0,8},
        {5,8,0,0,0,0,0,0,8},
    };
#endif

// Printout procedure for showing the answer
void output_grid() {
    for (int r = 0; r < GRID_SIZE; ++r) {
        for (int c = 0; c < GRID_SIZE; ++c) {
            cout << grid[r][c] << "|";
        }
        cout << "-";
        cout << endl;
    }
}

// We use this to check that all of the numbers are in a single connected component
void dfs_fill(int i, int j) {

    // efficiently checking neighbours
    const int di[] = {1,0,0,-1};
    const int dj[] = {0,1,-1,0};

    if (i >= 0 && j >= 0 && i < GRID_SIZE && j < GRID_SIZE && grid[i][j] > 0) {
        grid[i][j] = -grid[i][j];
        for (unsigned int t = 0; t < 4; ++t) {
            dfs_fill(i + di[t], j + dj[t]);
        }
    }
}

// For each grid cell we try and run a dfs from there. That dfs tries to set all
// the cells it encounters to a negative value (to indicate visited). If we find 
// a second positive value after running dfs the 2 positive values must be unconnected
// not ok. 
bool dfs_check() {
    bool found = false;
    bool ok = true;
    for (int i = 0; i < GRID_SIZE; ++i) {
        for (int j = 0; j < GRID_SIZE; ++j) {
            if (grid[i][j] > 0) {
                if (found) {
                    ok = false;
                    break;
                } else {
                    found = true;
                    dfs_fill(i, j);
                }
            }
        }
        if (!ok) break;
    }

    // RESET
    for (int i = 0; i < GRID_SIZE; ++i) {
        for (int j = 0; j < GRID_SIZE; ++j) {
            if (grid[i][j] < 0) {
                grid[i][j] = -grid[i][j];
            }
        }
    }
    return ok;
}

// Checks if the GCD constraint is satisfied for the i'th row of the grid. 
bool GCD_check_row(int i) {
    int curr_number = 0;
    int curr_gcd = 0;
    int targ_gcd = sideNums[i];

    for (int j = 0; j < GRID_SIZE; ++j) {
        if ((grid[i][j] == 0 || grid[i][j] == EMPTY) && curr_number > 0) {
            if (curr_gcd == 0) {
                curr_gcd = curr_number;
            } else {
                curr_gcd = __gcd(curr_gcd, curr_number);
            }
            curr_number = 0;
        } else {
            curr_number *= 10;
            curr_number += (grid[i][j] == EMPTY) ? 0 : grid[i][j];
        }
    }
    
    if (curr_gcd == 0) {
        curr_gcd = curr_number;
    } else {
        curr_gcd = __gcd(curr_gcd, curr_number);
    }
    return curr_gcd == targ_gcd;
}

// checks if the GCD constraint is satisfied for the i'th column of the grid
bool GCD_check_col(int i) {
    int curr_number = 0;
    int curr_gcd = 0;
    int targ_gcd = topNums[i];

    for (int j = 0; j < GRID_SIZE; ++j) {
        if ((grid[j][i] == 0 || grid[j][i] == EMPTY) && curr_number > 0) {
            if (curr_gcd == 0) {
                curr_gcd = curr_number;
            } else {
                curr_gcd = __gcd(curr_gcd, curr_number);
            }
            curr_number = 0;
        } else {
            curr_number *= 10;
            curr_number += (grid[j][i] == EMPTY) ? 0 : grid[j][i];
        }
    }

    if (curr_gcd == 0) {
        curr_gcd = curr_number;
    } else {
        curr_gcd = __gcd(curr_gcd, curr_number);
    }
    return curr_gcd == targ_gcd;    
}

// Checks if it is still possible to satisfy the GCD constraint in the i'th row of the grid
// (for pruning purposes), the i'th row being usually a row which we haven't finished filling up yet.
bool GCD_check_row_unfinished(int i) {
    int curr_number = 0;
    int curr_gcd = 0;
    int targ_gcd = sideNums[i];

    bool contains_empty = false;

    for (int j = 0; j < GRID_SIZE; ++j) {
        if ((grid[i][j] == 0) && curr_number > 0) {
            if (!contains_empty) {
                if (curr_gcd == 0) {
                    curr_gcd = curr_number;
                } else {
                    curr_gcd = __gcd(curr_gcd, curr_number);
                }
            }
            curr_number = 0;
            contains_empty = false;
        } else {
            curr_number *= 10;
            curr_number += (grid[i][j] == EMPTY) ? 0 : grid[i][j];
            if (grid[i][j] == EMPTY) {
                contains_empty = true;
            }
        }
    }
    if (!contains_empty) {
        if (curr_gcd == 0) {
            curr_gcd = curr_number;
        } else {
            curr_gcd = __gcd(curr_gcd, curr_number);
        }
    }
    return curr_gcd == 0 || curr_gcd % targ_gcd == 0;
}

// Checks if it is still possible to satisfy the GCD constraint in the i'th column of the grid
// (for pruning purposes), the i'th column being usually a column which we haven't finished filling up yet.
bool GCD_check_col_unfinished(int i) {
    int curr_number = 0;
    int curr_gcd = 0;
    int targ_gcd = topNums[i];

    bool contains_empty = false;

    for (int j = 0; j < GRID_SIZE; ++j) {
        if ((grid[j][i] == 0) && curr_number > 0) {
            if (!contains_empty) {
                if (curr_gcd == 0) {
                    curr_gcd = curr_number;
                } else {
                    curr_gcd = __gcd(curr_gcd, curr_number);
                }
            }
            curr_number = 0;
            contains_empty = false;
        } else {
            curr_number *= 10;
            curr_number += (grid[j][i] == EMPTY) ? 0 : grid[j][i];
            if (grid[j][i] == EMPTY) {
                contains_empty = true;
            }
        }
    }

    if (!contains_empty) {
        if (curr_gcd == 0) {
            curr_gcd = curr_number;
        } else {
            curr_gcd = __gcd(curr_gcd, curr_number);
        }
    }
    return curr_gcd == 0 || curr_gcd % targ_gcd == 0;    
}

// Checks if we can place a number in the cell at r,c. In particular this checks the
// condition for "Furthermore, every 2-by-2 region must contain at least one unfilled square."
// As is usual, we make these checks as we go along rather than at the end, which enables greater pruning.
bool can_place(int r, int c) {
    for (int dr = -1; dr != 3; dr += 2) {
        for (int dc = -1; dc != 3; dc += 2) {
            if (grid[r + dr][c] != 0 && grid[r][c + dc] != 0 && grid[r + dr][c + dc] != 0 && grid[r + dr][c] != EMPTY && grid[r][c + dc] != EMPTY && grid[r + dr][c + dc] != EMPTY) {
                return false;
            }
        }
    }
    return true;
}

// Corecursive with solve. General architecture of the solution is that we start with the whole grid. Solve tries to place a hook then place_numbers tries 
// to place numbers inside that hook. It then recursively calls solve which does the next hook for the inner portion of the grid. Obviously
// for the backtracking, solve and place_numbers are actually both trying all possibilities.
bool place_numbers(int r, int c, int l, int i, int space_left, int nums_left, int dr, int dc, int hook_corner, int new_top_left_r, int new_top_left_c) {
    if (nums_left > space_left) {
        return false;
    } else if (space_left == 0) {
        if (hook_corner % 2 == 0) {
            if (!GCD_check_col(c)) return false; // pruning
        } else {
            if (!GCD_check_row(r)) return false; // pruning
        }
        return solve(l - 1, new_top_left_r, new_top_left_c); // corecursion
    }

    // Detemines the next square to go to (have we gone round the corner yet or not)
    int nextR, nextC;
    if (space_left <= l) {
        if (hook_corner % 2 == 0) {
            nextR = r + dr;
            nextC = c;
        } else {
            nextR = r;
            nextC = c + dc;
        }
    } else {
        if (hook_corner % 2 == 0) {
            nextR = r;
            nextC = c + dc;
        } else {
            nextR = r + dr;
            nextC = c;
        }
    }

    // Check that placing a number here won't break the constraint for GCD in this row / column
    grid[r][c] = i;
    bool ok = true;
    if (space_left == l) {
        if (hook_corner % 2 == 0) {
            if (!GCD_check_row(r)) ok = false;
        } else {
            if (!GCD_check_col(c)) ok = false;
        }
    }

    if (space_left <= l) {
        if (hook_corner % 2 == 0) {
            if (!GCD_check_col_unfinished(c)) {
                ok = false;
            }
        } else {
            if (!GCD_check_row_unfinished(r)) {
                ok = false;
            }
        }
    } 
    if (space_left >= l) {
        if (hook_corner % 2 == 0) {
            if (!GCD_check_row_unfinished(r)) {
                ok = false;
            }
        } else {
            if (!GCD_check_col_unfinished(c)) {
                ok = false;
            }
        }
    }
    
    // try with a number here
    if (ok && nums_left > 0 && can_place(r, c) && place_numbers(nextR, nextC, l, i, space_left - 1, nums_left - 1, dr, dc, hook_corner, new_top_left_r, new_top_left_c)) { // try placing
        return true;
    }

    // Check that we can go by not placing a number here and try if we can.
    grid[r][c] = 0;
    ok = true;
    if (space_left == l) {
        if (hook_corner % 2 == 0) {
            if (!GCD_check_row(r)) ok = false;
        } else {
            if (!GCD_check_col(c)) ok = false;
        }
    }

    if (space_left <= l) {
        if (hook_corner % 2 == 0) {
            if (!GCD_check_col_unfinished(c)) ok = false;
        } else {
            if (!GCD_check_row_unfinished(r)) ok = false;
        }
    }
    if (space_left >= l){
        if (hook_corner % 2 == 0) {
            if (!GCD_check_row_unfinished(r)) ok = false;
        } else {
            if (!GCD_check_col_unfinished(c)) ok = false;
        }
    }

    if (ok && dfs_check() && place_numbers(nextR, nextC, l, i, space_left - 1, nums_left, dr, dc, hook_corner, new_top_left_r, new_top_left_c)) { // try not placing
        return true;
    }

    grid[r][c] = EMPTY;
    return false;
}

// Calculates where on the hook to start. 
// ^| = type 0 hook
// _| = type 1 hook
// |_ = type 2 hook
// |^ = type 3 hook
pair<int, int> calc_top_corner(int curr_size, int top_left_r, int top_left_c, int hook_corner) {
    switch (hook_corner) {
        case (0): return make_pair(top_left_r, top_left_c);
        case (1): return make_pair(top_left_r, top_left_c + curr_size - 1);
        case (2): return make_pair(top_left_r + curr_size - 1, top_left_c + curr_size - 1);
        case (3): return make_pair(top_left_r + curr_size - 1, top_left_c);
        default: exit(1000);
    }
}

// Places the hooks, delegating to place_numbers to actually place the numbers inside the hooks
bool solve(int curr_size, int top_left_r, int top_left_c) {
    const int dr[] = {1,1,-1,-1};
    const int dc[] = {1,-1,-1,1};

    if (curr_size == 0) { // done
        return dfs_check();
    } else {
        for (int hook_corner = 0; hook_corner < 4; ++hook_corner) {
            auto [r,c] = calc_top_corner(curr_size, top_left_r, top_left_c, hook_corner);
            int new_top_left_r = top_left_r + (hook_corner == 0 || hook_corner == 3);
            int new_top_left_c = top_left_c + (hook_corner == 2 || hook_corner == 3);
            for (int internal = 1; internal <= GRID_SIZE; internal++) {
                if (!avail_internal[internal-1] || 2 * curr_size - 1 < internal || (internal == 1 && curr_size != 1)) continue;
                else {
                    avail_internal[internal-1] = false;
                    if (place_numbers(r, c, curr_size, internal, curr_size * 2 - 1, internal, dr[hook_corner], dc[hook_corner], hook_corner, new_top_left_r, new_top_left_c)) {
                        return true;
                    }
                    avail_internal[internal-1] = true;
                }
            }

        } 
        return false;
    }
}

// Check that the GCD constraint is satisfied (can be used to check at the end, though for pruning reasons we actually do a different check throughout)
bool GCD_check() {
    bool ok = true;
    for (int i = 0; i < GRID_SIZE; ++i) {
        ok &= GCD_check_col(i);
        ok &= GCD_check_row(i);
    }
    return ok;
}

int main() {
    /* Initialisation step */

    // Available numbers to place internally (initially we can place all of them from 1x1 to 9x9)
    for (int i = 0; i < GRID_SIZE; ++i) {
        avail_internal[i] = true;
    }

    // Empty the grid by default
    for (int i = 0; i < GRID_SIZE; ++i) {
        for (int j = 0; j < GRID_SIZE; ++j) {
            grid[i][j] = EMPTY;
        }
    }


    /* Solve step */

    // Do note that this is incredibly slow. It took somewhere between 30 mins and a couple of hours to solve it. 
    cout << (solve(GRID_SIZE, 0, 0) ? "success" : "failure") << endl;

    /* Output step*/

    // Display the answer
    output_grid();
}
