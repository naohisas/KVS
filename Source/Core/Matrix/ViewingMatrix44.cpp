/*****************************************************************************/
/**
 *  @file   ViewingMatrix.cpp
 *  @author Naohisa Sakamoto
 */
/*****************************************************************************/
#include "ViewingMatrix44.h"

namespace kvs
{

template kvs::Matrix44<float> ViewingMatrix44<float>(
    const kvs::Vector3<float>& c,
    const kvs::Vector3<float>& u,
    const kvs::Vector3<float>& g );

template kvs::Matrix44<double> ViewingMatrix44<double>(
    const kvs::Vector3<double>& c,
    const kvs::Vector3<double>& u,
    const kvs::Vector3<double>& g );

} // end of namespace kvs
