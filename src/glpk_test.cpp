#include <iostream>
#include <vector>
#include <cmath>
#include <gmp.h>
#include <mpfr.h>
#include <glpk.h>
#include <string>

// Function to generate the first P primes
std::vector<int> generate_primes(int P) {
    std::vector<int> primes;
    int count = 0;
    int num = 2;
    while (count < P) {
        bool is_prime = true;
        for (int i = 2; i <= std::sqrt(num); ++i) {
            if (num % i == 0) {
                is_prime = false;
                break;
            }
        }
        if (is_prime) {
            primes.push_back(num);
            ++count;
        }
        ++num;
    }
    return primes;
}

// Function to calculate upper bounds for the variables
std::vector<int> calculate_upper_bounds(const mpz_t& target_value, const std::vector<int>& primes) {
    std::vector<int> upper_bounds;
    mpz_t temp;
    mpz_init(temp);

    for (int prime : primes) {
        mpz_set(temp, target_value);
        int bound = 0;
        while (mpz_cmp_ui(temp, 1) > 0) {
            mpz_div_ui(temp, temp, prime);
            ++bound;
        }
        upper_bounds.push_back(bound);
    }

    mpz_clear(temp);
    return upper_bounds;
}

// Function to find the best C * 2^p approximation
void find_best_approximation(const mpz_t& target_value, mpz_t& best_value, int& best_p, mpz_t& best_C) {
    mpz_t two_power_p, temp, temp_diff, min_diff, C;
    mpz_init(two_power_p);
    mpz_init(temp);
    mpz_init(temp_diff);
    mpz_init(min_diff);
    mpz_init(C);

    // Initialize min_diff to a large value
    mpz_init_set(min_diff, target_value);

    mpz_set_ui(best_C, 0); // Initialize best_C to 0
    mpz_set_ui(best_value, 0);
    best_p = 0;

    // Iterate over a range of p
    for (int p = 0; p < 200; ++p) { // Adjust the range as needed
        mpz_set_ui(two_power_p, 1);
        mpz_mul_2exp(two_power_p, two_power_p, p); // 2^p

        mpz_tdiv_q(C, target_value, two_power_p); // C = target_value / 2^p
        mpz_mul(temp, C, two_power_p); // C * 2^p
        mpz_sub(temp_diff, target_value, temp); // Difference = target_value - (C * 2^p)
        mpz_abs(temp_diff, temp_diff); // Absolute difference

        // Check if C <= p and if this approximation is better
        if (mpz_cmp_ui(C, p) <= 0 && mpz_cmp(temp_diff, min_diff) < 0) {
            mpz_set(min_diff, temp_diff);
            mpz_set(best_value, temp);
            best_p = p;
            mpz_set(best_C, C);
        }
    }

    std::cout << "Best approximation (C * 2^p): ";
    gmp_printf("%Zd * 2^%d\n", best_C, best_p);
    std::cout << "C: ";
    gmp_printf("%Zd\n", best_C);
    std::cout << "p: " << best_p << std::endl;

    mpz_clear(two_power_p);
    mpz_clear(temp);
    mpz_clear(temp_diff);
    mpz_clear(min_diff);
    mpz_clear(C);
}




