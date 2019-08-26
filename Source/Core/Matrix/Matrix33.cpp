/****************************************************************************/
/**
 *  @file   Matrix33.cpp
 *  @author Naohisa Sakamoto
 */
/*----------------------------------------------------------------------------
 *
 *  Copyright (c) Visualization Laboratory, Kyoto University.
 *  All rights reserved.
 *  See http://www.viz.media.kyoto-u.ac.jp/kvs/copyright/ for details.
 *
 *  $Id: Matrix33.cpp 602 2010-08-19 02:43:34Z naohisa.sakamoto $
 */
/****************************************************************************/
#include "Matrix33.h"


namespace kvs
{

// Template instantiation.
// template class Matrix33<char>;
// template class Matrix33<unsigned char>;
// template class Matrix33<short>;
// template class Matrix33<unsigned short>;
// template class Matrix33<int>;
// template class Matrix33<unsigned int>;
// template class Matrix33<long>;
// template class Matrix33<unsigned long>;
template class Matrix33<float>;
template class Matrix33<double>;

} // end of namespace kvs
