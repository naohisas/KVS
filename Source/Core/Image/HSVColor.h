/****************************************************************************/
/**
 *  @file HSVColor.h
 */
/*----------------------------------------------------------------------------
 *
 *  Copyright (c) Visualization Laboratory, Kyoto University.
 *  All rights reserved.
 *  See http://www.viz.media.kyoto-u.ac.jp/kvs/copyright/ for details.
 *
 *  $Id: HSVColor.h 1316 2012-09-24 10:40:16Z naohisa.sakamoto@gmail.com $
 */
/****************************************************************************/
#ifndef KVS__HSV_COLOR_H_INCLUDE
#define KVS__HSV_COLOR_H_INCLUDE

#include <kvs/Math>


namespace kvs
{

class RGBColor;

/*==========================================================================*/
/**
 *  HSV color class.
 */
/*==========================================================================*/
class HSVColor
{
protected:

    float m_hue; ///< hue angle [0-1]
    float m_saturation; ///< saturation [0-1]
    float m_value; ///< value (intensity) [0-1]

public:

    HSVColor( float hue = 0.0f, float saturation = 0.0f, float value = 0.0f );
    HSVColor( const HSVColor& hsv );
    HSVColor( const RGBColor& rgb );

public:

    HSVColor& operator += ( const HSVColor& hsv );
    HSVColor& operator -= ( const HSVColor& hsv );
    HSVColor& operator = ( const HSVColor& hsv );
    HSVColor& operator = ( const RGBColor& rgb );

public:

    friend bool operator == ( const HSVColor& a, const HSVColor& b )
    {
        return( kvs::Math::Equal( a.h(), b.h() ) &&
                kvs::Math::Equal( a.s(), b.s() ) &&
                kvs::Math::Equal( a.v(), b.v() ) );
    }

    friend HSVColor operator + ( const HSVColor& a, const HSVColor& b )
    {
        HSVColor ret( a ); ret += b;
        return( ret );
    }

public:

    void set( float hue, float saturation, float value );

public:

    float h() const;
    float hue() const;
    float s() const;
    float saturation() const;
    float v() const;
    float value() const;
    float intensity() const;
};

} // end of namespace kvs

#endif // KVS__HSV_COLOR_H_INCLUDE
