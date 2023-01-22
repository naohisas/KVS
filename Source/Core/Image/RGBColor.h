/****************************************************************************/
/**
 *  @file   RGBColor.h
 *  @author Naohisa Sakamoto
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
class HCLColor;
class HSLColor;
class HSVColor;
class XYZColor;
class MshColor;
class LabColor;

/*==========================================================================*/
/**
 *  RGB color class.
 */
/*==========================================================================*/
class RGBColor
{
private:
    kvs::UInt8 m_r = 0; ///< red [0-255]
    kvs::UInt8 m_g = 0; ///< green [0-255]
    kvs::UInt8 m_b = 0; ///< blue [0-255]

public:
    static RGBColor Black() { return { 0, 0, 0 }; }
    static RGBColor White() { return { 255, 255, 255 }; }
    static RGBColor Red() { return { 255, 0, 0 }; }
    static RGBColor Green() { return { 0, 255, 0 }; }
    static RGBColor Blue() { return { 0, 0, 255 }; }
    static RGBColor Yellow() { return { 255, 255, 0 }; }
    static RGBColor Cyan() { return { 0, 255, 255 }; }
    static RGBColor Magenta() { return { 255, 0, 255 }; }
    static RGBColor Mix( const RGBColor& rgb1, const RGBColor& rgb2, const kvs::Real32 t );

public:
    RGBColor() = default;
    RGBColor( kvs::UInt8 r, kvs::UInt8 g, kvs::UInt8 b ): m_r( r ), m_g( g ), m_b( b ) {}
    RGBColor( const kvs::UInt8 rgb[3] ): m_r( rgb[0] ), m_g( rgb[1] ), m_b( rgb[2] ) {}
    RGBColor( const kvs::RGBColor& rgb ): m_r( rgb.m_r ), m_g( rgb.m_g ), m_b( rgb.m_b ) {}
    RGBColor( const kvs::HSLColor& hsl );
    RGBColor( const kvs::HSVColor& hsv );
    RGBColor( const kvs::HCLColor& hcl );
    RGBColor( const kvs::MshColor& msh );
    RGBColor( const kvs::Vec3& rgb );
    RGBColor( const kvs::Vec3i& rgb );

    void set( kvs::UInt8 r, kvs::UInt8 g, kvs::UInt8 b ) { m_r = r; m_g = g; m_b = b; }
    kvs::UInt8 r() const { return m_r; }
    kvs::UInt8 g() const { return m_g; }
    kvs::UInt8 b() const { return m_b; }
    kvs::Vec3 toVec3() const;
    kvs::Vec3i toVec3i() const;
    kvs::HSLColor toHSLColor() const;
    kvs::HSVColor toHSVColor() const;
    kvs::XYZColor toXYZColor() const;
    kvs::MshColor toMshColor() const;
    kvs::HCLColor toHCLColor() const;

    kvs::RGBColor& operator += ( const kvs::RGBColor& rgb );
    kvs::RGBColor& operator -= ( const kvs::RGBColor& rgb );
    kvs::RGBColor& operator = ( const kvs::RGBColor& rgb );
    kvs::RGBColor& operator = ( const kvs::RGBAColor& rgba );
    kvs::RGBColor& operator = ( const kvs::HSLColor& hsl );
    kvs::RGBColor& operator = ( const kvs::HSVColor& hsv );
    kvs::RGBColor& operator = ( const kvs::HCLColor& hcl );
    kvs::RGBColor& operator = ( const kvs::MshColor& hsv );
    kvs::RGBColor& operator = ( const kvs::LabColor& lab );
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
