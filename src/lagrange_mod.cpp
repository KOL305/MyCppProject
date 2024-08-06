#include <iostream>
#include <gmp.h>
#include <mpfr.h>
#include <glpk.h>

int main() {
    // Initialize GMP and MPFR variables
    mpz_t k, n1, n2, n3, Q;
    mpz_init(k);
    mpz_init(n1);
    mpz_init(n2);
    mpz_init(n3);
    mpz_init(Q);

    mpfr_t ln_k, ln2, j;
    mpfr_init(ln_k);
    mpfr_init(ln2);
    mpfr_init(j);

    // Set k and base for exponentiation
    mpz_set_str(k, "8388609", 10); // Example large k

    // Initialize MPFR values for ln(k) and ln(2)
    mpfr_const_log2(ln2, MPFR_RNDN); // ln(2) = log2(e)
    mpfr_set_z(ln_k, k, MPFR_RNDN); // Convert GMP integer to MPFR
    mpfr_log(ln_k, ln_k, MPFR_RNDN); // Compute ln(k)

    // Compute j = floor(ln(k) / ln(2))
    mpfr_div(j, ln_k, ln2, MPFR_RNDN);
    mpfr_floor(j, j); // Floor function

    // Convert MPFR result to GMP integer
    mpz_t j_gmp;
    mpz_init(j_gmp);
    mpfr_get_z(j_gmp, j, MPFR_RNDN); // Convert MPFR j to GMP j_gmp

    // Compute 2^j
    mpz_ui_pow_ui(Q, 2, mpz_get_ui(j_gmp)); // 2^j

    // Setup GLPK
    glp_prob *lp;
    lp = glp_create_prob();
    glp_set_prob_name(lp, "maximize_Q");

    // Define problem dimensions
    glp_add_rows(lp, 2); // Constraints
    glp_add_cols(lp, 4); // Variables

    // Set row names and bounds
    glp_set_row_name(lp, 1, "constraint1");
    glp_set_row_bnds(lp, 1, GLP_FX, mpz_get_d(k), mpz_get_d(k)); // k = n1^2 + n2^2 + n3^2 + Q
    glp_set_row_name(lp, 2, "constraint2");
    glp_set_row_bnds(lp, 2, GLP_FX, mpz_get_d(Q), mpz_get_d(Q)); // Q = 2^j

    // Define columns (variables n1, n2, n3, Q)
    glp_set_col_name(lp, 1, "n1");
    glp_set_col_bnds(lp, 1, GLP_LO, 0.0, 0.0); // n1 >= 0
    glp_set_col_name(lp, 2, "n2");
    glp_set_col_bnds(lp, 2, GLP_LO, 0.0, 0.0); // n2 >= 0
    glp_set_col_name(lp, 3, "n3");
    glp_set_col_bnds(lp, 3, GLP_LO, 0.0, 0.0); // n3 >= 0
    glp_set_col_name(lp, 4, "Q");
    glp_set_col_bnds(lp, 4, GLP_LO, 0.0, 0.0); // Q >= 0

    // Define the objective function: maximize Q
    glp_set_obj_dir(lp, GLP_MAX);
    glp_set_obj_coef(lp, 4, 1.0);

    // Set constraints
    // Constraint matrix
    int ia[7] = {1, 1, 1, 1, 2, 2, 2}; // Row indices (1-based)
    int ja[7] = {1, 2, 3, 4, 1, 2, 4}; // Column indices (1-based)
    double ar[7] = {1.0, 1.0, 1.0, 1.0, 0.0, 0.0, 1.0}; // Coefficients

    // Load matrix into GLPK
    glp_load_matrix(lp, 7, ia, ja, ar);

    // Solve the problem
    glp_simplex(lp, NULL);

    // Output results
    std::cout << "Maximum value of Q: " << glp_get_obj_val(lp) << std::endl;

    // Output the values of n1, n2, n3, Q, and j
    std::cout << "Values:" << std::endl;
    std::cout << "n1: " << glp_get_col_prim(lp, 1) << std::endl;
    std::cout << "n2: " << glp_get_col_prim(lp, 2) << std::endl;
    std::cout << "n3: " << glp_get_col_prim(lp, 3) << std::endl;
    std::cout << "Q: " << glp_get_col_prim(lp, 4) << std::endl;
    std::cout << "j: " << mpz_get_d(j_gmp) << std::endl;

    // Cleanup
    glp_delete_prob(lp);
    mpz_clear(k);
    mpz_clear(n1);
    mpz_clear(n2);
    mpz_clear(n3);
    mpz_clear(Q);
    mpfr_clear(ln_k);
    mpfr_clear(ln2);
    mpfr_clear(j);
    mpz_clear(j_gmp);

    return 0;
}
