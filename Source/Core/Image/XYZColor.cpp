/*****************************************************************************/
/**
 *  @file   XYZColor.cpp
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
#include "XYZColor.h"
#include "LabColor.h"
#include <kvs/RGBColor>
#include <kvs/Matrix33>
#include <kvs/Math>


namespace
{

inline kvs::Real32 ToStandard( const kvs::Real32 C )
{
    kvs::Real32 Cs = 0;
    if ( C <= 0.0031308f ) { Cs = 12.92f * C; }
    else { Cs = 1.055f * std::pow( C, 1.0f / 2.4f ) - 0.055f; }
    return kvs::Math::Clamp( Cs, 0.0f, 1.0f );
}

inline kvs::Real32 ToLinear( const kvs::Real32 C )
{
    kvs::Real32 Cl = 0;
    if ( C <= 0.04045f ) { Cl = C / 12.92f; }
    else { Cl = std::pow( ( C + 0.055f ) / 1.055f, 2.4f ); }
    return kvs::Math::Clamp( Cl, 0.0f, 1.0f );
}

/*===========================================================================*/
/**
 *  @brief  Converts color values from sRGB to linear sRGB.
 *  @param  sRGB [in] standard RGB
 *  @return lineared sRGB
 */
/*===========================================================================*/
inline kvs::Vec3 sRGB2RGB( const kvs::Vec3 sRGB )
{
    const kvs::Real32 R = ToLinear( sRGB[0] );
    const kvs::Real32 G = ToLinear( sRGB[1] );
    const kvs::Real32 B = ToLinear( sRGB[2] );
    return kvs::Vec3( R, G, B );
}

/*===========================================================================*/
/**
 *  @brief  Converts color values from linear sRGB to sRGB.
 *  @param  RGB [in] lineared sRGB
 *  @return standard RGB
 */
/*===========================================================================*/
inline kvs::Vec3 RGB2sRGB( const kvs::Vec3 RGB )
{
    const kvs::Real32 sR = ToStandard( RGB[0] );
    const kvs::Real32 sG = ToStandard( RGB[1] );
    const kvs::Real32 sB = ToStandard( RGB[2] );
    return kvs::Vec3( sR, sG, sB );
}

inline kvs::XYZColor RGB2XYZ( const kvs::RGBColor& rgb )
{
    const kvs::Vec3 sRGB = rgb.toVec3();
    const kvs::Vec3 RGB = sRGB2RGB( sRGB );
    const kvs::Mat3 M(
        0.412391f, 0.357584f, 0.180481f,
        0.212639f, 0.715169f, 0.072192f,
        0.019331f, 0.119195f, 0.950532f );
    const kvs::Vec3 XYZ = M * RGB;
    return kvs::XYZColor( XYZ[0], XYZ[1], XYZ[2] );
}

inline kvs::RGBColor XYZ2RGB( const kvs::XYZColor& xyz )
{
    const kvs::Mat3 Minv(
         3.240970f, -1.537383f, -0.498611f,
        -0.969244f,  1.875968f,  0.041555f,
         0.055630f, -0.203977f,  1.056972f );
    const kvs::Vec3 RGB = Minv * xyz.toVec3();
    const kvs::Vec3 sRGB = RGB2sRGB( RGB );
    return kvs::RGBColor( sRGB );
}

}


namespace kvs
{

XYZColor::XYZColor( kvs::Real32 x, kvs::Real32 y, kvs::Real32 z ):
    m_x( x ),
    m_y( y ),
    m_z( z )
{
}

XYZColor::XYZColor( const kvs::Vec3& xyz ):
    m_x( xyz[0] ),
    m_y( xyz[1] ),
    m_z( xyz[2] )
{
}

XYZColor::XYZColor( const kvs::XYZColor& xyz ):
    m_x( xyz.x() ),
    m_y( xyz.y() ),
    m_z( xyz.z() )
{
}

XYZColor::XYZColor( const kvs::RGBColor& rgb )
{
    *this = ::RGB2XYZ( rgb );
}

kvs::RGBColor XYZColor::toRGBColor() const
{
    return ::XYZ2RGB( *this );
}

kvs::LabColor XYZColor::toLabColor() const
{
    return kvs::LabColor( *this );
}

kvs::XYZColor& XYZColor::operator += ( const kvs::XYZColor& xyz )
{
    m_x += xyz.x();
    m_y += xyz.y();
    m_z += xyz.z();
    return *this;
}

kvs::XYZColor& XYZColor::operator -= ( const kvs::XYZColor& xyz )
{
    m_x -= xyz.x();
    m_y -= xyz.y();
    m_z -= xyz.z();
    return *this;
}

kvs::XYZColor& XYZColor::operator = ( const kvs::XYZColor& xyz )
{
    m_x = xyz.x();
    m_y = xyz.y();
    m_z = xyz.z();
    return *this;
}

kvs::XYZColor& XYZColor::operator = ( const kvs::RGBColor& rgb )
{
    *this = ::RGB2XYZ( rgb );
    return *this;
}

kvs::XYZColor operator + ( const kvs::XYZColor& a, const kvs::XYZColor& b )
{
    kvs::XYZColor ret( a ); ret += b;
    return ret;
}

bool operator == ( const kvs::XYZColor& a, const kvs::XYZColor& b )
{
    return
        kvs::Math::Equal( a.x(), b.x() ) &&
        kvs::Math::Equal( a.y(), b.y() ) &&
        kvs::Math::Equal( a.z(), b.z() );
}

} // end of namespace kvs
