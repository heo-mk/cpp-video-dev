#include <iostream>
#include <chrono>

long long calculate_sum(long long max_num) {
    long long sum = 0;
    for (long long i = 1; i <= max_num; ++i) {
        sum += i;
    }
    return sum;
}

int main() {
    const long long MAX_NUM = 1000000; // 100만

    std::cout << "=== 1단계: 1부터 100만까지 합 계산 및 시간 측정 ===" << std::endl;

    // --- 1. main 함수 내 반복문 직접 사용 버전 ---
    auto start1 = std::chrono::high_resolution_clock::now();

    long long sum1 = calculate_sum(MAX_NUM);

    auto end1 = std::chrono::high_resolution_clock::now();
    std::chrono::duration<double, std::milli> duration1 = end1 - start1;

    std::cout << "[버전 1: main 반복문]" << std::endl;
    std::cout << "합계: " << sum1 << std::endl;
    std::cout << "소요 시간: " << duration1.count() << " ms" << std::endl << std::endl;

    return 0;
}
