/****************************************************************************/
/**
 *  @file   RGBAColor.h
 *  @author Naohisa Sakamoto
 */
/****************************************************************************/
#pragma once
#include "RGBColor.h"
#include <kvs/Type>
#include <kvs/Vector4>
#include <kvs/Deprecated>


namespace kvs
{

/*==========================================================================*/
/**
 *  RGB-a color class.
 */
/*==========================================================================*/
class RGBAColor : public kvs::RGBColor
{
private:
    kvs::Real32 m_a = 1.0f; ///< opacity [0-1]

public:
    RGBAColor() = default;
    RGBAColor( kvs::Real32 a ): m_a( a ) {}
    RGBAColor( kvs::UInt8 r, kvs::UInt8 g, kvs::UInt8 b, kvs::Real32 a = 1.0f );
    RGBAColor( const kvs::UInt8 rgb[3], kvs::Real32 a = 1.0f );
    RGBAColor( const kvs::RGBColor& rgb, kvs::Real32 a = 1.0f );
    RGBAColor( const kvs::Vec4& rgba );

    kvs::Real32 a() const { return m_a; }
    kvs::Vec4 toVec4() const { return { toVec3(), m_a }; }

    kvs::RGBAColor& operator = ( const kvs::RGBAColor& rgba );
    kvs::RGBAColor& operator = ( const kvs::RGBColor& rgb );
    kvs::RGBAColor& operator = ( const kvs::Vec4& rgba );

public:
    KVS_DEPRECATED( float alpha() const ) { return this->a(); }
    KVS_DEPRECATED( float opacity() const ) { return this->a(); }
};

} // end of namespace kvs
