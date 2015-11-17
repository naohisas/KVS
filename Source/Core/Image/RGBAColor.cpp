/****************************************************************************/
/**
 *  @file   RGBAColor.cpp
 *  @author Naohisa Sakamoto
 */
/*----------------------------------------------------------------------------
 *
 *  Copyright (c) Visualization Laboratory, Kyoto University.
 *  All rights reserved.
 *  See http://www.viz.media.kyoto-u.ac.jp/kvs/copyright/ for details.
 *
 *  $Id: RGBAColor.cpp 1798 2014-08-04 05:25:10Z naohisa.sakamoto@gmail.com $
 */
/****************************************************************************/
#include "RGBAColor.h"
#include "RGBColor.h"


namespace kvs
{

RGBAColor::RGBAColor( kvs::Real32 a ):
    kvs::RGBColor( 0, 0, 0 ),
    m_a( a )
{
}

RGBAColor::RGBAColor( kvs::UInt8 r, kvs::UInt8 g, kvs::UInt8 b, kvs::Real32 a ):
    kvs::RGBColor( r, g, b ),
    m_a( a )
{
}

RGBAColor::RGBAColor( const kvs::UInt8 rgb[3], kvs::Real32 a ):
    kvs::RGBColor( rgb[0], rgb[1], rgb[2] ),
    m_a( a )
{
}

RGBAColor::RGBAColor( const kvs::RGBColor& rgb, kvs::Real32 a ):
    kvs::RGBColor( rgb ),
    m_a( a )
{
}

kvs::RGBAColor& RGBAColor::operator = ( const kvs::RGBAColor& rgba )
{
    kvs::RGBColor::set( rgba.r(), rgba.g(), rgba.b() );
    m_a = rgba.m_a;
    return *this;
}

kvs::RGBAColor& RGBAColor::operator = ( const kvs::RGBColor& rgb )
{
    kvs::RGBColor::set( rgb.r(), rgb.g(), rgb.b() );
    m_a = 1.0f;
    return *this;
}

kvs::RGBAColor& RGBAColor::operator = ( const kvs::Vec4& rgba )
{
    KVS_ASSERT( 0.0f <= rgba.x() && rgba.x() <= 1.0f );
    KVS_ASSERT( 0.0f <= rgba.y() && rgba.y() <= 1.0f );
    KVS_ASSERT( 0.0f <= rgba.z() && rgba.z() <= 1.0f );
    KVS_ASSERT( 0.0f <= rgba.w() && rgba.w() <= 1.0f );

    const kvs::UInt8 r = kvs::Math::Round( rgba.x() * 255.0f );
    const kvs::UInt8 g = kvs::Math::Round( rgba.y() * 255.0f );
    const kvs::UInt8 b = kvs::Math::Round( rgba.z() * 255.0f );
    kvs::RGBColor::set( r, g, b );
    m_a = rgba.w();
    return *this;
}

} // end of namespace kvs
