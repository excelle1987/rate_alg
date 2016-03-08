#ifndef COMMON_H
#define COMMON_H

#include <gsl/gsl_matrix.h>
#include <gsl/gsl_eigen.h>
#include <gsl/gsl_linalg.h>
#include <gsl/gsl_blas.h>

namespace rating
{

// Ax = b, return x
gsl_vector* solve(const gsl_matrix* A, const gsl_vector* b);


}; // namespace rating
#endif  /*COMMON_H*/
