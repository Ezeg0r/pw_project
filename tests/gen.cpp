#include <bits/stdc++.h>
using namespace std;

int main() {
    srand(time(0));

    vector<int> sizes = {1000, 10000, 20000};

    int t = 1;

    for (int n : sizes) {
        for (int j = 0; j < 5; j++) {
            ofstream out("input_" + to_string(t++) + ".txt");

            out << n << '\n';

            for (int i = 0; i < n; i++) {
                out << 1 + rand() % 100000 << ' ';
            }
            out << '\n';
        }
    }

    return 0;
}