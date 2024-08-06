#include <iostream>
#include <ilcplex/ilocplex.h>
#include <gmp.h>
#include <mpfr.h>

ILOSTLBEGIN

// Function to convert mpz_t to double
double mpz_to_double(const mpz_t& mpz_value) {
    return mpz_get_d(mpz_value);  // Convert mpz_t to double
}

int main() {
    IloEnv env;
    try {
        IloModel model(env);

        // Define variables
        IloNumVar n1(env, 0, IloInfinity, ILOINT);
        IloNumVar n2(env, 0, IloInfinity, ILOINT);
        IloNumVar n3(env, 0, IloInfinity, ILOINT);
        IloNumVar n4(env, 0, IloInfinity, ILOINT);
        IloNumVar Q(env, 0, IloInfinity, ILOINT);
        IloNumVar k_target(env, 8388609, IloInfinity, ILOINT);

        // Objective function: Maximize Q
        IloObjective objective = IloMaximize(env, Q);
        model.add(objective);

        // Compute j
        mpz_t k;
        mpz_init_set_str(k, "8388609", 10);
        double k_value = mpz_to_double(k);

        mpfr_t k_mpfr, log_k, j;
        mpfr_init(k_mpfr);
        mpfr_init(log_k);
        mpfr_init(j);

        // Convert k to mpfr
        mpfr_set_z(k_mpfr, k, MPFR_RNDN);
        mpfr_log2(log_k, k_mpfr, MPFR_RNDN);
        mpfr_floor(j, log_k); // Corrected to use two arguments

        // Set j as a fixed value
        double j_value = mpfr_get_d(j, MPFR_RNDN);
        mpfr_clear(k_mpfr);
        mpfr_clear(log_k);
        mpfr_clear(j);
        mpz_clear(k);

        // Constraints
        IloExpr sum(env);
        sum = IloPower(n1, 2) + IloPower(n2, 2) + IloPower(n3, 2) + Q;
        //model.add(k_value == sum);

        // model.add(n4 >= n3);
        // model.add(n4 >= n2);
        // model.add(n4 >= n1);
        model.add(Q == IloPower(2, j_value)); // Use j_value for power of 2
        // model.add(n1 <= n2);
        // model.add(n1 <= n3);

        // Clean up
        sum.end();

        // Solve the model
        IloCplex cplex(model);
        if (!cplex.solve()) {
            std::cerr << "Failed to solve the model!" << std::endl;
            return 1;
        }

        // Output results
        std::cout << "Optimal value of Q: " << cplex.getObjValue() << std::endl;
        std::cout << "N: " << j_value << std::endl;

    } catch (IloException& e) {
        std::cerr << "Error: " << e << std::endl;
        return 1;
    } catch (...) {
        std::cerr << "Unknown exception caught!" << std::endl;
        return 1;
    }

    env.end();
    return 0;
}
