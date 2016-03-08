#include "common.h"
#include <iostream>

using namespace std;
using namespace rating;

gsl_vector* rating::solve(const gsl_matrix* A, const gsl_vector* b)
{
    gsl_vector* x = gsl_vector_alloc(A->size2);
    gsl_matrix* A_ = gsl_matrix_alloc(A->size1, A->size2);
    gsl_matrix_memcpy(A_, A);
    
    int signum;
    gsl_permutation* permu = gsl_permutation_alloc(A->size1); 
    gsl_linalg_LU_decomp(A_, permu, &signum);
    gsl_linalg_LU_solve(A_, permu, b, x);

    gsl_permutation_free(permu);
    gsl_matrix_free(A_);

    //cout << "A:" << endl;
    //for(size_t i = 0; i < A->size1; ++i)
    //{
    //    for(size_t j = 0; j < A->size2; ++j)
    //    {
    //        cout << gsl_matrix_get(A, i, j) << " ";
    //    }
    //    cout << endl;
    //}

    //cout << "b:" << endl;
    //for(size_t i = 0; i < b->size; ++i)
    //{
    //    cout << gsl_vector_get(b, i) << " ";
    //}
    //cout << endl;

    return x;

}
