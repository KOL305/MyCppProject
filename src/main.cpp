// main.cpp
#include <gmp.h>
#include <gmpxx.h>
#include <iostream>
#include <cmath>

// Function to find the largest power of 2 less than or equal to k
mpz_class find_largest_power_of_2(const mpz_class& k) {
    int j = 0;
    mpz_class Q = 1;
    while (Q <= k) {
        Q <<= 1; // Q = Q * 2
        j += 1;
    }
    Q >>= 1; // Q = Q / 2, because Q has gone one step beyond the largest power of 2 <= k
    j -= 1;
    std::cout << "Value of j: " << j << std::endl;
    return Q;
}

int main() {
    // Constants (these should be defined as per the problem requirements)
    mpz_class k = 8388609;  // Example value

    // Find Q
    mpz_class Q = find_largest_power_of_2(k);

    // Target value to match
    mpz_class target = k - Q;

    // Variables to store the minimal n1 and corresponding n2, n3
    mpz_class min_n1, best_n2, best_n3;
    min_n1 = -1; // Initialize to an invalid state

    // Iterate over possible values for n1, n2, and n3
    for (mpz_class n1 = 0; n1 * n1 <= target; ++n1) {
        for (mpz_class n2 = n1; n2 * n2 + n1 * n1 <= target; ++n2) {
            mpz_class n1_squared = n1 * n1;
            mpz_class n2_squared = n2 * n2;
            mpz_class remaining = target - n1_squared - n2_squared;

            if (remaining < 0) {
                continue;
            }

            // Calculate n3, it must be an integer such that n3^2 = remaining
            mpz_class n3_sqrt;
            mpz_sqrt(n3_sqrt.get_mpz_t(), remaining.get_mpz_t());

            if (n3_sqrt * n3_sqrt != remaining) {
                continue;
            }

            // Ensure n3 >= n2
            if (n3_sqrt >= n2) {
                if (min_n1 == -1 || n1 < min_n1) {
                    min_n1 = n1;
                    best_n2 = n2;
                    best_n3 = n3_sqrt;
                }
            }
        }
    }

    if (min_n1 != -1) {
        std::cout << "Optimal values found:\n";
        std::cout << "n1: " << min_n1 << "\n";
        std::cout << "n2: " << best_n2 << "\n";
        std::cout << "n3: " << best_n3 << "\n";
    } else {
        std::cout << "No valid solution found.\n";
    }

    std::cout << "Q: " << Q << "\n";

    return 0;
}
