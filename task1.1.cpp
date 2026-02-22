// forTest.cpp : Этот файл содержит функцию "main". Здесь начинается и заканчивается выполнение программы.
//
#include <random>
#include <iostream>
#include <boost/thread.hpp>
#include <vector>
#include <chrono>
template<size_t n>
void function(int(&ans)[n][n], int(&arr1)[n][n], int(&arr2)[n][n], int i) {
	for (int j = 0; j < n; ++j) {
		ans[i][j] = 0;
		for (int k = 0; k < n; ++k) {
			ans[i][j] += arr1[i][k] * arr2[k][j];
		}
	}
}

int main() {
	const int n = 290;

	std::random_device rd;
	std::mt19937 gen(rd());
	std::uniform_int_distribution<> dist(1, 100); 

	int arr1[n][n];
	int arr2[n][n];

	
	for (int i = 0; i < n; ++i) {
		for (int j = 0; j < n; ++j) {
			arr1[i][j] = dist(gen);
			arr2[i][j] = dist(gen);
		}
	}

	
	int ans[n][n] = {};

	auto start = std::chrono::high_resolution_clock::now();

	std::vector<boost::thread> threads;
	for (int i = 0; i < n; ++i) {

		threads.emplace_back(function<n>, std::ref(ans), std::ref(arr1), std::ref(arr2), i);

	}


	for (auto& t : threads) t.join();

	auto end = std::chrono::high_resolution_clock::now();
	auto duration = std::chrono::duration_cast<std::chrono::milliseconds>(end - start);

	std::cout << "Time: " << duration.count() << " ms" << std::endl;

	/*for (int i = 0; i < n; ++i) {
		for (int j = 0; j < n; ++j) {
			std::cout << ans[i][j] << " ";
		}
		std::cout << std::endl;
	}*/

	return 0;
}

