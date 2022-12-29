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

Font::Font( const Family& family, const float size )
{
    this->setFamily( family );
    this->setSize( size );
}

Font::Font( const Family& family, const float size, const kvs::RGBAColor& color )
{
    this->setFamily( family );
    this->setSize( size );
    this->setColor( color );
}

Font::Font( const Family& family, const Style& style, const float size )
{
    this->setFamily( family );
    this->setStyle( style );
    this->setSize( size );
}

Font::Font( const Family& family, const Style& style, const float size, const kvs::RGBAColor& color )
{
    this->setFamily( family );
    this->setStyle( style );
    this->setSize( size );
    this->setColor( color );
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
