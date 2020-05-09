/****************************************************************************/
/**
 *  @file   HSVColor.h
 *  @author Naohisa Sakamoto
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
#pragma once
#include <kvs/Math>
#include <kvs/Vector3>
#include <kvs/Type>
#include <kvs/Deprecated>


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
private:
    kvs::Real32 m_h; ///< hue angle [0-1]
    kvs::Real32 m_s; ///< saturation [0-1]
    kvs::Real32 m_v; ///< value (intensity) [0-1]

public:
    static kvs::HSVColor Mix( const kvs::HSVColor& hsv1, const kvs::HSVColor& hsv2, const kvs::Real32 t );

public:
    HSVColor( kvs::Real32 h = 0.0f, kvs::Real32 s = 0.0f, kvs::Real32 v = 0.0f );
    HSVColor( const kvs::Vec3& hsv );
    HSVColor( const kvs::HSVColor& hsv );
    HSVColor( const kvs::RGBColor& rgb );

    void set( kvs::Real32 h, kvs::Real32 s, kvs::Real32 v ) { m_h = h; m_s = s; m_v = v; }
    kvs::Real32 h() const { return m_h; }
    kvs::Real32 s() const { return m_s; }
    kvs::Real32 v() const { return m_v; }
    kvs::Vec3 toVec3() const { return kvs::Vec3( m_h, m_s, m_v ); }
    kvs::RGBColor toRGBColor() const;

    kvs::HSVColor& operator += ( const kvs::HSVColor& hsv );
    kvs::HSVColor& operator -= ( const kvs::HSVColor& hsv );
    kvs::HSVColor& operator = ( const kvs::HSVColor& hsv );
    kvs::HSVColor& operator = ( const kvs::RGBColor& rgb );
    friend bool operator == ( const kvs::HSVColor& a, const kvs::HSVColor& b );
    friend kvs::HSVColor operator + ( const kvs::HSVColor& a, const kvs::HSVColor& b );

public:
    KVS_DEPRECATED( float hue() const ) { return this->h(); }
    KVS_DEPRECATED( float saturation() const ) { return this->s(); }
    KVS_DEPRECATED( float value() const ) { return this->v(); }
    KVS_DEPRECATED( float intensity() const ) { return this->v(); }
};

} // end of namespace kvs
