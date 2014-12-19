/****************************************************************************/
/**
 *  @file RGBColor.h
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
#ifndef KVS__RGB_COLOR_H_INCLUDE
#define KVS__RGB_COLOR_H_INCLUDE

#include <iostream>
#include <iomanip>
#include <kvs/Math>
#include <kvs/Type>
#include <kvs/Vector3>


namespace kvs
{

class HSVColor;
class RGBAColor;

/*==========================================================================*/
/**
 *  RGB color class.
 */
/*==========================================================================*/
class RGBColor
{
protected:

    kvs::UInt8 m_red; ///< red [0-255]
    kvs::UInt8 m_green; ///< green [0-255]
    kvs::UInt8 m_blue; ///< blue [0-255]

public:

    static RGBColor Black();
    static RGBColor White();
    static RGBColor Red();
    static RGBColor Green();
    static RGBColor Blue();
    static RGBColor Yellow();
    static RGBColor Cyan();
    static RGBColor Magenta();

public:

    RGBColor();
    RGBColor( kvs::UInt8 red, kvs::UInt8 green, kvs::UInt8 blue );
    RGBColor( const kvs::UInt8 rgb[3] );
    RGBColor( const RGBColor& rgb );
    RGBColor( const HSVColor& hsv );
    RGBColor( const kvs::Vec3& rgb );

public:

    RGBColor& operator += ( const RGBColor& rgb );
    RGBColor& operator -= ( const RGBColor& rgb );
    RGBColor& operator = ( const RGBColor& rgb );
    RGBColor& operator = ( const RGBAColor& rgba );
    RGBColor& operator = ( const HSVColor& hsv );
    RGBColor& operator = ( const kvs::Vec3& rgb );

public:

    friend bool operator == ( const RGBColor& a, const RGBColor& b )
    {
        return( ( a.m_red   == b.m_red   ) &&
                ( a.m_green == b.m_green ) &&
                ( a.m_blue  == b.m_blue  ) );
    }

    friend RGBColor operator + ( const RGBColor& a, const RGBColor& b )
    {
        RGBColor ret( a ); ret += b;
        return( ret );
    }

    friend RGBColor operator - ( const RGBColor& a, const RGBColor& b )
    {
        RGBColor ret( a ); ret -= b;
        return( ret );
    }

    template <typename T>
    friend RGBColor operator * ( const T a, const RGBColor& rgb )
    {
        return( RGBColor( static_cast<kvs::UInt8>( kvs::Math::Round( a * rgb.r() ) ),
                          static_cast<kvs::UInt8>( kvs::Math::Round( a * rgb.g() ) ),
                          static_cast<kvs::UInt8>( kvs::Math::Round( a * rgb.b() ) ) ) );
    }

    template <typename T>
    friend RGBColor operator * ( const RGBColor& rgb, const T a )
    {
        return( RGBColor( static_cast<kvs::UInt8>( kvs::Math::Round( a * rgb.r() ) ),
                          static_cast<kvs::UInt8>( kvs::Math::Round( a * rgb.g() ) ),
                          static_cast<kvs::UInt8>( kvs::Math::Round( a * rgb.b() ) ) ) );
    }

    template <typename T>
    friend RGBColor operator / ( const RGBColor& rgb, const T a )
    {
        return( RGBColor( static_cast<kvs::UInt8>( kvs::Math::Round( rgb.r() / a ) ),
                          static_cast<kvs::UInt8>( kvs::Math::Round( rgb.g() / a ) ),
                          static_cast<kvs::UInt8>( kvs::Math::Round( rgb.b() / a ) ) ) );
    }

    friend std::ostream& operator << ( std::ostream& os, const RGBColor& rgb )
    {
        const size_t width = 8;
        const size_t precision = 3;

        const std::ios_base::fmtflags original_flags( os.flags() );

        os.setf( std::ios::fixed );
        os.setf( std::ios::showpoint );

        os << std::setw(width) << std::setprecision(precision) << (int)rgb.r() << " ";
        os << std::setw(width) << std::setprecision(precision) << (int)rgb.g() << " ";
        os << std::setw(width) << std::setprecision(precision) << (int)rgb.b();

        os.flags( original_flags );

        return( os );
    }

public:

    void set( kvs::UInt8 red, kvs::UInt8 green, kvs::UInt8 blue );

public:

    kvs::UInt8 r() const { return m_red; }
    kvs::UInt8 g() const { return m_green; }
    kvs::UInt8 b() const { return m_blue; }
    kvs::UInt8 red() const { return m_red; }
    kvs::UInt8 green() const { return m_green; }
    kvs::UInt8 blue() const { return m_blue; }
    kvs::Vec3 toVec3() const;
};

} // end of namespace kvs

#endif // KVS__RGB_COLOR_H_INCLUDE
