#include "massey.h"
#include "colley.h"
#include "od.h"
#include <iostream>

using namespace std;
using namespace rating;

int main(int argc, char** argv)
{
    double scores_data[] = {
        0.0, -45.0, -3.0, -31.0, -45,
        45.0, 0.0, 18.0, 8.0, 20.0,
        3.0, -18.0, 0.0, 2.0, -27,
        31.0, -8.0, -2.0, 0.0, -38.0,
        45.0, -20.0, 27.0, 38.0, 0.0
    }; 

    double offensiveScores_data[] = {
        0.0, 7.0, 21.0, 7.0, 0.0,
        52.0, 0.0, 34.0, 25.0, 27.0,
        24.0, 16.0, 0.0, 7.0, 3.0,
        38.0, 17.0, 5.0, 0.0, 14.0,
        45.0, 7.0, 30.0, 52.0, 0.0
    };

    double defensiveScores_data[] = {
        0.0, 52.0, 24.0, 38.0, 45.0,
        7.0, 0.0, 16.0, 17.0, 7.0,
        21.0, 34.0, 0.0, 5.0, 30.0,
        7.0, 25.0, 7.0, 0.0, 52.0,
        0.0, 27.0, 3.0, 14.0, 0.0
    };

    // Massey::rate test
    gsl_matrix_view scores = gsl_matrix_view_array (scores_data, 5, 5);
    gsl_vector* r = Massey::rate(&scores.matrix);

    cout << "=============test Massey::rate======================" << endl;
    for(size_t i = 0; i < r->size; ++i)
    {
        cout << gsl_vector_get(r, i) << " ";
    }
    cout << endl;

    gsl_vector_free(r);

    // Massey::rateOD test
    gsl_matrix_view offensiveScores = gsl_matrix_view_array(offensiveScores_data, 5, 5);
    gsl_matrix_view defensiveScores = gsl_matrix_view_array(defensiveScores_data, 5, 5);

    pair<gsl_vector*, gsl_vector*> od = Massey::rateOD(&offensiveScores.matrix, &defensiveScores.matrix);

    cout << "=============test Massey::rateOD=============="  << endl;
    for(size_t i = 0; i < od.first->size; ++i)
    {
        cout << gsl_vector_get(od.first, i) << " ";
    }
    cout << endl;

    for(size_t i = 0; i < od.second->size; ++i)
    {
        cout << gsl_vector_get(od.second, i) << " ";
    }
    cout << endl;

    gsl_vector_free(od.first);
    gsl_vector_free(od.second);

    // Colley::rate test
    double winOrLose_data[] = {
        0.0, -1.0, -1.0, -1.0, -1.0,
        1.0, 0.0, 1.0, 1.0, 1.0,
        1.0, -1.0, 0.0, 1.0, -1.0,
        1.0, -1.0, -1.0, 0.0, -1.0,
        1.0, -1.0, 1.0, 1.0, 0.0
    };

    gsl_matrix_view winOrLose = gsl_matrix_view_array(winOrLose_data, 5, 5);

    r = Colley::rate(&winOrLose.matrix);
    cout << "=================test Colley::rate==============" << endl; 
    for(size_t i = 0; i < r->size; ++i)
    {
        cout << gsl_vector_get(r, i) << " ";
    }
    cout << endl;

    gsl_vector_free(r);

    // OD::reate test
    od = OD::rate(&offensiveScores.matrix, &defensiveScores.matrix);
    cout << "=============test OD::rate=============="  << endl;
    for(size_t i = 0; i < od.first->size; ++i)
    {
        cout << gsl_vector_get(od.first, i) << " ";
    }
    cout << endl;

    for(size_t i = 0; i < od.second->size; ++i)
    {
        cout << gsl_vector_get(od.second, i) << " ";
    }
    cout << endl;

    gsl_vector_free(od.first);
    gsl_vector_free(od.second);

    // revised OD::rate test
    cout << "=============test OD::rate revised=============="  << endl;
    gsl_matrix* offensiveScoresRevised = gsl_matrix_alloc(offensiveScores.matrix.size1, offensiveScores.matrix.size2);
    gsl_matrix* defensiveScoresRevised = gsl_matrix_alloc(defensiveScores.matrix.size1, defensiveScores.matrix.size2);
    gsl_matrix_memcpy(offensiveScoresRevised, &offensiveScores.matrix);
    gsl_matrix_set(offensiveScoresRevised, 0, 4, 0.1);
    gsl_matrix_transpose_memcpy(defensiveScoresRevised, offensiveScoresRevised);

    od = OD::rate(offensiveScoresRevised, defensiveScoresRevised);
    for(size_t i = 0; i < od.first->size; ++i)
    {
        cout << gsl_vector_get(od.first, i) << " ";
    }
    cout << endl;

    for(size_t i = 0; i < od.second->size; ++i)
    {
        cout << gsl_vector_get(od.second, i) << " ";
    }
    cout << endl;

    gsl_matrix_free(offensiveScoresRevised);
    gsl_matrix_free(defensiveScoresRevised);
    

    return 0;
}
