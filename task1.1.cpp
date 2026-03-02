// forTest.cpp : Этот файл содержит функцию "main". Здесь начинается и заканчивается выполнение программы.
//
#include <random>
#include <iostream>
#include <boost/thread.hpp>
#include <vector>
#include <chrono>
/*template<size_t n>
void function(int(&ans)[n][n], int(&arr1)[n][n], int(&arr2)[n][n], int i) {
	for (int j = 0; j < n; ++j) {
		ans[i][j] = 0;
		for (int k = 0; k < n; ++k) {
			ans[i][j] += arr1[i][k] * arr2[k][j];
		}
	}
}*/

int main() {
	int num = 2;
    int n = 250;

    for (num; num < 9; num *= 2) {
        std::cout << ">>>>>>>>>>>  " << num << "  <<<<<<<<<<" << std::endl;
        std::random_device rd;
        std::mt19937 gen(rd());
        std::uniform_int_distribution<> dist(1, 100);
        n = 250;

        for (n; n < 1001; n *= 2) {
            int** arr1 = new int* [n];
            int** arr2 = new int* [n];
            int** ans1 = new int* [n];
            int** ans2 = new int* [n];

            for (int i = 0; i < n; ++i) {
                arr1[i] = new int[n];
                arr2[i] = new int[n];
                ans1[i] = new int[n];
                ans2[i] = new int[n];
            }

            for (int i = 0; i < n; ++i) {
                for (int j = 0; j < n; ++j) {
                    arr1[i][j] = dist(gen);
                    arr2[i][j] = dist(gen);
                }
            }

            auto start = std::chrono::high_resolution_clock::now();

            std::vector<boost::thread> threads;
            int rows = n / num;

            for (int t = 0; t < num; ++t) {
                int start_row = t * rows;
                int end_row = start_row + rows;

                threads.emplace_back([&, start_row, end_row]() {
                    for (int i = start_row; i < end_row; ++i) {
                        for (int j = 0; j < n; ++j) {
                            ans1[i][j] = 0;
                            for (int k = 0; k < n; ++k) {
                                ans1[i][j] += arr1[i][k] * arr2[k][j];
                            }
                        }
                    }
                    });
            }

            for (auto& t : threads) {
                t.join();
            }

            auto end = std::chrono::high_resolution_clock::now();
            auto duration = std::chrono::duration_cast<std::chrono::milliseconds>(end - start);

            std::cout << "Multipotock time with " << n << " potocks: " << duration.count() << " ms" << std::endl;


            auto start1 = std::chrono::high_resolution_clock::now();

            for (int i = 0; i < n; ++i) {
                for (int j = 0; j < n; ++j) {
                    ans2[i][j] = 0;
                    for (int k = 0; k < n; ++k) {
                        ans2[i][j] += arr1[i][k] * arr2[k][j];
                    }
                }

            }

            auto end1 = std::chrono::high_resolution_clock::now();
            auto duration1 = std::chrono::duration_cast<std::chrono::milliseconds>(end1 - start1);

            std::cout << "One potock time with " << n << " x " << n << " matrix: " << duration1.count() << " ms" << std::endl;
        }
    }

	return 0;
}

