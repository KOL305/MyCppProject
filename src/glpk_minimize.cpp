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

// Function to minimize the given expression
void minimize_expression(const char *target_str, int P) {
    mpz_t target_value;
    mpz_init_set_str(target_value, target_str, 10);

    // Convert target_value to mpfr_t for calculations
    mpfr_t log_target_value;
    mpfr_init2(log_target_value, 256);
    mpfr_set_z(log_target_value, target_value, MPFR_RNDN);
    mpfr_log(log_target_value, log_target_value, MPFR_RNDN);

    // Generate the first P primes
    std::vector<int> primes = generate_primes(P);

    // Calculate upper bounds for the variables
    std::vector<int> upper_bounds = calculate_upper_bounds(target_value, primes);

    // Initialize GLPK problem
    glp_prob *lp;
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
    glp_set_row_bnds(lp, 1, GLP_LO, mpfr_get_d(log_target_value, MPFR_RNDN), 0.0);

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

    // Calculate the difference
    mpz_t calculated_value, prime_power;
    mpz_init(calculated_value);
    mpz_set_ui(calculated_value, 1);

    for (int i = 0; i < P; ++i) {
        mpz_init(prime_power);
        mpz_ui_pow_ui(prime_power, primes[i], mpz_get_ui(results[i]));
        mpz_mul(calculated_value, calculated_value, prime_power);
        mpz_clear(prime_power);
    }

    mpz_t difference;
    mpz_init(difference);
    mpz_sub(difference, target_value, calculated_value);

    // Print results
    gmp_printf("Target value: %Zd\n", target_value);
    gmp_printf("Calculated value: %Zd\n", calculated_value);
    gmp_printf("Difference: %Zd\n", difference);

    mpfr_clears(log_target_value, (mpfr_ptr) 0);
    for (auto &res : results) {
        mpz_clear(res);
    }
    mpz_clears(target_value, calculated_value, difference, (mpz_ptr) 0);

    // Free GLPK problem
    glp_delete_prob(lp);
}

int main() {
    const char *target_value = "123456789123456789";
    int P = 100; // Example: use the first 100 primes
    minimize_expression(target_value, P);
    return 0;
}