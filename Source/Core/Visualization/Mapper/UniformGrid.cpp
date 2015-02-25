/*****************************************************************************/
/**
 *  @file   UniformGrid.cpp
 *  @author Naohisa Sakamoto
 */
/*----------------------------------------------------------------------------
 *
 *  Copyright (c) Visualization Laboratory, Kyoto University.
 *  All rights reserved.
 *  See http://www.viz.media.kyoto-u.ac.jp/kvs/copyright/ for details.
 *
 *  $Id$
 */
/*****************************************************************************/
#include "UniformGrid.h"


namespace kvs
{

UniformGrid::UniformGrid( const kvs::StructuredVolumeObject* volume ):
    kvs::GridBase( volume )
{
    // Set the initial interpolation functions and differential functions.
    BaseClass::updateInterpolationFunctions( BaseClass::localPoint() );
    BaseClass::updateDifferentialFunctions( BaseClass::localPoint() );
}

int UniformGrid::findGrid( const kvs::Vec3& global ) const
{
    const kvs::Vec3ui& dim = BaseClass::referenceVolume()->resolution();
    if ( global.x() < 0 || global.x() >= dim.x() - 1 ) { return -1; }
    if ( global.y() < 0 || global.y() >= dim.y() - 1 ) { return -1; }
    if ( global.z() < 0 || global.z() >= dim.z() - 1 ) { return -1; }

    const size_t i = static_cast<size_t>( global.x() );
    const size_t j = static_cast<size_t>( global.y() );
    const size_t k = static_cast<size_t>( global.z() );
    return BaseClass::gridIndexOf( kvs::Vec3ui( i, j, k ) );
}

kvs::Vec3 UniformGrid::globalToLocal( const kvs::Vec3& global ) const
{
    KVS_ASSERT( this->findGrid( global ) >= 0 );

    const kvs::Real32 p = global.x() - BaseClass::baseIndex().x();
    const kvs::Real32 q = global.y() - BaseClass::baseIndex().y();
    const kvs::Real32 r = global.z() - BaseClass::baseIndex().z();
    KVS_ASSERT( p <= 1.0 && q <= 1.0 && r <= 1.0 );

    return kvs::Vec3( p, q, r );
}

} // end of namespace kvs