// Function to minimize the given expression
void minimize_expression(const char* target_str, int P) {
    mpz_t target_value, best_value, diff_value, remaining_value, best_C;
    mpz_init_set_str(target_value, target_str, 10);
    mpz_init(best_value);
    mpz_init(diff_value);
    mpz_init(remaining_value);
    mpz_init(best_C);

    // Find the best approximation C * 2^p
    int best_p;
    find_best_approximation(target_value, best_value, best_p, best_C);

    // Subtract best approximation from target value
    mpz_sub(remaining_value, target_value, best_value);

    // Convert remaining_value to mpfr_t for calculations
    mpfr_t log_remaining_value;
    mpfr_init2(log_remaining_value, 256);
    mpfr_set_z(log_remaining_value, remaining_value, MPFR_RNDN);
    mpfr_log(log_remaining_value, log_remaining_value, MPFR_RNDN);

    // Generate the first P primes
    std::vector<int> primes = generate_primes(P);

    // Calculate upper bounds for the variables
    std::vector<int> upper_bounds = calculate_upper_bounds(remaining_value, primes);

    // Initialize GLPK problem
    glp_prob* lp;
    lp = glp_create_prob();

    glp_set_prob_name(lp, "minimize_expression");
    glp_set_obj_dir(lp, GLP_MIN);

    // Add P variables for the primes
    glp_add_cols(lp, P);

    // Set up the variables and objective coefficients
    for (int i = 0; i < P; ++i) {
        std::string var_name = "x" + std::to_string(i + 1);
        glp_set_col_name(lp, i + 1, var_name.c_str());
        glp_set_col_bnds(lp, i + 1, GLP_DB, 0.0, upper_bounds[i]);
        glp_set_col_kind(lp, i + 1, GLP_IV); // Set variable as an integer
        glp_set_obj_coef(lp, i + 1, std::log(primes[i]));
    }

    // Add constraint ln(k) <= sum(xi * ln(primes[i]))
    glp_add_rows(lp, 1);
    glp_set_row_name(lp, 1, "constraint");
    glp_set_row_bnds(lp, 1, GLP_LO, mpfr_get_d(log_remaining_value, MPFR_RNDN), 0.0);

    std::vector<int> ia(P + 1);
    std::vector<int> ja(P + 1);
    std::vector<double> ar(P + 1);

    for (int i = 0; i < P; ++i) {
        ia[i + 1] = 1;
        ja[i + 1] = i + 1;
        ar[i + 1] = std::log(primes[i]);
    }

    glp_load_matrix(lp, P, ia.data(), ja.data(), ar.data());

    // Solve the problem
    glp_iocp parm;
    glp_init_iocp(&parm);
    parm.presolve = GLP_ON;
    glp_intopt(lp, &parm);

    // Set tolerance settings
    parm.tol_int = 1e-9; // Integer feasibility tolerance
    parm.tol_obj = 1e-9; // Optimality tolerance




    // Get the results
    std::vector<mpz_t> results(P);
    for (int i = 0; i < P; ++i) {
        mpz_init(results[i]);
        mpz_set_d(results[i], glp_mip_col_val(lp, i + 1));
    }

    std::cout << "Results:" << std::endl;
    for (int i = 0; i < P; ++i) {
        std::cout << "x" << i + 1 << " (prime " << primes[i] << "): " << mpz_get_d(results[i]) << std::endl;
    }

    // Calculate the final calculated value
    mpz_t calculated_value, prime_power;
    mpz_init(calculated_value);
    mpz_set_ui(calculated_value, 1);

    for (int i = 0; i < P; ++i) {
        mpz_init(prime_power);
        mpz_ui_pow_ui(prime_power, primes[i], mpz_get_ui(results[i]));
        mpz_mul(calculated_value, calculated_value, prime_power);
        mpz_clear(prime_power);
    }

    // Calculate the final value
    mpz_t final_value, difference;
    mpz_init(final_value);
    mpz_set_ui(final_value, 0);
    mpz_mul_2exp(final_value, best_value, best_p); // final_value = C * 2^p
    mpz_add(final_value, final_value, calculated_value); // final_value = C * 2^p + calculated_value

    mpz_init(difference);
    mpz_sub(difference, target_value, final_value);

    // Print results
    gmp_printf("Target value: %Zd\n", target_value);
    gmp_printf("Best approximation (C * 2^p): %Zd * 2^%d\n", best_C, best_p);
    gmp_printf("Remaining value after best approximation: %Zd\n", remaining_value);
    gmp_printf("Calculated value: %Zd\n", calculated_value);
    gmp_printf("Final value (C * 2^p + calculated value): %Zd\n", final_value);
    gmp_printf("Difference: %Zd\n", difference);

    mpfr_clears(log_remaining_value, (mpfr_ptr)0);
    for (auto &res : results) {
        mpz_clear(res);
    }
    mpz_clears(target_value, best_value, remaining_value, best_C, calculated_value, final_value, difference, (mpz_ptr)0);

    // Free GLPK problem
    glp_delete_prob(lp);
}


int main() {
    const char* target_value = "123456789123456789";
    int P = 100; // Example: use the first 3 primes
    minimize_expression(target_value, P);
    return 0;
}
