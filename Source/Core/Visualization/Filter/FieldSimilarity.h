/*****************************************************************************/
/**
 *  @file   FieldSimilarity.h
 *  @author Go Tamura, Naohisa Sakamoto
 */
/*----------------------------------------------------------------------------
 *
 *  A. Fofonov, L. Linsen, Projected Field Similarity for Comparative Visualization
 *  of Multi-Run Multi-Field Time-Varying Spatial Data, Computer Graphics Forum,
 *  Volume 38, Issue 1, pp.286-299, 2019.
 */
/*****************************************************************************/
#pragma once
#include <kvs/VolumeObjectBase>


namespace kvs
{

float FieldSimilarity(
    const kvs::VolumeObjectBase& volume0,
    const kvs::VolumeObjectBase& volume1 );

float FieldSimilarity(
    const kvs::VolumeObjectBase& volume0,
    const kvs::VolumeObjectBase& volume1,
    const float min_value,
    const float max_value );

} // end of namespace kvs
