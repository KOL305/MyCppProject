#include <iostream>
#include <cmath>
#include <vector>
#include <algorithm>

int triangularNumber(int k) {
    return k * (k + 1) / 2;
}

std::vector<int> findThreeTriangularNumbers(int n) {
    std::vector<int> result(3, 0);
    int maxK = static_cast<int>(sqrt(2 * n));

    for (int i = maxK; i >= 0; --i) {
        int Ti = triangularNumber(i);
        if (Ti > n) continue;
        for (int j = 0; j <= maxK; ++j) {
            int Tj = triangularNumber(j);
            if (Ti + Tj > n) continue;
            int remaining = n - Ti - Tj;
            int k = static_cast<int>((sqrt(8 * remaining + 1) - 1) / 2);
            if (triangularNumber(k) == remaining) {
                result[0] = i;
                result[1] = j;
                result[2] = k;
                return result;
            }
        }
    }
    return result; // This should never be reached
}

int main() {
    int n;
    std::cout << "Enter a number: ";
    std::cin >> n;

    std::vector<int> triangulars = findThreeTriangularNumbers(n);
    std::cout << n << " = T(" << triangulars[0] << ") + T(" << triangulars[1] << ") + T(" << triangulars[2] << ")\n";

    return 0;
}