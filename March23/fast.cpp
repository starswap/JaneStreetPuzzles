#include <bits/stdc++.h>
using namespace std;

const int N_TRIES = 10000000;
const double EPS = 0.0000000001;

double random() {
    return (double) rand() / (double) RAND_MAX;
}

double jump(double curr) {
    if (curr > 1) {
        return 0;
    } else {
        return curr + random();
    }
}

double score(double jump_threshold) {

    double curr = 0;
    while (curr < jump_threshold) {
        curr += random();
    }
    return jump(curr);
}

double get_avg(double jump_threshold, int n) {
    double total = 0;
    for (int i = 0; i < n; ++i) {
        total += score(jump_threshold);
    }
    return total / n;
}

double get_final_answer(double jump_threshold, int n) {
    double total = 0;
    for (int i = 0; i < n; ++i) {
        total += (score(jump_threshold) == 0);
    }
    return total / n;
}


int main() {
    srand ( time(NULL) );

    double lo = 0;
    // double hi = 1;
    // while (hi - lo > EPS) {
    //     double ml = (lo + 1 * (hi - lo) / 3);
    //     double mh = (lo + 2 * (hi - lo) / 3);
        
    //     double fl = get_avg(ml, N_TRIES);
    //     double fh = get_avg(mh, N_TRIES);
    //     if (fl < fh) {
    //         lo = ml;
    //     } else {
    //         hi = mh;
    //     }
    // }

    // cout << setprecision(10) << get_avg(0.4, 100000000) << endl;

    const double THRESH = 0.3027756276107445;
    cout << setprecision(10) << get_final_answer(THRESH, 1000000000) << endl;

    // cout << setprecision(10) << ((hi + lo) / 2) << endl;
    return 0;
}
