
#include <bits/stdc++.h>

using namespace std;

int main() {
    ios::sync_with_stdio(0);
    cin.tie(0);

    int n;
    cin >> n;

    vector<int> a(n);
    for (int& x : a) {
        cin >> x;
    }

    sort(a.begin(), a.end());

    long long res = 0;

    for (int k = 2; k < n; ++k) {
        int l = 0;
        int r = k - 1;

        while (l < r) {
            if (1LL * a[l] + a[r] > a[k]) {
                res += r - l;
                r--;
            } else {
                l++;
            }
        }
    }

    cout << res << '\n';

    return 0;
}