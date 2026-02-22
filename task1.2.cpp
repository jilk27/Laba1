// task1.2.cpp : Этот файл содержит функцию "main". Здесь начинается и заканчивается выполнение программы.
//

#include <iostream>
#include <random>
#include <boost/thread.hpp>
#include <vector>
int bank_balance = 0;
void increment() {
	std::random_device rd;
	std::mt19937 gen(rd());
	std::uniform_int_distribution<> dist(1, 100);
	int a;
	int b;

	if (dist(gen) < 50) {
		a = dist(gen);
		std::cout <<  "add" << a << "\n";
		bank_balance += a;
	}
	else {
		b = dist(gen);
		std::cout <<  "delete" << b << "\n";
		bank_balance -= b;
	}
}
int main() {
	int n = 5;

	std::vector<boost::thread> threads;
	for (int i = 0; i < n; i++) {
		threads.emplace_back(increment);
	}
	for (auto& t : threads) t.join();

	std::cout << "Balance: " << bank_balance << std::endl;
	return 0;
}