/*****************************************************************************/
/**
 *  @file   DivergingColorMap.h
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
#pragma once

#include <kvs/RGBColor>
#include <kvs/ColorMap>


namespace kvs
{

namespace DivergingColorMap
{

kvs::ColorMap Create( const kvs::RGBColor& rgb1, const kvs::RGBColor& rgb2, const size_t resolution );

/*===========================================================================*/
/**
 *  @brief  Returns Cool-to-Warm colormap.
 *  @param  resolution [in] table resolution
 *  @return colormap
 */
/*===========================================================================*/
inline kvs::ColorMap CoolWarm( const size_t resolution )
{
    const kvs::RGBColor rgb1(  59, 76, 192 );
    const kvs::RGBColor rgb2( 180,  4,  38 );
    return Create( rgb1, rgb2, resolution );
}

};

} // end of namespace kvs
