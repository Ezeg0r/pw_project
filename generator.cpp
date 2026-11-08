#include <iostream>
#include <random>

using namespace std;

int main() {
    int n;
    cin >> n;

    mt19937 rng(42);
    uniform_int_distribution<int> length(1, 10000);

    cout << n << '\n';
    for (int i = 0; i < n; ++i) {
        if (i > 0)
            cout << ' ';
        cout << length(rng);
    }
    cout << '\n';
}
