#include <bits/stdc++.h>
using namespace std;

// const int HASHI_HEIGHT = 7;
// const int HASHI_WIDTH  = 7;
const int HASHI_HEIGHT = 13;
const int HASHI_WIDTH  = 17;
const int HASHI_MIN    = 1;
const int HASHI_MAX    = 7;
const int BLANK_SQUARE = -1;
const int HORIZONTAL_BRIDGE = HASHI_MAX+1;
const int VERTICAL_BRIDGE   = HASHI_MAX+2;
const int HORIZONTAL_DOUBLE = HASHI_MAX+3;
const int VERTICAL_DOUBLE   = HASHI_MAX+4;

const int dr[] = {0,0,1,-1};
const int dc[] = {1,-1,0,0};

typedef array<array<int,HASHI_WIDTH>,HASHI_HEIGHT> HashiPuzzle;

ostream& operator << (ostream& o, HashiPuzzle p) {
    for (auto row : p) {
        for (auto number : row) {
            if (0 <= number && number <= HASHI_MAX) {
                o << static_cast<char>('0'+number);
            }
            else if (number == BLANK_SQUARE) {
                o << ' ';
            }
            else if (number == HORIZONTAL_BRIDGE) {
                o << '-';
            }
            else if (number == HORIZONTAL_DOUBLE) {
                o << '=';
            }
            else if (number == VERTICAL_BRIDGE) {
                o << '|';
            }
            else if (number == VERTICAL_DOUBLE) {
                o << "H"; // Should be unicode ‖ but windows terminal bad. Will just find and replace after smh.
            }
            else {
                o << "ERROR: Unexpected integer token " << number << " in Hashi Puzzle" << endl;
            }
        }
        o << endl;
    }


    return o;
}


void setRange(HashiPuzzle &hashi, int dr, int dc, int r, int c, int tr, int tc, int val) {
    tr -= dr;
    tc -= dc;
    while (r != tr || c != tc) {
        hashi[tr][tc] = val;
        tr -= dr;
        tc -= dc;
    }
}


// Add change one character as a move
bool makeMove(HashiPuzzle& hashi) {
    for (int r=0;r<HASHI_HEIGHT;r++) {
        for (int c=0;c<HASHI_WIDTH;c++) {
            int &number = hashi[r][c];

            if (!(HASHI_MIN <= number && number <= HASHI_MAX)) {continue;} 
            else { //is a number
                for (int direction = 0; direction < 4; direction++) {
                    
                    int tr = r + dr[direction];
                    int tc = c + dc[direction];
                    bool thisDirectionFailed = false;
                    while (tr >= 0 && tr < HASHI_HEIGHT && tc >= 0 && tc < HASHI_WIDTH) {
                        if (hashi[tr][tc] == HORIZONTAL_BRIDGE || hashi[tr][tc] == HORIZONTAL_DOUBLE || hashi[tr][tc] == VERTICAL_BRIDGE || hashi[tr][tc] == VERTICAL_DOUBLE  || hashi[tr][tc] == 0) {
                            thisDirectionFailed = true;
                            break;
                        }
                        if (HASHI_MIN <= hashi[tr][tc] && hashi[tr][tc] <= HASHI_MAX) {
                            break;
                        }
                        tr += dr[direction];
                        tc += dc[direction];
                    } 

                    if (thisDirectionFailed || tr < 0 || tr >= HASHI_HEIGHT || tc < 0 || tc >= HASHI_WIDTH) { // try the next direction.
                        continue;
                    }

                    // Otherwise, tr, tc represents the target to join up to.



                    if (number >= 2 && hashi[tr][tc] >= 2) { // try double bridges
                        hashi[r][c]   -= 2;
                        hashi[tr][tc] -= 2;
                        setRange(hashi,dr[direction],dc[direction],r,c,tr,tc,(dr[direction] == 0 ? HORIZONTAL_DOUBLE : VERTICAL_DOUBLE));
                        
                        if (makeMove(hashi)) {
                            hashi[r][c]   += 2;
                            hashi[tr][tc] += 2;
                            return true;
                        }
                        else {
                            setRange(hashi,dr[direction],dc[direction],r,c,tr,tc,BLANK_SQUARE);
                            hashi[r][c]   += 2;
                            hashi[tr][tc] += 2;
                        }
                    }

                    hashi[r][c]   -= 1;
                    hashi[tr][tc] -= 1;
                    setRange(hashi,dr[direction],dc[direction],r,c,tr,tc,(dr[direction] == 0 ? HORIZONTAL_BRIDGE : VERTICAL_BRIDGE));
                    
                    if (makeMove(hashi)) {
                        hashi[r][c]   += 1;
                        hashi[tr][tc] += 1;
                        return true;
                    }
                    else {
                        //RESET
                        setRange(hashi,dr[direction],dc[direction],r,c,tr,tc,BLANK_SQUARE);
                        hashi[r][c]   += 1;
                        hashi[tr][tc] += 1;
                    }
            
                    // try single bridges
                }
            }
            return false; // Failed on this island.
        }
    }

    return true;
}

