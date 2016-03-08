#include "massey.h"
#include <iostream>

using namespace std;
using namespace rating;

gsl_vector* Massey::rate(const gsl_matrix* scores)
{
    // diagonal elements should be 0.0

    Massey inst;
    size_t dimension = scores->size1;
    inst.get_M(dimension);
    inst.get_p(scores);

    inst.solve_r();

    gsl_matrix_free(inst.m_M);
    gsl_vector_free(inst.m_p);

    return inst.m_r;
}

std::pair<gsl_vector*, gsl_vector*> Massey::rateOD(const gsl_matrix* offensiveScores, const gsl_matrix* defensiveScores)
{
    Massey inst;
    size_t dimension = offensiveScores->size1;
    inst.get_T(dimension);
    inst.get_P(dimension);
    inst.get_f(offensiveScores);
    inst.get_a(defensiveScores);

    // first we get M and p, then get r
    inst.get_M(dimension);
    inst.solve_p();
    inst.solve_r();

    // finally archive d and o = r - d;
    inst.solve_d();
    inst.solve_o();

    // free resources
    gsl_matrix_free(inst.m_T);
    gsl_matrix_free(inst.m_P);
    gsl_vector_free(inst.m_f);
    gsl_vector_free(inst.m_a);
    gsl_matrix_free(inst.m_M);
    gsl_vector_free(inst.m_p);
    gsl_vector_free(inst.m_r);

    return make_pair(inst.m_o, inst.m_d);
}

gsl_vector* Massey::accumulateRow(const gsl_matrix* m)
{
    gsl_vector* v = gsl_vector_alloc(m->size1);
    for(size_t i = 0; i < m->size1; ++i)
    {
        double accumu = 0.0;
        for(size_t j = 0; j < m->size2; ++j)
        {
            accumu += gsl_matrix_get(m, i, j);
        }
        gsl_vector_set(v, i, accumu);
    }
    return v;
}


gsl_matrix* Massey::get_M(size_t dimension)
{
    m_M = gsl_matrix_alloc(dimension, dimension); 
    gsl_matrix_set_all(m_M, -1.0);

    for(size_t i = 0; i < dimension; ++i)
    {
        gsl_matrix_set(m_M, i, i, dimension - 1.0);
    }

    return m_M;
}


gsl_vector* Massey::get_p(const gsl_matrix* scores)
{
    m_p = accumulateRow(scores);
    return m_p;
}

gsl_vector* Massey::solve_r()
{
    // store original value
    gsl_vector* backupM = gsl_vector_alloc(m_M->size2);
    gsl_matrix_get_row(backupM, m_M, 0);

    double backupp = gsl_vector_get(m_p, 0);
    // set Row 1 all 1.0
    for(size_t i = 0; i < m_M->size1; ++i)
    {
        gsl_matrix_set(m_M, 0, i, 1.0);
    }
    gsl_vector_set(m_p, 0, 0.0);

    // now we get \bar{M} and \bar{p}
    
    m_r = solve(m_M, m_p);

    // set original value
    gsl_matrix_set_row(m_M, 0, backupM);
    gsl_vector_set(m_p, 0, backupp);

    gsl_vector_free(backupM);

    return m_r;
}

gsl_matrix* Massey::get_T(size_t dimension)
{
    m_T = gsl_matrix_calloc(dimension, dimension);
    for(size_t i = 0; i < dimension; ++i)
    {
        gsl_matrix_set(m_T, i, i, dimension - 1.0);
    }
    return m_T;
}

gsl_matrix* Massey::get_P(size_t dimension)
{
    m_P = gsl_matrix_alloc(dimension, dimension);
    gsl_matrix_set_all(m_P, 1.0);

    for(size_t i = 0; i < dimension; ++i)
    {
        gsl_matrix_set(m_P, i, i, 0.0);
    }
    return m_P;
}

gsl_vector* Massey::get_f(const gsl_matrix* offensiveScores)
{
    m_f = accumulateRow(offensiveScores);
    return m_f;
}

gsl_vector* Massey::get_a(const gsl_matrix* defensiveScores)
{
    m_a = accumulateRow(defensiveScores);
    return m_a;
}

// when already archive f and a;
gsl_vector* Massey::solve_p(void)
{
    m_p = gsl_vector_alloc(m_T->size1);
    gsl_vector_memcpy(m_p, m_f);
    gsl_vector_sub(m_p, m_a);
    return m_p;
}

gsl_vector* Massey::solve_d()
{
    // A = (T + P)
    gsl_matrix* A = gsl_matrix_alloc(m_T->size1, m_T->size2);
    gsl_matrix_memcpy(A, m_T);
    gsl_matrix_add(A, m_P);

    // b = Tr - f
    gsl_vector* b = gsl_vector_alloc(m_f->size);
    gsl_vector_memcpy(b, m_f);

    // calculate b
    //gsl_blas_dgemv(CblasNoTrans, 1.0, m_T, m_r, -1.0, b);
    gsl_blas_dsymv(CblasUpper, 1.0, m_T, m_r, -1.0, b);

    // solve Ad = b, get d
    m_d = solve(A, b);

    gsl_vector_free(b);
    gsl_matrix_free(A);    

    return m_d;
}

gsl_vector* Massey::solve_o()
{
    m_o = gsl_vector_alloc(m_r->size);
    gsl_vector_memcpy(m_o, m_r);
    gsl_vector_sub(m_o, m_d);
    return m_o;
}

//void Massey::clean_all()
//{
//    if(m_M)
//        gsl_matrix_free(m_M);
//    if(m_p)
//        gsl_vector_free(p);
//    if(m_r)
//        gsl_vector_free(r);
//
//    if(m_T)
//        gsl_matrix_free(m_T);
//    if(m_P)
//        gsl_matrix_free(m_P);
//    if(m_o)
//        gsl_vector_free(m_o);
//    if(m_d)
//        gsl_vector_free(m_d);
//    if(m_f)
//        gsl_vector_free(m_f);
//    if(m_a)
//        gsl_vector_free(m_a);
//}


