/*****************************************************************************/
/**
 *  @file   PerspectiveMatrix44.cpp
 *  @author Naohisa Sakamoto
 */
/*****************************************************************************/
#include "PerspectiveMatrix44.h"

namespace kvs
{

template kvs::Matrix44<float> PerspectiveMatrix44<float>(
    const float fov,
    const float aspect,
    const float near,
    const float far );

template kvs::Matrix44<double> PerspectiveMatrix44<double>(
    const double fov,
    const double aspect,
    const double near,
    const double far );

} // end of namespace kvs
