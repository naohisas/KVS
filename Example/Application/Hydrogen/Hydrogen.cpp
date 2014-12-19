/*****************************************************************************/
/**
 *  @file   Hydrogen.cpp
 *  @author Naohisa Sakamoto
 */
/*----------------------------------------------------------------------------
 *
 *  Copyright (c) Visualization Laboratory, Kyoto University.
 *  All rights reserved.
 *  See http://www.viz.media.kyoto-u.ac.jp/kvs/copyright/ for details.
 *
 *  $Id: Hydrogen.cpp 1094 2012-04-18 05:06:00Z s.yamada0808@gmail.com $
 */
/*****************************************************************************/
#include "Hydrogen.h"
#include <kvs/AnyValueArray>
#include <kvs/Vector3>


namespace
{

/*===========================================================================*/
/**
 *  @brief  Generates hydrogen volume.
 *  @param  resolution [in] volume resolution
 *  @return value array of the hydrogen volume
 */
/*===========================================================================*/
inline kvs::AnyValueArray GenerateVolume(
    const kvs::Vector3ui& resolution )
{
    const kvs::UInt32 dim1 = resolution.x();
    const kvs::UInt32 dim2 = resolution.y();
    const kvs::UInt32 dim3 = resolution.z();

    const kvs::Real64 kr1 = 32.0 / dim1;
    const kvs::Real64 kr2 = 32.0 / dim2;
    const kvs::Real64 kr3 = 32.0 / dim3;
    const kvs::Real64 kd = 6.0;

    kvs::ValueArray<kvs::UInt8> data( dim1 * dim2 * dim3 );

    kvs::UInt8* pdata = data.data();
    kvs::UInt64 index = 0;
    for ( kvs::UInt32 z = 0; z < dim3; ++z )
    {
        const kvs::Real64 dz = kr3 * ( z - ( dim3 / 2.0 ) );
        for ( kvs::UInt32 y = 0; y < dim2; ++y )
        {
            const kvs::Real64 dy = kr2 * ( y - ( dim2 / 2.0 ) );
            for ( kvs::UInt32 x = 0; x < dim1; ++x )
            {
                const kvs::Real64 dx = kr1 * ( x - ( dim1 / 2.0 ));
                const kvs::Real64 r = std::sqrt( dx * dx + dy * dy + dz * dz );
                const kvs::Real64 cos_theta = dz / r;
                const kvs::Real64 phi = kd * ( r*r ) * std::exp( -r/2 ) * ( 3*cos_theta*cos_theta-1 );

                kvs::Real64 c = phi * phi;
                if ( c > 255.0 ) { c = 255.0; }

                pdata[ index++ ] = static_cast<kvs::UInt8>( c );
            }
        }
    }

    return kvs::AnyValueArray( data );
}

} // end of namespace

/*===========================================================================*/
/**
 *  @brief  Constructs a new hydrogen class.
 *  @param  resolution [in] volume resolution
 */
/*===========================================================================*/
Hydrogen::Hydrogen( const kvs::Vector3ui& resolution )
{
    SuperClass::setGridType( kvs::StructuredVolumeObject::Uniform );
    SuperClass::setResolution( resolution );
    SuperClass::setVeclen( 1 );
    SuperClass::setValues( ::GenerateVolume( resolution ) );
    SuperClass::updateMinMaxCoords();
    SuperClass::updateMinMaxValues();
}

/*===========================================================================*/
/**
 *  @brief  Destructs the Hydrogen class.
 */
/*===========================================================================*/
Hydrogen::~Hydrogen( void )
{
}

/*===========================================================================*/
/**
 *  @brief  Constructs a new Hydrogen class.
 */
/*===========================================================================*/
Hydrogen::Hydrogen( void )
{
}
