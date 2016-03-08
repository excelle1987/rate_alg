/*==========================================
# File Name:		colley.h
# Description:		Colley rating algorithm
# Author:			Feng Gao <gaofeng@ict.ac.cn>
# Create Date:		2016-03-03
# Last Modified:	2016-03-03
==========================================*/

#ifndef COLLEY_H
#define COLLEY_H

#include "common.h"

namespace rating
{

class Colley
{
    Colley() = delete;
    ~Colley() = delete;
public:
    static gsl_vector* rate(gsl_matrix* winOrLose);
}; // class Colley

}; // namespace rating

#endif  /*COLLEY_H*/
