/*****************************************************************************/
/**
 *  @file   UIColor.h
 *  @author Naohisa Sakamoto
 */
/*****************************************************************************/
#pragma once
#include <kvs/RGBAColor>


namespace kvs
{

/*===========================================================================*/
/**
 *  @brief  UIColor class.
 */
/*===========================================================================*/
class UIColor
{
public:
    static UIColor& Instance();
    static kvs::RGBAColor Blue();
    static kvs::RGBAColor Green();
    static kvs::RGBAColor Indigo();
    static kvs::RGBAColor Orange();
    static kvs::RGBAColor Pink();
    static kvs::RGBAColor Purple();
    static kvs::RGBAColor Red();
    static kvs::RGBAColor Teal();
    static kvs::RGBAColor Yellow();
    static kvs::RGBAColor Gray();
    static kvs::RGBAColor Gray2();
    static kvs::RGBAColor Gray3();
    static kvs::RGBAColor Gray4();
    static kvs::RGBAColor Gray5();
    static kvs::RGBAColor Gray6();
    static kvs::RGBAColor Label();
    static kvs::RGBAColor SecondaryLabel();
    static kvs::RGBAColor TertiaryLabel();
    static kvs::RGBAColor QuaternaryLabel();
    static kvs::RGBAColor Fill();
    static kvs::RGBAColor SecondaryFill();
    static kvs::RGBAColor TertiaryFill();
    static kvs::RGBAColor QuaternaryFill();
    static kvs::RGBAColor PlaceholderText();
    static kvs::RGBAColor Background();
    static kvs::RGBAColor SecondaryBackground();
    static kvs::RGBAColor TertiaryBackground();
    static kvs::RGBAColor GroupedBackground();
    static kvs::RGBAColor SecondaryGroupedBackground();
    static kvs::RGBAColor TertiaryGroupedBackground();
    static kvs::RGBAColor Separator();
    static kvs::RGBAColor OpaqueSeparator();
    static kvs::RGBAColor Link();
    static kvs::RGBAColor DarkText();
    static kvs::RGBAColor LightText();

public:
    enum Mode
    {
        Dark, // dark color mode
        Light // light color mode
    };

private:
    static Mode m_mode; ///< color mode (Dark or Light)

public:
    void setMode( const Mode mode ) { m_mode = mode; }
    void setModeToDark() { m_mode = Dark; }
    void setModeToLight() { m_mode = Light; }
    Mode mode() const { return m_mode; }

private:
    UIColor() {}
    UIColor( const UIColor& ) {}
};

} // end of namespace kvs
