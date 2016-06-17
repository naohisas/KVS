/*****************************************************************************/
/**
 *  @file   TornadoVolumeData.cpp
 *  @author Naohisa Sakamoto
 */
/*----------------------------------------------------------------------------
 *
 *  Copyright (c) Visualization Laboratory, Kyoto University.
 *  All rights reserved.
 *  See http://www.viz.media.kyoto-u.ac.jp/kvs/copyright/ for details.
 *
 *  $Id: TornadoVolumeData.cpp 1373 2012-11-30 03:26:51Z naohisa.sakamoto@gmail.com $
 */
/*****************************************************************************/
#include "TornadoVolumeData.h"
#include <kvs/AnyValueArray>
#include <kvs/Vector3>


namespace kvs
{

/*===========================================================================*/
/**
 *  @brief  Constructs a new TronadoData class.
 *  @param  resolution [in] volume resolution
 *  @param  time [in] time value
 */
/*===========================================================================*/
TornadoVolumeData::TornadoVolumeData( const kvs::Vector3ui resolution, const int time )
{
    SuperClass::setResolution( resolution );
    this->setTime( time );
    this->exec();
}

/*===========================================================================*/
/**
 *  @brief  Destructs the TornadoVolumeData class.
 */
/*===========================================================================*/
TornadoVolumeData::~TornadoVolumeData()
{
}

/*===========================================================================*/
/**
 *  @brief  Constructs a new TornadoVolumeData class.
 */
/*===========================================================================*/
TornadoVolumeData::TornadoVolumeData()
{
}

/*===========================================================================*/
/**
 *  @brief  Returns the time value.
 *  @return time value
 */
/*===========================================================================*/
int TornadoVolumeData::time() const
{
    return( m_time );
}

/*===========================================================================*/
/**
 *  @brief  Sets a time value.
 *  @param  time [in] time value
 */
/*===========================================================================*/
void TornadoVolumeData::setTime( const int time )
{
    m_time = time;
}

/*===========================================================================*/
/**
 *  @brief  Main routine of the tornado volume data generator.
 *  @return pointer to the structured volume object
 *
 *  Orignal code was developed by R. Crawfis at Ohio State University.
 *  <br><a ref="http://www.cse.ohio-state.edu/~crawfis/Data/Tornado/">
 *  http://www.cse.ohio-state.edu/~crawfis/Data/Tornado/</a>
 */
/*===========================================================================*/
TornadoVolumeData::SuperClass* TornadoVolumeData::exec()
{
    const kvs::UInt64 veclen = 3;
    const kvs::UInt64 dim1 = SuperClass::resolution().x();
    const kvs::UInt64 dim2 = SuperClass::resolution().y();
    const kvs::UInt64 dim3 = SuperClass::resolution().z();

    const double dx = 1.0 / ( dim1 - 1.0 );
    const double dy = 1.0 / ( dim2 - 1.0 );
    const double dz = 1.0 / ( dim3 - 1.0 );

    kvs::ValueArray<kvs::Real32> values( static_cast<size_t>( dim1 * dim2 * dim3 * veclen ) );
    kvs::Real32* pvalues = values.data();
    kvs::UInt64 index = 0;
    for( kvs::UInt64 k = 0; k < dim3; k++ )
    {
        /* map z to 0->1
         * For each z-slice, determine the spiral circle.
         * (xc,yc) determine the center of the circle.
         * The radius also changes at each z-slice.
         * r is the center radius, r2 is for damping
         */
        const double z  = k * dz;
        const double xc = 0.5 + 0.1 * std::sin( 0.04 * m_time + 10.0 * z );
        const double yc = 0.5 + 0.1 * std::cos( 0.03 * m_time + 3.0 * z );
        const double r  = 0.1 + 0.4 * z * z + 0.1 * z * std::sin( 8.0 * z );
        const double r2 = 0.2 + 0.1 * z;

        for( kvs::UInt64 j = 0; j < dim2; j++ )
        {
            const double y = j * dy;
            for( kvs::UInt64 i = 0; i < dim1; i++, index++ )
            {
                const double x = i * dx;

                double temp  = std::sqrt( (y-yc) * (y-yc) + (x-xc) * (x-xc) );
                double scale = std::fabs( r - temp );

                /*  I do not like this next line. It produces a discontinuity
                 *  in the magnitude. Fi it later.
                 */
                scale = scale > r2 ? 0.8 - scale : 1.0;

                double z0 = 0.1 * ( 0.1 - temp * z );
                if ( z0 < 0.0 )  z0 = 0.0;

                temp = std::sqrt( temp * temp + z0 * z0 );

                const double epsiron = 0.00000000001;
                scale = ( r + r2 - temp ) * scale / ( temp + epsiron );
                scale = scale / ( 1 + z );

                const double v[3] = {
                    scale *  ( y - yc ) + 0.1 * ( x - xc ),
                    scale * -( x - xc ) + 0.1 * ( y - yc ),
                    scale * z0
                };

                pvalues[ 3 * index + 0 ] = static_cast<kvs::Real32>( v[0] );
                pvalues[ 3 * index + 1 ] = static_cast<kvs::Real32>( v[1] );
                pvalues[ 3 * index + 2 ] = static_cast<kvs::Real32>( v[2] );
            }
        }
    }

    SuperClass::setGridType( kvs::StructuredVolumeObject::Uniform );
    SuperClass::setVeclen( veclen );
    SuperClass::setValues( kvs::AnyValueArray( values ) );
    SuperClass::updateMinMaxCoords();
    SuperClass::updateMinMaxValues();

    return( this );
}

} // end of namespace kvs
