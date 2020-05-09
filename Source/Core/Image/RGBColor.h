/****************************************************************************/
/**
 *  @file   RGBColor.h
 *  @author Naohisa Sakamoto
 */
/*----------------------------------------------------------------------------
 *
 *  Copyright (c) Visualization Laboratory, Kyoto University.
 *  All rights reserved.
 *  See http://www.viz.media.kyoto-u.ac.jp/kvs/copyright/ for details.
 *
 *  $Id: RGBColor.h 1798 2014-08-04 05:25:10Z naohisa.sakamoto@gmail.com $
 */
/****************************************************************************/
#pragma once
#include <iostream>
#include <iomanip>
#include <kvs/Math>
#include <kvs/Type>
#include <kvs/Vector3>
#include <kvs/Deprecated>


namespace kvs
{

class RGBAColor;
class HSVColor;
class XYZColor;
class MshColor;

/*==========================================================================*/
/**
 *  RGB color class.
 */
/*==========================================================================*/
class RGBColor
{
private:
    kvs::UInt8 m_r; ///< red [0-255]
    kvs::UInt8 m_g; ///< green [0-255]
    kvs::UInt8 m_b; ///< blue [0-255]

public:
    static RGBColor Black() { return kvs::RGBColor( 0, 0, 0 ); }
    static RGBColor White() { return kvs::RGBColor( 255, 255, 255 ); }
    static RGBColor Red() { return kvs::RGBColor( 255, 0, 0 ); }
    static RGBColor Green() { return kvs::RGBColor( 0, 255, 0 ); }
    static RGBColor Blue() { return kvs::RGBColor( 0, 0, 255 ); }
    static RGBColor Yellow() { return kvs::RGBColor( 255, 255, 0 ); }
    static RGBColor Cyan() { return kvs::RGBColor( 0, 255, 255 ); }
    static RGBColor Magenta() { return kvs::RGBColor( 255, 0, 255 ); }
    static RGBColor Mix( const kvs::RGBColor& rgb1, const kvs::RGBColor& rgb2, const kvs::Real32 t );

public:
    RGBColor();
    RGBColor( kvs::UInt8 r, kvs::UInt8 g, kvs::UInt8 b );
    RGBColor( const kvs::UInt8 rgb[3] );
    RGBColor( const kvs::RGBColor& rgb );
    RGBColor( const kvs::HSVColor& hsv );
    RGBColor( const kvs::MshColor& msh );
    RGBColor( const kvs::Vec3& rgb );
    RGBColor( const kvs::Vec3i& rgb );

    void set( kvs::UInt8 r, kvs::UInt8 g, kvs::UInt8 b ) { m_r = r; m_g = g; m_b = b; }
    kvs::UInt8 r() const { return m_r; }
    kvs::UInt8 g() const { return m_g; }
    kvs::UInt8 b() const { return m_b; }
    kvs::Vec3 toVec3() const;
    kvs::Vec3i toVec3i() const;
    kvs::HSVColor toHSVColor() const;
    kvs::XYZColor toXYZColor() const;
    kvs::MshColor toMshColor() const;

    kvs::RGBColor& operator += ( const kvs::RGBColor& rgb );
    kvs::RGBColor& operator -= ( const kvs::RGBColor& rgb );
    kvs::RGBColor& operator = ( const kvs::RGBColor& rgb );
    kvs::RGBColor& operator = ( const kvs::RGBAColor& rgba );
    kvs::RGBColor& operator = ( const kvs::HSVColor& hsv );
    kvs::RGBColor& operator = ( const kvs::MshColor& hsv );
    kvs::RGBColor& operator = ( const kvs::Vec3& rgb );
    kvs::RGBColor& operator = ( const kvs::Vec3i& rgb );
    friend bool operator == ( const kvs::RGBColor& a, const kvs::RGBColor& b );
    friend kvs::RGBColor operator + ( const kvs::RGBColor& a, const kvs::RGBColor& b );
    friend kvs::RGBColor operator - ( const kvs::RGBColor& a, const kvs::RGBColor& b );
    friend std::ostream& operator << ( std::ostream& os, const RGBColor& rgb );

    template <typename T>
    friend kvs::RGBColor operator * ( const T a, const kvs::RGBColor& rgb )
    {
        const kvs::UInt8 r = static_cast<kvs::UInt8>( kvs::Math::Round( a * rgb.r() ) );
        const kvs::UInt8 g = static_cast<kvs::UInt8>( kvs::Math::Round( a * rgb.g() ) );
        const kvs::UInt8 b = static_cast<kvs::UInt8>( kvs::Math::Round( a * rgb.b() ) );
        return kvs::RGBColor( r, g, b );
    }

    template <typename T>
    friend RGBColor operator * ( const RGBColor& rgb, const T a )
    {
        const kvs::UInt8 r = static_cast<kvs::UInt8>( kvs::Math::Round( a * rgb.r() ) );
        const kvs::UInt8 g = static_cast<kvs::UInt8>( kvs::Math::Round( a * rgb.g() ) );
        const kvs::UInt8 b = static_cast<kvs::UInt8>( kvs::Math::Round( a * rgb.b() ) );
        return kvs::RGBColor( r, g, b );
    }

    template <typename T>
    friend RGBColor operator / ( const RGBColor& rgb, const T a )
    {
        const kvs::UInt8 r = static_cast<kvs::UInt8>( kvs::Math::Round( rgb.r() / a ) );
        const kvs::UInt8 g = static_cast<kvs::UInt8>( kvs::Math::Round( rgb.g() / a ) );
        const kvs::UInt8 b = static_cast<kvs::UInt8>( kvs::Math::Round( rgb.b() / a ) );
        return kvs::RGBColor( r, g, b );
    }

public:
    KVS_DEPRECATED( kvs::UInt8 red() const ) { return this->r(); }
    KVS_DEPRECATED( kvs::UInt8 green() const ) { return this->g(); }
    KVS_DEPRECATED( kvs::UInt8 blue() const ) { return this->b(); }
};

} // end of namespace kvs
