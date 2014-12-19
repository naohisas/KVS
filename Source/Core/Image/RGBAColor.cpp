/****************************************************************************/
/**
 *  @file RGBAColor.cpp
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

RGBAColor::RGBAColor( float opacity ):
    kvs::RGBColor( 0, 0, 0 ),
    m_opacity( opacity )
{
}

RGBAColor::RGBAColor( kvs::UInt8 red, kvs::UInt8 green, kvs::UInt8 blue, float opacity ):
    kvs::RGBColor( red, green, blue ),
    m_opacity( opacity )
{
}

RGBAColor::RGBAColor( const kvs::UInt8 rgb[3], float opacity ):
    kvs::RGBColor( rgb[0], rgb[1], rgb[2] ),
    m_opacity( opacity )
{
}

RGBAColor::RGBAColor( const kvs::RGBColor& rgb, float opacity ):
    kvs::RGBColor( rgb ),
    m_opacity( opacity )
{
}

RGBAColor& RGBAColor::operator = ( const RGBAColor& rgba )
{
    m_red = rgba.m_red;
    m_green = rgba.m_green;
    m_blue = rgba.m_blue;
    m_opacity = rgba.m_opacity;
    return *this;
}

RGBAColor& RGBAColor::operator = ( const RGBColor& rgb )
{
    m_red = rgb.r();
    m_green = rgb.g();
    m_blue = rgb.b();
    m_opacity = 1.0f;
    return *this;
}

RGBAColor& RGBAColor::operator = ( const kvs::Vec4& rgba )
{
    KVS_ASSERT( 0.0f <= rgba.x() && rgba.x() <= 1.0f );
    KVS_ASSERT( 0.0f <= rgba.y() && rgba.y() <= 1.0f );
    KVS_ASSERT( 0.0f <= rgba.z() && rgba.z() <= 1.0f );
    KVS_ASSERT( 0.0f <= rgba.w() && rgba.w() <= 1.0f );

    m_red = kvs::Math::Round( rgba.x() * 255.0f );
    m_green = kvs::Math::Round( rgba.y() * 255.0f );
    m_blue = kvs::Math::Round( rgba.z() * 255.0f );
    m_opacity = rgba.w();
    return *this;
}

kvs::Vec4 RGBAColor::toVec4() const
{
    return kvs::Vec4( toVec3(), m_opacity );
}

} // end of namespace kvs
