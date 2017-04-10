/*****************************************************************************/
/**
 *  @file   Font.h
 *  @author Naohisa Sakamoto
 */
/*----------------------------------------------------------------------------
 *
 *  Copyright (c) Visualization Laboratory, Kyoto University.
 *  All rights reserved.
 *  See http://www.viz.media.kyoto-u.ac.jp/kvs/copyright/ for details.
 *
 *  $Id$
 */
/*****************************************************************************/
#pragma once
#include <string>
#include <kvs/Vector2>
#include <kvs/RGBAColor>


namespace kvs
{

/*===========================================================================*/
/**
 *  @brief  Font class
 */
/*===========================================================================*/
class Font
{
public:
    enum Family
    {
        Sans = 0, ///< Sans font
        Serif,    ///< Sans-Serif font
    };

    enum Style
    {
        Regular = 0, ///< regular style
        Italic,      ///< italic
        Bold,        ///< bold
        BoldItalic,  ///< bold italic
    };

    enum HorizontalAlign
    {
        Left = 1 << 0,
        Center = 1 << 1,
        Right = 1 << 2,
    };

    enum VerticalAlign
    {
        Top = 1 << 3,
        Middle = 1 << 4,
        Bottom = 1 << 5,
        Baseline = 1 << 6,
    };

    enum Icon
    {
        Search = 0x1F50D,
        CircledCross = 0x2716,
        Check = 0x2713,
        Login = 0xE740,
        Trash = 0xE729,
    };

private:
    Family m_family; ///< font family
    Style m_style; ///< font style
    HorizontalAlign m_horizontal_align; ///< horizontal alignment
    VerticalAlign m_vertical_align; ///< vertical alignment
    float m_size; ///< font size
    kvs::RGBAColor m_color; ///< font color
    bool m_enabled_shadow; ///< flag for shadowing
    kvs::RGBAColor m_shadow_color; ///< shadow color
    float m_shadow_size_ratio; ///< shadow size
    float m_shadow_distance; ///< shadow distance
    float m_shadow_angle; ///< shadow angle [deg]
    float m_shadow_blur; ///< shadow blur level

public:
//    static void AddSearchPath( const std::string& path );
//    static void SetSearchPath( const std::string& path );
//    static void ResetSearchPath();
//    static void RemoveSearchPath();

public:
    Font();
    Font( const Family& family, const float size );
    Font( const Family& family, const float size, const kvs::RGBAColor& color );
    Font( const Family& family, const Style& style, const float size );
    Font( const Family& family, const Style& style, const float size, const kvs::RGBAColor& color );
    virtual ~Font();

    Family family() const { return m_family; }
    Style style() const { return m_style; }
    HorizontalAlign horizontalAlign() const { return m_horizontal_align; }
    VerticalAlign verticalAlign() const { return m_vertical_align; }
    float size() const { return m_size; }
    const kvs::RGBAColor& color() const { return m_color; }
    bool isEnabledShadow() const { return m_enabled_shadow; }
    const kvs::RGBAColor& shadowColor() const { return m_shadow_color; }
    float shadowSizeRatio() const { return m_shadow_size_ratio; }
    float shadowDistance() const { return m_shadow_distance; }
    float shadowAngle() const { return m_shadow_angle; }
    float shadowBlur() const { return m_shadow_blur; }

    void setFamily( const Family family ) { m_family = family; }
    void setFamilyToSans() { this->setFamily( Sans ); }
    void setFamilyToSerif() { this->setFamily( Serif ); }
    void setStyle( const Style style ) { m_style = style; }
    void setStyleToRegular() { this->setStyle( Regular ); }
    void setStyleToItalic() { this->setStyle( Italic ); }
    void setStyleToBold() { this->setStyle( Bold ); }
    void setStyleToBoldItalic() { this->setStyle( BoldItalic ); }
    void setHorizontalAlign( const HorizontalAlign align ) { m_horizontal_align = align; }
    void setHorizontalAlignToLeft() { this->setHorizontalAlign( Left ); }
    void setHorizontalAlignToCenter() { this->setHorizontalAlign( Center ); }
    void setHorizontalAlignToRight() { this->setHorizontalAlign( Right ); }
    void setVerticalAlign( const VerticalAlign align ) { m_vertical_align = align; }
    void setVerticalAlignToTop() { this->setVerticalAlign( Top ); }
    void setVerticalAlignToMiddle() { this->setVerticalAlign( Middle ); }
    void setVerticalAlignToBottom() { this->setVerticalAlign( Bottom ); }
    void setVerticalAlignToBaseline() { this->setVerticalAlign( Baseline ); }
    void setSize( const float size ) { m_size = size; }
    void setColor( const kvs::RGBAColor& color ) { m_color = color; }
    void setEnabledShadow( const bool enabled ) { m_enabled_shadow = enabled; }
    void setShadowColor( const kvs::RGBAColor& color ) { m_shadow_color = color; }
    void setShadowSizeRatio( const float ratio ) { m_shadow_size_ratio = ratio; }
    void setShadowDistance( const float distance ) { m_shadow_distance = distance; }
    void setShadowAngle( const float angle ) { m_shadow_angle = angle; }
    void setShadowBlur( const float blur ) { m_shadow_blur = blur; }

    std::string familyName() const;
    std::string styleName() const;
    std::string fontName() const;

    // Deprecated.
    float width( const std::string& /*text*/ ) const { return 0; }
    float height() const { return 0; }
    void draw( const kvs::Vec2& /*p*/, const std::string& /*text*/ ) const {}
    void draw( const kvs::Vec2& /*p*/, const Icon& /*icon*/, const float /*size*/ ) const {}
};

} // end of namespace kvs
