// task1.2.cpp : Этот файл содержит функцию "main". Здесь начинается и заканчивается выполнение программы.
//

#include <iostream>
#include <random>
#include <boost/thread.hpp>
#include <vector>
#include <atomic>
#include <chrono>
std::atomic<int> bank_balance(0);
void increment() {
	std::random_device rd;
	std::mt19937 gen(rd());
	std::uniform_int_distribution<> dist(1, 100);
	int a;
	int b;

	if (dist(gen) < 50) {
		a = dist(gen);
		
		bank_balance += a;
	}
	else {
		b = dist(gen);
		
		bank_balance -= b;
	}
}
int main() {
	int n = 300000;

	auto start = std::chrono::high_resolution_clock::now();

	std::vector<boost::thread> threads;
	for (int i = 0; i < n; i++) {
		threads.emplace_back(increment);
	}
	for (auto& t : threads) t.join();

	auto end = std::chrono::high_resolution_clock::now();
	auto duration = std::chrono::duration_cast<std::chrono::milliseconds>(end - start);

	std::cout << "Time: " << duration.count() << " ms" << std::endl;

	std::cout << "Balance: " << bank_balance << std::endl;
	return 0;
}