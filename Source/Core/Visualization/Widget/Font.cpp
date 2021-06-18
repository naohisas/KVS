/*****************************************************************************/
/**
 *  @file   Font.cpp
 *  @author Naohisa Sakamoto
 */
/*****************************************************************************/
#include "Font.h"
#include <kvs/UIColor>


namespace
{

const std::string FamilyName[] =
{
    "Sans",
    "Serif"
};

const std::string StyleName[] =
{
    "Regular",
    "Italic",
    "Bold",
    "BoldItalic"
};

}

namespace kvs
{

Font::Font()
{
    this->setFamily( Sans );
    this->setStyle( Regular );
    this->setHorizontalAlign( Left );
    this->setVerticalAlign( Baseline );
    this->setSize( 18.0f );
    this->setColor( kvs::UIColor::Label() );
    this->setEnabledShadow( false );
    this->setShadowColor( kvs::UIColor::Label() );
    this->setShadowSizeRatio( 1.0f );
    this->setShadowDistance( 6.0f );
    this->setShadowAngle( 45.0f );
    this->setShadowBlur( 4.0f );
}

Font::Font( const Family& family, const float size )
{
    this->setFamily( family );
    this->setStyle( Regular );
    this->setHorizontalAlign( Left );
    this->setVerticalAlign( Baseline );
    this->setSize( size );
    this->setColor( kvs::UIColor::Label() );
    this->setEnabledShadow( false );
    this->setShadowColor( kvs::UIColor::Label() );
    this->setShadowSizeRatio( 1.0f );
    this->setShadowDistance( 6.0f );
    this->setShadowAngle( 45.0f );
    this->setShadowBlur( 4.0f );
}

Font::Font( const Family& family, const float size, const kvs::RGBAColor& color )
{
    this->setFamily( family );
    this->setStyle( Regular );
    this->setHorizontalAlign( Left );
    this->setVerticalAlign( Baseline );
    this->setSize( size );
    this->setColor( color );
    this->setEnabledShadow( false );
    this->setShadowColor( kvs::UIColor::Label() );
    this->setShadowSizeRatio( 1.0f );
    this->setShadowDistance( 6.0f );
    this->setShadowAngle( 45.0f );
    this->setShadowBlur( 4.0f );
}

Font::Font( const Family& family, const Style& style, const float size )
{
    this->setFamily( family );
    this->setStyle( style );
    this->setHorizontalAlign( Left );
    this->setVerticalAlign( Baseline );
    this->setSize( size );
    this->setColor( kvs::UIColor::Label() );
    this->setEnabledShadow( false );
    this->setShadowColor( kvs::UIColor::Label() );
    this->setShadowSizeRatio( 1.0f );
    this->setShadowDistance( 6.0f );
    this->setShadowAngle( 45.0f );
    this->setShadowBlur( 4.0f );
}

Font::Font( const Family& family, const Style& style, const float size, const kvs::RGBAColor& color )
{
    this->setFamily( family );
    this->setStyle( style );
    this->setHorizontalAlign( Left );
    this->setVerticalAlign( Baseline );
    this->setSize( size );
    this->setColor( color );
    this->setEnabledShadow( false );
    this->setShadowColor( kvs::UIColor::Label() );
    this->setShadowSizeRatio( 1.0f );
    this->setShadowDistance( 6.0f );
    this->setShadowAngle( 45.0f );
    this->setShadowBlur( 4.0f );
}

Font::~Font()
{
}

std::string Font::familyName() const
{
    return ::FamilyName[ this->family() ];
}

std::string Font::styleName() const
{
    return ::StyleName[ this->style() ];
}

std::string Font::fontName() const
{
    return this->familyName() + this->styleName();
}

} // end of namespace kvs
