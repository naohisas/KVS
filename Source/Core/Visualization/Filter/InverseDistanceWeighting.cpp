/*****************************************************************************/
/**
 *  @file   InverseDistanceWeighting.cpp
 *  @author Naohisa Sakamoto
 */
/*****************************************************************************/
#include "InverseDistanceWeighting.h"


namespace kvs
{

template class InverseDistanceWeighting<kvs::Real32>;
template class InverseDistanceWeighting<kvs::Vec3>;
template class InverseDistanceWeighting<kvs::Mat3>;

} // end of namespace kvs
