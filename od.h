/*==========================================
# File Name:		od.h
# Description:		offensive & defensive rating
# Author:			Feng Gao <gaofeng@ict.ac.cn>
# Create Date:		2016-03-04
# Last Modified:	2016-03-04
==========================================*/

#ifndef OD_H
#define OD_H

#include "common.h"
#include <utility>

namespace rating
{

class OD
{
    static const double ACCURACY; // default 1e-3
    gsl_vector* m_o;
    gsl_vector* m_d;
public:
    static std::pair<gsl_vector*, gsl_vector*> rate(gsl_matrix* offensiveScores, gsl_matrix* defensiveScores);

private:
    OD() = default;
    static bool isConvergence(const gsl_vector* oldVal, const gsl_vector* newVal);
    static void calculate(gsl_matrix* m, gsl_vector* src, gsl_vector* dest);
    void calculate_d(gsl_matrix* defensiveScores);
    void calculate_o(gsl_matrix* offensiveScores);

}; // class OD

}; // namespace rating


#endif  /*OD_H*/
