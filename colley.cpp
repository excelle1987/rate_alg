#include "colley.h"
#include <iostream>

using namespace std;
using namespace rating;

// Cr = b, get r
gsl_vector* Colley::rate(gsl_matrix* winOrLose)
{
    // get C
    size_t teamNum = winOrLose->size1;
    gsl_matrix* C = gsl_matrix_alloc(teamNum, teamNum); 
    gsl_matrix_set_all(C, -1);
    for(size_t i = 0; i < teamNum; ++i)
    {
        gsl_matrix_set(C, i, i, teamNum + 1.0);
    }

    // get b
    gsl_vector* b = gsl_vector_alloc(teamNum);
    for(size_t i = 0; i < winOrLose->size1; ++i)
    {
        double matchSum = 0.0;
        for(size_t j = 0; j < winOrLose->size2; ++j)
        {
            matchSum += gsl_matrix_get(winOrLose, i, j);
        }
        gsl_vector_set(b, i, matchSum / 2 + 1);
    }

    gsl_vector* r = solve(C, b);

    gsl_vector_free(b);
    gsl_matrix_free(C);

    return r;
}



