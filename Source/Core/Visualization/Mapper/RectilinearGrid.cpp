/*****************************************************************************/
/**
 *  @file   RectilinearGrid.cpp
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
#include "RectilinearGrid.h"


namespace
{

int BinarySearch( const float v, const float* array, const size_t size )
{
    int low = 0;
    int middle = 0;
    int high = size - 1;

    while ( low <= high )
    {
        middle = ( low + high ) / 2;

        if ( array[ middle ] <= v )
        {
            if ( array[ middle + 1 ] > v ) { return middle; }
            else { low = middle + 1; }
        }
        else if ( array[ middle ] > v )
        {
            if ( array[ middle - 1 ] <= v ) { return middle - 1; }
            else { high = middle - 1; }
        }
        else { low = middle + 1; }
    }

    return -1;
}

}

namespace kvs
{

RectilinearGrid::RectilinearGrid( const kvs::StructuredVolumeObject* volume ):
    kvs::GridBase( volume )
{
    // Set the initial interpolation functions and differential functions.
    BaseClass::updateInterpolationFunctions( BaseClass::localPoint() );
    BaseClass::updateDifferentialFunctions( BaseClass::localPoint() );
}

int RectilinearGrid::findGrid( const kvs::Vec3& global ) const
{
    const kvs::Vec3& min_obj = BaseClass::referenceVolume()->minObjectCoord();
    const kvs::Vec3& max_obj = BaseClass::referenceVolume()->maxObjectCoord();
    if ( global.x() < min_obj.x() || global.x() >= max_obj.x() ) { return -1; }
    if ( global.y() < min_obj.y() || global.y() >= max_obj.y() ) { return -1; }
    if ( global.z() < min_obj.z() || global.z() >= max_obj.z() ) { return -1; }

    const size_t dimx = BaseClass::referenceVolume()->resolution().x();
    const size_t dimy = BaseClass::referenceVolume()->resolution().y();
    const size_t dimz = BaseClass::referenceVolume()->resolution().z();
    const float* xcoords = BaseClass::referenceVolume()->coords().data();
    const float* ycoords = BaseClass::referenceVolume()->coords().data() + dimx;
    const float* zcoords = BaseClass::referenceVolume()->coords().data() + dimx + dimy;

    const int i = ::BinarySearch( global.x(), xcoords, dimx );
    const int j = ::BinarySearch( global.y(), ycoords, dimy );
    const int k = ::BinarySearch( global.z(), zcoords, dimz );
    return BaseClass::gridIndexOf( kvs::Vec3ui( i, j, k ) );
}

kvs::Vec3 RectilinearGrid::globalToLocal( const kvs::Vec3& global ) const
{
    KVS_ASSERT( this->findGrid( global ) >= 0 );

    const size_t dimx = BaseClass::referenceVolume()->resolution().x();
    const size_t dimy = BaseClass::referenceVolume()->resolution().y();
    const float* xcoords = BaseClass::referenceVolume()->coords().data();
    const float* ycoords = BaseClass::referenceVolume()->coords().data() + dimx;
    const float* zcoords = BaseClass::referenceVolume()->coords().data() + dimx + dimy;

    const float x = xcoords[ BaseClass::baseIndex().x() ];
    const float y = ycoords[ BaseClass::baseIndex().y() ];
    const float z = zcoords[ BaseClass::baseIndex().z() ];
    const float dx = xcoords[ BaseClass::baseIndex().x() + 1 ] - x;
    const float dy = ycoords[ BaseClass::baseIndex().y() + 1 ] - y;
    const float dz = zcoords[ BaseClass::baseIndex().z() + 1 ] - z;
    const kvs::Vec3 local = ( global - kvs::Vec3( x, y, z ) ) / kvs::Vec3( dx, dy, dz );
    KVS_ASSERT( local.x() >= 0.0 && local.y() >= 0.0 && local.z() >= 0.0 );
    KVS_ASSERT( local.x() <= 1.0 && local.y() <= 1.0 && local.z() <= 1.0 );

    return local;
}

} // end of namespace kvs
