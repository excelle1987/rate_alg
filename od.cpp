#include "od.h"
#include <iostream>
#include <cmath>

using namespace rating;
using namespace std;

const double OD::ACCURACY{1e-3};

std::pair<gsl_vector*, gsl_vector*> OD::rate(gsl_matrix* offensiveScores, gsl_matrix* defensiveScores)
{
    size_t dimension = offensiveScores->size1;

    OD inst;
    inst.m_o = gsl_vector_alloc(dimension);
    gsl_vector* o_old = gsl_vector_calloc(dimension);
    inst.m_d = gsl_vector_alloc(dimension);
    gsl_vector* d_old = gsl_vector_calloc(dimension);

    gsl_vector_set_all(inst.m_o, 1.0);
    gsl_vector_set_all(inst.m_d, 1.0);


    bool isChanged = true;
    while(isChanged)
    {
        isChanged = false;
        if( ! isConvergence(o_old, inst.m_o) )
        {   
            // archive o
            gsl_vector_swap(o_old, inst.m_o);
            inst.calculate_o(offensiveScores);        

            // archive d
            gsl_vector_swap(d_old, inst.m_d);
            inst.calculate_d(defensiveScores);
            isChanged = true;
        }
    }
    gsl_vector_free(o_old);
    gsl_vector_free(d_old);

    return make_pair(inst.m_o, inst.m_d);
}


bool OD::isConvergence(const gsl_vector* oldVal, const gsl_vector* newVal)
{
    for(size_t i = 0; i < oldVal->size; ++i)
    {
        if(abs(gsl_vector_get(newVal,i) - gsl_vector_get(oldVal,i)) > ACCURACY)
        {
            return false;
        }
    }
    return true;
}

void OD::calculate(gsl_matrix* m, gsl_vector* src, gsl_vector* dest)
{
    for(size_t i = 0; i < m->size1; ++i)
    {
        double val = 0.0;
        for(size_t j = 0; j < m->size2; ++j)
        {
            val += (gsl_matrix_get(m, i, j) / gsl_vector_get(src, j));
        }
        gsl_vector_set(dest, i, val);
    }
}

void OD::calculate_o(gsl_matrix* offensiveScores)
{
    calculate(offensiveScores, m_d, m_o);
}

void OD::calculate_d(gsl_matrix* defensiveScores)
{
    calculate(defensiveScores, m_o, m_d); 
}
