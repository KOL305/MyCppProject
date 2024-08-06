#include <sstream> // Include for std::ostringstream
#include <iostream>
#include <glpk.h>

std::string intToString(int i) {
    std::ostringstream oss;
    oss << i;
    return oss.str();
}

int main() {
    // Initialize the GLPK problem
    glp_prob *lp;
    lp = glp_create_prob();
    glp_set_prob_name(lp, "qp_test");
    glp_set_obj_dir(lp, GLP_MIN); // Minimize the objective function

    // Define the problem dimensions
    int num_vars = 3;
    int num_cons = 2;
    
    // Add columns (variables)
    glp_add_cols(lp, num_vars);
    for (int i = 1; i <= num_vars; ++i) {
        glp_set_col_name(lp, i, ("x" + intToString(i)).c_str());
        glp_set_col_bnds(lp, i, GLP_LO, 0.0, 0.0); // x >= 0
    }
    
    // Add rows (constraints)
    glp_add_rows(lp, num_cons);
    glp_set_row_name(lp, 1, "c1");
    glp_set_row_bnds(lp, 1, GLP_UP, 0.0, 10.0); // c1 <= 10
    glp_set_row_name(lp, 2, "c2");
    glp_set_row_bnds(lp, 2, GLP_UP, 0.0, 20.0); // c2 <= 20

    // Define the matrix for the constraints
    int ia[] = {0, 1, 1, 2, 2};
    int ja[] = {0, 1, 2, 1, 3};
    double ar[] = {0, 2.0, 3.0, 1.0, 4.0};

    glp_load_matrix(lp, 4, ia, ja, ar);

    // Define quadratic objective function coefficients
    glp_set_obj_coef(lp, 1, 0.5); // 0.5 * x1^2
    glp_set_obj_coef(lp, 2, 0.5); // 0.5 * x2^2
    glp_set_obj_coef(lp, 3, 0.5); // 0.5 * x3^2

    // Set linear coefficients
    glp_set_obj_coef(lp, 1, 1.0); // x1
    glp_set_obj_coef(lp, 2, 1.0); // x2
    glp_set_obj_coef(lp, 3, 1.0); // x3

    // Solve the problem using the quadratic solver
    glp_qp(lp, NULL);

    // Retrieve and display results
    std::cout << "Objective value: " << glp_get_obj_val(lp) << std::endl;
    for (int i = 1; i <= num_vars; ++i) {
        std::cout << "x" << i << " = " << glp_get_col_prim(lp, i) << std::endl;
    }

    // Clean up
    glp_delete_prob(lp);

    return 0;
}
