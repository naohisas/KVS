/****************************************************************************/
/**
 *  @file RGBAColor.h
 */
/*----------------------------------------------------------------------------
 *
 *  Copyright (c) Visualization Laboratory, Kyoto University.
 *  All rights reserved.
 *  See http://www.viz.media.kyoto-u.ac.jp/kvs/copyright/ for details.
 *
 *  $Id: RGBAColor.h 1798 2014-08-04 05:25:10Z naohisa.sakamoto@gmail.com $
 */
/****************************************************************************/
#ifndef KVS__RGBA_COLOR_H_INCLUDE
#define KVS__RGBA_COLOR_H_INCLUDE

#include "RGBColor.h"
#include <kvs/Type>
#include <kvs/Vector4>


namespace kvs
{

/*==========================================================================*/
/**
 *  RGB-a color class.
 */
/*==========================================================================*/
class RGBAColor : public kvs::RGBColor
{
protected:

    float m_opacity; ///< opacity [0-1]

public:

    RGBAColor( float opacity = 1.0f );
    RGBAColor( kvs::UInt8 red, kvs::UInt8 greeb, kvs::UInt8 blue, float opacity = 1.0f );
    RGBAColor( const kvs::UInt8 rgb[3], float opacity = 1.0f );
    RGBAColor( const kvs::RGBColor& rgb, float opacity = 1.0f );
    RGBAColor( const kvs::Vec4& rgba );

public:

    RGBAColor& operator = ( const RGBAColor& rgba );
    RGBAColor& operator = ( const RGBColor& rgb );
    RGBAColor& operator = ( const kvs::Vec4& rgba );

public:

    float a() const { return m_opacity; }
    float alpha() const { return m_opacity; }
    float opacity() const { return m_opacity; }
    kvs::Vec4 toVec4() const;
};

} // end of namespace kvs

#endif // KVS__RGBA_COLOR_H_INCLUDE
