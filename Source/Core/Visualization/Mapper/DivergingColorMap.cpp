/*****************************************************************************/
/**
 *  @file   DivergingColorMap.cpp
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
#include "DivergingColorMap.h"
#include <kvs/RGBColor>
#include <kvs/XYZColor>
#include <kvs/LabColor>
#include <kvs/MshColor>


namespace
{

kvs::MshColor RGB2Msh( const kvs::RGBColor& rgb )
{
    return rgb.toXYZColor().toLabColor().toMshColor();
}

kvs::RGBColor Msh2RGB( const kvs::MshColor& msh )
{
    return msh.toLabColor().toXYZColor().toRGBColor();
}

kvs::RGBColor Interpolate(
    const kvs::RGBColor& rgb1,
    const kvs::RGBColor& rgb2,
    const kvs::Real32 t )
{
    const kvs::MshColor msh1 = RGB2Msh( rgb1 );
    const kvs::MshColor msh2 = RGB2Msh( rgb2 );
    const kvs::MshColor msh = kvs::MshColor::Mix( msh1, msh2, t );
    return Msh2RGB( msh );
}

}

namespace kvs
{

kvs::ColorMap DivergingColorMap::Create(
    const kvs::RGBColor& rgb1,
    const kvs::RGBColor& rgb2,
    const size_t resolution )
{
    kvs::ColorMap::Table table( 3 * resolution );
    kvs::UInt8* color = table.data();
    for ( size_t i = 0; i < resolution; i++ )
    {
        const kvs::Real32 ratio = kvs::Real32(i) / ( resolution - 1 );
        const kvs::RGBColor rgb = ::Interpolate( rgb1, rgb2, ratio );
        *( color++ ) = rgb.r();
        *( color++ ) = rgb.g();
        *( color++ ) = rgb.b();
    }

    return kvs::ColorMap( table );
}

} // end of namespace kvs
