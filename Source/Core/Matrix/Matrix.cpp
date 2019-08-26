/****************************************************************************/
/**
 *  @file   Matrix.cpp
 *  @author Naohisa Sakamoto
 */
/*----------------------------------------------------------------------------
 *
 *  Copyright (c) Visualization Laboratory, Kyoto University.
 *  All rights reserved.
 *  See http://www.viz.media.kyoto-u.ac.jp/kvs/copyright/ for details.
 *
 *  $Id: Matrix.cpp 602 2010-08-19 02:43:34Z naohisa.sakamoto $
 */
/****************************************************************************/
#include "Matrix.h"


namespace kvs
{

// Template instantiation.
// template class Matrix<char>;
// template class Matrix<unsigned char>;
// template class Matrix<short>;
// template class Matrix<unsigned short>;
// template class Matrix<int>;
// template class Matrix<unsigned int>;
// template class Matrix<long>;
// template class Matrix<unsigned long>;
template class Matrix<float>;
template class Matrix<double>;

} // end of namespace kvs
