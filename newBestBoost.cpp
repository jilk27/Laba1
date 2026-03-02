#include <iostream>
#include <random>
#include <boost/thread.hpp>
#include <vector>
#include <atomic>
#include <chrono>
#include <mutex>

std::mutex mtx;
int bank_balance = 0;

void increment_with_mutex(int iterations) {
    std::random_device rd;
    std::mt19937 gen(rd());
    std::uniform_int_distribution<> dist(-100, 100);

    for (int j = 0; j < iterations; j++) {
        int a = dist(gen);
        std::lock_guard<std::mutex> lock(mtx);
        bank_balance += a;
    }
}

std::atomic<int> balance(0);

void increment_with_atomic(int iterations) {
    std::random_device rd;
    std::mt19937 gen(rd());
    std::uniform_int_distribution<> dist(-100, 100);

    for (int j = 0; j < iterations; j++) {
        int a = dist(gen);
        balance += a;    
    }
}

int main() {
    int num_threads = 2; 

    for (num_threads; num_threads < 9; num_threads *= 2) {
        std::cout << ">>>>>>>>>>>>  " << num_threads << "  <<<<<<<<<<<<<" << std::endl;
        for (int total_ops = 200000; total_ops <= 1000000; total_ops += 100000) {
            std::cout << "========== " << total_ops << " operations" << std::endl;

            bank_balance = 0;
            balance = 0;


            auto start = std::chrono::high_resolution_clock::now();

            std::vector<boost::thread> threads_mutex;
            for (int i = 0; i < num_threads; i++) {
                threads_mutex.emplace_back(increment_with_mutex, total_ops / num_threads);
            }
            for (auto& t : threads_mutex) t.join();

            auto end = std::chrono::high_resolution_clock::now();
            auto duration = std::chrono::duration_cast<std::chrono::milliseconds>(end - start);

            std::cout << "Mutex: " << duration.count() << " ms, Balance: " << bank_balance << std::endl;





            start = std::chrono::high_resolution_clock::now();

            std::vector<boost::thread> threads_atomic;
            for (int i = 0; i < num_threads; i++) {
                threads_atomic.emplace_back(increment_with_atomic, total_ops / num_threads);
            }
            for (auto& t : threads_atomic) t.join();

            end = std::chrono::high_resolution_clock::now();
            duration = std::chrono::duration_cast<std::chrono::milliseconds>(end - start);

            std::cout << "Atomic: " << duration.count() << " ms, Balance: " << balance << std::endl;
        }
    }

    return 0;

}
