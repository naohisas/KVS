#pragma once
#include <kvs/Math>
#include <kvs/Vector3>
#include <kvs/Type>


namespace kvs
{

class RGBColor;

class HSLColor
{
private:
    kvs::Real32 m_h = 0.0f; ///< hue angle [0-1]
    kvs::Real32 m_s = 0.0f; ///< saturation [0-1]
    kvs::Real32 m_l = 0.0f; ///< lightness [0-1]

public:
    static kvs::HSLColor Mix( const kvs::HSLColor& hsl1, const kvs::HSLColor& hsl2, const kvs::Real32 t );

public:
    HSLColor() = default;
    HSLColor( kvs::Real32 h, kvs::Real32 s, kvs::Real32 l ): m_h( h ), m_s( s ), m_l( l ) {}
    HSLColor( const kvs::Vec3& hsl ): m_h( hsl[0] ), m_s( hsl[1] ), m_l( hsl[2] ) {}
    HSLColor( const kvs::HSLColor& hsl ): m_h( hsl.m_h ), m_s( hsl.m_s ), m_l( hsl.m_l ) {}
    HSLColor( const kvs::RGBColor& rgb );

    void set( kvs::Real32 h, kvs::Real32 s, kvs::Real32 l ) { m_h = h; m_s = s; m_l = l; }
    kvs::Real32 h() const { return m_h; }
    kvs::Real32 s() const { return m_s; }
    kvs::Real32 l() const { return m_l; }
    kvs::Vec3 toVec3() const { return kvs::Vec3( m_h, m_s, m_l ); }
    kvs::RGBColor toRGBColor() const;

    kvs::HSLColor& operator += ( const kvs::HSLColor& hsv );
    kvs::HSLColor& operator -= ( const kvs::HSLColor& hsv );
    kvs::HSLColor& operator = ( const kvs::HSLColor& hsv );
    kvs::HSLColor& operator = ( const kvs::RGBColor& rgb );
    friend bool operator == ( const kvs::HSLColor& a, const kvs::HSLColor& b );
    friend kvs::HSLColor operator + ( const kvs::HSLColor& a, const kvs::HSLColor& b );
};

} // end of namespace kvs
