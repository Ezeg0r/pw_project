#include <algorithm>
#include <atomic>
#include <iostream>
#include <numeric>
#include <string_view>
#include <system_error>
#include <thread>
#include <vector>
#include <string>

enum class Schedule { sequential, equal, balanced, dynamic };

long long count_range(const std::vector<int>& lengths, int begin, int end) {
    long long count = 0;
    for (int k = begin; k < end; ++k) {
        int left = 0;
        int right = k - 1;
        while (left < right) {
            if (static_cast<long long>(lengths[left]) + lengths[right] > lengths[k]) {
                count += right - left;
                --right;
            } else {
                ++left;
            }
        }
    }
    return count;
}

long long work_before(int end) {
    return 1LL * (end - 2) * (end - 1) / 2;
}

int balanced_boundary(int size, int worker, int worker_count) {
    const long long target = work_before(size) * worker / worker_count;
    int low = 2;
    int high = size;
    while (low < high) {
        const int mid = low + (high - low) / 2;
        if (work_before(mid) < target) {
            low = mid + 1;
        } else {
            high = mid;
        }
    }
    return low;
}

long long count_triangles(const std::vector<int>& lengths, Schedule schedule, int threads) {
    const int size = static_cast<int>(lengths.size());
    if (size < 3) {
        return 0;
    }
    const int worker_count = std::min(threads, size - 2);
    if (schedule == Schedule::sequential || worker_count == 1) {
        return count_range(lengths, 2, size);
    }

    constexpr int chunk_size = 32;
    std::atomic<int> next{2};
    std::vector<long long> totals(worker_count);
    const auto count_worker = [&](int worker) {
        if (schedule == Schedule::dynamic) {
            long long total = 0;
            for (;;) {
                const int begin = next.fetch_add(chunk_size, std::memory_order_relaxed);
                if (begin >= size) {
                    break;
                }
                total += count_range(lengths, begin, std::min(begin + chunk_size, size));
            }
            totals[worker] = total;
            return;
        }

        const auto boundary = [&](int index) {
            if (schedule == Schedule::equal) {
                return 2 + static_cast<int>(1LL * (size - 2) * index / worker_count);
            }
            return balanced_boundary(size, index, worker_count);
        };
        totals[worker] = count_range(lengths, boundary(worker), boundary(worker + 1));
    };

    std::vector<std::thread> workers;
    for (int worker = 1; worker < worker_count; ++worker) {
        workers.emplace_back(count_worker, worker);
    }
    count_worker(0);
    for (auto& thread : workers) {
        thread.join();
    }
    return std::accumulate(totals.begin(), totals.end(), 0LL);
}

int main(int argc, char* argv[]) {
    std::ios::sync_with_stdio(false);
    std::cin.tie(nullptr);

    Schedule schedule = Schedule::dynamic;
    constexpr unsigned default_max_threads = 128;
    int threads = static_cast<int>(
        std::clamp(std::thread::hardware_concurrency(), 1U, default_max_threads));

    if (argc >= 2) {
        std::string mode = argv[1];
        if (mode == "seq") {
            schedule = Schedule::sequential;
        } else if (mode == "equal") {
            schedule = Schedule::equal;
        } else if (mode == "balanced") {
            schedule = Schedule::balanced;
        } else if (mode == "dynamic") {
            schedule = Schedule::dynamic;
        }
    }
    if (argc == 3) {
        threads = atoi(argv[2]);
    }

    
    int size;
    std::cin >> size;
    std::vector<int> lengths(size);
    for (int& length : lengths) {
        std::cin >> length;
    }
    std::sort(lengths.begin(), lengths.end());
    const long long count = count_triangles(lengths, schedule, threads);
    std::cout << count << '\n';
    return 0;
}
