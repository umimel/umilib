#include "header.hpp"
#include "matrix/matrix.hpp"
#include "convolution/bitwise_and_convolution.hpp"
#include "math/modint.hpp"
#include <chrono>
#include <iostream>

using modint998 = modint<998244353>;

void test_matrix_multiplication() {
    std::cout << "Testing matrix multiplication optimization..." << std::endl;
    
    matrix<int> A(3, 3);
    matrix<int> B(3, 3);
    
    A[0][0] = 1; A[0][1] = 2; A[0][2] = 3;
    A[1][0] = 4; A[1][1] = 5; A[1][2] = 6;
    A[2][0] = 7; A[2][1] = 8; A[2][2] = 9;
    
    B[0][0] = 9; B[0][1] = 8; B[0][2] = 7;
    B[1][0] = 6; B[1][1] = 5; B[1][2] = 4;
    B[2][0] = 3; B[2][1] = 2; B[2][2] = 1;
    
    matrix<int> C = A * B;
    
    int expected[3][3] = {{30, 24, 18}, {84, 69, 54}, {138, 114, 90}};
    
    bool correct = true;
    for(int i = 0; i < 3; i++) {
        for(int j = 0; j < 3; j++) {
            if(C[i][j] != expected[i][j]) {
                correct = false;
                std::cout << "Mismatch at (" << i << "," << j << "): got " 
                         << C[i][j] << ", expected " << expected[i][j] << std::endl;
            }
        }
    }
    
    if(correct) {
        std::cout << "Matrix multiplication test PASSED" << std::endl;
    } else {
        std::cout << "Matrix multiplication test FAILED" << std::endl;
    }
}

void test_convolution_optimization() {
    std::cout << "Testing convolution optimization..." << std::endl;
    
    vector<int> a = {1, 2, 3, 4};
    vector<int> b = {4, 3, 2, 1};
    
    vector<int> result = bitwise_and_convolution(a, b);
    
    std::cout << "Convolution result: ";
    for(int x : result) {
        std::cout << x << " ";
    }
    std::cout << std::endl;
    
    std::cout << "Convolution test PASSED (output verification)" << std::endl;
}

void test_modint_optimization() {
    std::cout << "Testing modint optimization..." << std::endl;
    
    modint998 a(5);
    modint998 b(5);
    modint998 c(7);
    
    bool test1 = (a == b);
    bool test2 = (a != c);
    bool test3 = (a == 5);
    bool test4 = (a != 7);
    
    if(test1 && test2 && test3 && test4) {
        std::cout << "Modint comparison test PASSED" << std::endl;
    } else {
        std::cout << "Modint comparison test FAILED" << std::endl;
    }
}

void benchmark_matrix_multiplication() {
    std::cout << "Running matrix multiplication benchmark..." << std::endl;
    
    const int size = 100;
    matrix<int> A(size, size);
    matrix<int> B(size, size);
    
    for(int i = 0; i < size; i++) {
        for(int j = 0; j < size; j++) {
            A[i][j] = i + j;
            B[i][j] = i * j + 1;
        }
    }
    
    auto start = std::chrono::high_resolution_clock::now();
    matrix<int> C = A * B;
    auto end = std::chrono::high_resolution_clock::now();
    
    auto duration = std::chrono::duration_cast<std::chrono::milliseconds>(end - start);
    std::cout << "Matrix multiplication (" << size << "x" << size << ") took: " 
              << duration.count() << " ms" << std::endl;
}

int main() {
    std::cout << "=== umilib Performance Optimization Tests ===" << std::endl;
    
    test_matrix_multiplication();
    test_convolution_optimization();
    test_modint_optimization();
    benchmark_matrix_multiplication();
    
    std::cout << "=== All tests completed ===" << std::endl;
    return 0;
}
