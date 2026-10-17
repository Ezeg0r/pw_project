#include <bits/stdc++.h>

using namespace std;

int main(int argc, char* argv[]) {
    ios::sync_with_stdio(0);
    cin.tie(0);

    int T = argc > 1
        ? stoi(argv[1])
        : static_cast<int>(max(1u, thread::hardware_concurrency()));
    int BLOCK = argc > 2 ? stoi(argv[2]) : 32;

    int n;
    cin >> n;

    vector<int> a(n);
    for (int& x : a) {
        cin >> x;
    }

    sort(a.begin(), a.end());

    int next_k = 2;
    mutex mx;
    vector<thread> threads;
    vector<long long> results(T);
    threads.reserve(T);

    for (int tid = 0; tid < T; ++tid) {
        threads.emplace_back([&, tid] {
            long long local_res = 0;

            while (true) {
                int begin, end;

                {
                    lock_guard<mutex> lock(mx);
                    if (next_k >= n) {
                        break;
                    }

                    begin = next_k;
                    end = begin + min(BLOCK, n - begin);
                    next_k = end;
                }

                for (int k = begin; k < end; ++k) {
                    int l = 0;
                    int r = k - 1;

                    while (l < r) {
                        if (1LL * a[l] + a[r] > a[k]) {
                            local_res += r - l;
                            r--;
                        } else {
                            l++;
                        }
                    }
                }
            }

            results[tid] = local_res;
        });
    }

    for (auto& t : threads) {
        t.join();
    }

    long long res = accumulate(results.begin(), results.end(), 0LL);
    cout << res << '\n';

    return 0;
}