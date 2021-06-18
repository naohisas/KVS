/*****************************************************************************/
/**
 *  @file   OrthogonalMatrix44.cpp
 *  @author Naohisa Sakamoto
 */
/*****************************************************************************/
#include "OrthogonalMatrix44.h"


namespace kvs
{

template kvs::Matrix44<float> OrthogonalMatrix44<float>(
    const float left,
    const float right,
    const float bottom,
    const float top,
    const float near,
    const float far );

template kvs::Matrix44<double> OrthogonalMatrix44<double>(
    const double left,
    const double right,
    const double bottom,
    const double top,
    const double near,
    const double far );

} // end of namespace kvs
