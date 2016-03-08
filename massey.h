/*==========================================
# File Name:		massey.h
# Description:		Massey rating algorithm, including offensive rating and defensive rating
# Author:			Feng Gao <gaofeng@ict.ac.cn>
# Create Date:		2016-03-02
# Last Modified:	2016-03-02
==========================================*/
#ifndef MASSEY_H
#define MASSEY_H

#include "common.h"
#include <utility>

namespace rating
{

class Massey
{
    // for normal rating
    gsl_matrix* m_M{nullptr};
    gsl_vector* m_p{nullptr};
    gsl_vector* m_r{nullptr};
    
    // for offensive-defensive rating
    gsl_matrix* m_T{nullptr};
    gsl_matrix* m_P{nullptr};
    gsl_vector* m_o{nullptr};
    gsl_vector* m_d{nullptr};
    gsl_vector* m_f{nullptr};
    gsl_vector* m_a{nullptr};

public:
    static gsl_vector* rate(const gsl_matrix* scores);

    static std::pair<gsl_vector*,gsl_vector*> rateOD(const gsl_matrix* offensiveScores, const gsl_matrix* defensiveScores);

private:
    static gsl_vector* accumulateRow(const gsl_matrix* m);

private:
    Massey() = default;
    
    gsl_matrix* get_M(size_t dimension);
    gsl_vector* get_p(const gsl_matrix* scores);

    gsl_vector* solve_r();

    gsl_matrix* get_T(size_t dimension);
    gsl_matrix* get_P(size_t dimension);
    gsl_vector* get_f(const gsl_matrix* offensiveScores);
    gsl_vector* get_a(const gsl_matrix* defensiveScores);

    gsl_vector* solve_p(void);

    gsl_vector* solve_d();

    gsl_vector* solve_o();

    //void clean_all();

}; // class Massey

}; // namespace rating


#endif  /*MASSEY_H*/