int main() {
    // HashiPuzzle hashi = {
    //     {2,-1,-1, 1,-1,-1, 3,
    //     -1,-1,-1,-1,-1,-1,-1,
    //      3,-1,-1,-1,-1,-1, 5,
    //     -1,-1,-1,-1,-1,-1,-1,
    //      3,-1,-1,-1,-1, 1,-1,
    //     -1,-1,-1,-1,-1,-1,-1,
    //      4,-1,-1,-1,-1,-1, 4}
    // };
    // HashiPuzzle originalHashi = {
    //      4,-1,-1,-1,-1,-1, 5,-1, 3,-1,-1,-1,-1,-1, 4,-1, 1,
    //     -1,-1,-1,-1,-1,-1,-1,-1,-1,-1,-1,-1,-1,-1,-1,-1,-1,
    //     -1,-1,-1,-1,-1,-1,-1,-1, 2,-1,-1,-1,-1,-1,-1,-1,-1,
    //     -1,-1,-1,-1,-1,-1,-1,-1,-1,-1,-1,-1,-1,-1,-1,-1,-1,
    //      6,-1, 2,-1,-1,-1, 5,-1, 5,-1,-1,-1, 4,-1,-1,-1,-1,
    //     -1,-1,-1,-1,-1,-1,-1,-1,-1,-1,-1,-1,-1,-1,-1,-1,-1,
    //     -1,-1, 3,-1,-1,-1,-1,-1,-1,-1,-1,-1,-1,-1, 6,-1, 4,
    //     -1,-1,-1,-1,-1,-1,-1,-1,-1,-1,-1,-1,-1,-1,-1,-1,-1,
    //     -1,-1, 6,-1,-1,-1, 3,-1,-1,-1,-1,-1,-1,-1,-1,-1,-1,
    //     -1,-1,-1,-1,-1,-1,-1,-1,-1,-1,-1,-1,-1,-1,-1,-1,-1,
    //      2,-1,-1,-1,-1,-1,-1,-1, 4,-1,-1,-1, 4,-1,-1,-1,-1,
    //     -1,-1,-1,-1,-1,-1,-1,-1,-1,-1,-1,-1,-1,-1,-1,-1,-1,
    //     -1,-1, 3,-1,-1,-1,-1,-1,-1,-1,-1,-1,-1,-1, 5,-1, 4,
    // };

    // HashiPuzzle originalHashi = {
    //          4,-1, 4,-1, 4,-1,-1,-1,-1,-1, 5,-1,-1,-1, 2,-1, 1,
    //         -1,-1,-1,-1,-1,-1,-1,-1,-1,-1,-1,-1,-1,-1,-1,-1,-1,
    //         -1,-1, 4,-1,-1,-1,-1,-1,-1,-1,-1,-1, 1,-1,-1,-1,-1,
    //         -1,-1,-1,-1,-1,-1,-1,-1,-1,-1,-1,-1,-1,-1,-1,-1,-1,
    //          3,-1, 2,-1, 4,-1,-1,-1, 4,-1, 4,-1,-1,-1, 5,-1, 3,
    //         -1,-1,-1,-1,-1,-1,-1,-1,-1,-1,-1,-1,-1,-1,-1,-1,-1,
    //          5,-1, 4,-1, 4,-1, 3,-1,-1,-1, 5,-1, 1,-1,-1,-1,-1,
    //         -1,-1,-1,-1,-1,-1,-1,-1,-1,-1,-1,-1,-1,-1,-1,-1,-1,
    //         -1,-1,-1,-1,-1,-1,-1,-1,-1,-1, 6,-1, 4,-1, 5,-1, 4,
    //         -1,-1,-1,-1,-1,-1,-1,-1,-1,-1,-1,-1,-1,-1,-1,-1,-1,
    //          6,-1, 6,-1,-1,-1,-1,-1,-1,-1,-1,-1,-1,-1,-1,-1,-1,
    //         -1,-1,-1,-1,-1,-1,-1,-1,-1,-1,-1,-1,-1,-1,-1,-1,-1,
    //          3,-1, 4,-1, 5,-1,-1,-1, 2,-1, 4,-1, 2,-1, 2,-1, 4,
    //     };

    HashiPuzzle originalHashi = {
             4,-1, 4,-1, 2,-1,-1,-1, 2,-1, 4,-1,-1,-1, 4,-1, 2,
            -1,-1,-1,-1,-1,-1,-1,-1,-1,-1,-1,-1,-1,-1,-1,-1,-1,
            -1,-1, 4,-1,-1,-1,-1,-1,-1,-1, 4,-1,-1,-1,-1,-1, 1,
            -1,-1,-1,-1,-1,-1,-1,-1,-1,-1,-1,-1,-1,-1,-1,-1,-1,
             4,-1, 7,-1, 6,-1, 4,-1,-1,-1,-1,-1,-1,-1, 5,-1, 4,
            -1,-1,-1,-1,-1,-1,-1,-1,-1,-1,-1,-1,-1,-1,-1,-1,-1,
            -1,-1,-1,-1, 5,-1,-1,-1,-1,-1,-1,-1,-1,-1, 4,-1,-1,
            -1,-1,-1,-1,-1,-1,-1,-1,-1,-1,-1,-1,-1,-1,-1,-1,-1,
            -1,-1,-1,-1,-1,-1, 4,-1, 5,-1, 7,-1, 2,-1, 2,-1, 2,
            -1,-1,-1,-1,-1,-1,-1,-1,-1,-1,-1,-1,-1,-1,-1,-1,-1,
             3,-1, 4,-1,-1,-1,-1,-1,-1,-1, 4,-1,-1,-1,-1,-1,-1,
            -1,-1,-1,-1,-1,-1,-1,-1,-1,-1,-1,-1,-1,-1,-1,-1,-1,
             1,-1,-1,-1, 3,-1,-1, 1,-1,-1, 4,-1,-1,-1, 4,-1, 2,
        };


    // HashiPuzzle originalHashi = {
    //          4,-1, 2,-1, 3,-1, 4,-1,-1,-1, 5,-1, 5,-1,-1,-1, 4,
    //         -1,-1,-1,-1,-1,-1,-1,-1,-1,-1,-1,-1,-1,-1,-1,-1,-1,
    //         -1,-1,-1,-1,-1,-1, 5,-1,-1,-1, 6,-1,-1,-1,-1,-1,-1,
    //         -1,-1,-1,-1,-1,-1,-1,-1,-1,-1,-1,-1,-1,-1,-1,-1,-1,
    //          4,-1, 4,-1,-1,-1,-1,-1,-1,-1,-1,-1, 6,-1,-1,-1, 4,
    //         -1,-1,-1,-1,-1,-1,-1,-1,-1,-1,-1,-1,-1,-1,-1,-1,-1,
    //         -1,-1,-1,-1,-1,-1, 4,-1,-1,-1, 4,-1,-1,-1, 4,-1, 2,
    //         -1,-1,-1,-1,-1,-1,-1,-1,-1,-1,-1,-1,-1,-1,-1,-1,-1,
    //          3,-1, 4,-1, 6,-1,-1,-1, 4,-1,-1,-1,-1,-1,-1,-1,-1,
    //         -1,-1,-1,-1,-1,-1,-1,-1,-1,-1,-1,-1,-1,-1,-1,-1,-1,
    //         -1,-1,-1,-1,-1,-1,-1,-1,-1,-1, 4,-1, 4,-1, 7,-1, 3,
    //         -1,-1,-1,-1,-1,-1,-1,-1,-1,-1,-1,-1,-1,-1,-1,-1,-1,
    //          2,-1,-1,-1, 3,-1,-1,-1, 2,-1,-1,-1,-1,-1, 4,-1, 3,
    //     };


    HashiPuzzle hashi = originalHashi;
    cout << (makeMove(hashi) ? "Succeeded in Solving" : "Failed to Solve") << endl;

    for (int r=0;r<HASHI_HEIGHT;r++) {
        for (int c=0;c<HASHI_WIDTH;c++) {
            if (HASHI_MIN <= hashi[r][c] && hashi[r][c] <= HASHI_MAX) {
                for (int i=HASHI_MIN;i<=HASHI_MAX;++i) {
                    int old = hashi[r][c];
                    hashi[r][c] = i;
                    if (makeMove(hashi)) {
                        cout << "Solved switching " << "(" << r << "," << c << ")" << " from " << old << " to " << i << "." << endl;
                        cout << hashi << endl;
                        hashi = originalHashi; // We need this to reset the bridges after a successful solve.
                    }
                    hashi[r][c] = old;
                }
            
                int old = hashi[r][c];
                hashi[r][c] = -1;
                if (makeMove(hashi)) {
                    cout << "Solved switching " << "(" << r << "," << c << ")" << " from " << old << " to " << -1 << "." << endl;
                    cout << hashi << endl;
                }
                hashi[r][c] = old;
            
            }

            // cout << hashi << endl;

        }
    }



}
