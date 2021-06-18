/*****************************************************************************/
/**
 *  @file   UIColor.cpp
 *  @author Naohisa Sakamoto
 */
/*****************************************************************************/
#include "UIColor.h"


namespace kvs
{

UIColor::Mode UIColor::m_mode = UIColor::Light;

/*===========================================================================*/
/**
 *  @brief  Instance a new UIColor.
 *  @return UIColor
 */
/*===========================================================================*/
UIColor& UIColor::Instance()
{
    static UIColor instance;
    return instance;
}

kvs::RGBAColor UIColor::Blue()
{
    switch ( m_mode )
    {
    case Dark:
        return kvs::RGBAColor( 10, 132, 255, 1.0f );
    case Light:
        return kvs::RGBAColor( 0, 122, 255, 1.0f );
    default:
        return kvs::RGBAColor( 0, 0, 255, 1.0f );
    }
}

kvs::RGBAColor UIColor::Green()
{
    switch ( m_mode )
    {
    case Dark:
        return kvs::RGBAColor( 48, 209, 88, 1.0f );
    case Light:
        return kvs::RGBAColor( 52, 199, 89, 1.0f );
    default:
        return kvs::RGBAColor( 0, 0, 255, 1.0f );
    }
}

kvs::RGBAColor UIColor::Indigo()
{
    switch ( m_mode )
    {
    case Dark:
        return kvs::RGBAColor( 94, 92, 230, 1.0f );
    case Light:
        return kvs::RGBAColor( 88, 86, 214, 1.0f );
    default:
        return kvs::RGBAColor( 75, 0, 130, 1.0f );
    }
}

kvs::RGBAColor UIColor::Orange()
{
    switch ( m_mode )
    {
    case Dark:
        return kvs::RGBAColor( 255, 159, 10, 1.0f );
    case Light:
        return kvs::RGBAColor( 255, 149, 0, 1.0f );
    default:
        return kvs::RGBAColor( 255, 165, 0, 1.0f );
    }
}

kvs::RGBAColor UIColor::Pink()
{
    switch ( m_mode )
    {
    case Dark:
        return kvs::RGBAColor( 255, 55, 95, 1.0f );
    case Light:
        return kvs::RGBAColor( 255, 45, 85, 1.0f );
    default:
        return kvs::RGBAColor( 255, 192, 203, 1.0f );
    }
}

kvs::RGBAColor UIColor::Purple()
{
    switch ( m_mode )
    {
    case Dark:
        return kvs::RGBAColor( 191, 90, 242, 1.0f );
    case Light:
        return kvs::RGBAColor( 175, 82, 222, 1.0f );
    default:
        return kvs::RGBAColor( 128, 0, 128, 1.0f );
    }
}

kvs::RGBAColor UIColor::Red()
{
    switch ( m_mode )
    {
    case Dark:
        return kvs::RGBAColor( 255, 69, 58, 1.0f );
    case Light:
        return kvs::RGBAColor( 255, 59, 48, 1.0f );
    default:
        return kvs::RGBAColor( 255, 0, 0, 1.0f );
    }
}

kvs::RGBAColor UIColor::Teal()
{
    switch ( m_mode )
    {
    case Dark:
        return kvs::RGBAColor( 100, 210, 255, 1.0f );
    case Light:
        return kvs::RGBAColor( 90, 200, 250, 1.0f );
    default:
        return kvs::RGBAColor( 0, 128, 128, 1.0f );
    }
}

kvs::RGBAColor UIColor::Yellow()
{
    switch ( m_mode )
    {
    case Dark:
        return kvs::RGBAColor( 255, 214, 10, 1.0f );
    case Light:
        return kvs::RGBAColor( 255, 204, 0, 1.0f );
    default:
        return kvs::RGBAColor( 255, 255, 0, 1.0f );
    }
}

kvs::RGBAColor UIColor::Gray()
{
    switch ( m_mode )
    {
    case Dark:
        return kvs::RGBAColor( 142, 142, 147, 1.0f );
    case Light:
        return kvs::RGBAColor( 142, 142, 147, 1.0f );
    default:
        return kvs::RGBAColor( 142, 142, 142, 1.0f );
    }
}

kvs::RGBAColor UIColor::Gray2()
{
    switch ( m_mode )
    {
    case Dark:
        return kvs::RGBAColor( 99, 99, 102, 1.0f );
    case Light:
        return kvs::RGBAColor( 174, 174, 178, 1.0f );
    default:
        return kvs::RGBAColor( 174, 174, 174, 1.0f );
    }
}

kvs::RGBAColor UIColor::Gray3()
{
    switch ( m_mode )
    {
    case Dark:
        return kvs::RGBAColor( 72, 72, 74, 1.0f );
    case Light:
        return kvs::RGBAColor( 199, 199, 204, 1.0f );
    default:
        return kvs::RGBAColor( 199, 199, 199, 1.0f );
    }
}

kvs::RGBAColor UIColor::Gray4()
{
    switch ( m_mode )
    {
    case Dark:
        return kvs::RGBAColor( 58, 58, 60, 1.0f );
    case Light:
        return kvs::RGBAColor( 209, 209, 214, 1.0f );
    default:
        return kvs::RGBAColor( 209, 209, 209, 1.0f );
    }
}

kvs::RGBAColor UIColor::Gray5()
{
    switch ( m_mode )
    {
    case Dark:
        return kvs::RGBAColor( 44, 44, 46, 1.0f );
    case Light:
        return kvs::RGBAColor( 229, 229, 234, 1.0f );
    default:
        return kvs::RGBAColor( 229, 229, 229, 1.0f );
    }
}

kvs::RGBAColor UIColor::Gray6()
{
    switch ( m_mode )
    {
    case Dark:
        return kvs::RGBAColor( 28, 28, 30, 1.0f );
    case Light:
        return kvs::RGBAColor( 242, 242, 247, 1.0f );
    default:
        return kvs::RGBAColor( 242, 242, 242, 1.0f );
    }
}

kvs::RGBAColor UIColor::Label()
{
    switch ( m_mode )
    {
    case Dark:
        return kvs::RGBAColor( 255, 255, 255, 1.0f );
    case Light:
        return kvs::RGBAColor( 0, 0, 0, 1.0f );
    default:
        return kvs::RGBAColor( 0, 0, 0, 1.0f );
    }
}

kvs::RGBAColor UIColor::SecondaryLabel()
{
    switch ( m_mode )
    {
    case Dark:
        return kvs::RGBAColor( 235, 235, 245, 0.6f );
    case Light:
        return kvs::RGBAColor( 60, 60, 67, 0.6f );
    default:
        return kvs::RGBAColor( 60, 60, 67, 0.6f );
    }
}

kvs::RGBAColor UIColor::TertiaryLabel()
{
    switch ( m_mode )
    {
    case Dark:
        return kvs::RGBAColor( 235, 235, 245, 0.6f );
    case Light:
        return kvs::RGBAColor( 60, 60, 67, 0.3f );
    default:
        return kvs::RGBAColor( 60, 60, 67, 0.3f );
    }
}

kvs::RGBAColor UIColor::QuaternaryLabel()
{
    switch ( m_mode )
    {
    case Dark:
        return kvs::RGBAColor( 235, 235, 245, 0.18f );
    case Light:
        return kvs::RGBAColor( 60, 60, 67, 0.18f );
    default:
        return kvs::RGBAColor( 60, 60, 67, 0.18f );
    }
}

kvs::RGBAColor UIColor::Fill()
{
    switch ( m_mode )
    {
    case Dark:
        return kvs::RGBAColor( 120, 120, 128, 0.36f );
    case Light:
        return kvs::RGBAColor( 120, 120, 128, 0.2f );
    default:
        return kvs::RGBAColor( 120, 120, 128, 0.2f );
    }
}

kvs::RGBAColor UIColor::SecondaryFill()
{
    switch ( m_mode )
    {
    case Dark:
        return kvs::RGBAColor( 120, 120, 128, 0.32f );
    case Light:
        return kvs::RGBAColor( 120, 120, 128, 0.16f );
    default:
        return kvs::RGBAColor( 120, 120, 128, 0.16f );
    }
}

kvs::RGBAColor UIColor::TertiaryFill()
{
    switch ( m_mode )
    {
    case Dark:
        return kvs::RGBAColor( 118, 118, 128, 0.24f );
    case Light:
        return kvs::RGBAColor( 118, 118, 128, 0.12f );
    default:
        return kvs::RGBAColor( 118, 118, 128, 0.12f );
    }
}

kvs::RGBAColor UIColor::QuaternaryFill()
{
    switch ( m_mode )
    {
    case Dark:
        return kvs::RGBAColor( 118, 118, 128, 0.18f );
    case Light:
        return kvs::RGBAColor( 116, 116, 128, 0.08f );
    default:
        return kvs::RGBAColor( 116, 116, 128, 0.08f );
    }
}

kvs::RGBAColor UIColor::PlaceholderText()
{
    switch ( m_mode )
    {
    case Dark:
        return kvs::RGBAColor( 235, 235, 245, 0.3f );
    case Light:
        return kvs::RGBAColor( 60, 60, 67, 0.3f );
    default:
        return kvs::RGBAColor( 60, 60, 67, 0.3f );
    }
}

kvs::RGBAColor UIColor::Background()
{
    switch ( m_mode )
    {
    case Dark:
        return kvs::RGBAColor( 0, 0, 0, 1.0f );
    case Light:
        return kvs::RGBAColor( 255, 255, 255, 1.0f );
    default:
        return kvs::RGBAColor( 255, 255, 255, 1.0f );
    }
}

kvs::RGBAColor UIColor::SecondaryBackground()
{
    switch ( m_mode )
    {
    case Dark:
        return kvs::RGBAColor( 28, 28, 30, 1.0f );
    case Light:
        return kvs::RGBAColor( 242, 242, 247, 1.0f );
    default:
        return kvs::RGBAColor( 242, 242, 247, 1.0f );
    }
}

kvs::RGBAColor UIColor::TertiaryBackground()
{
    switch ( m_mode )
    {
    case Dark:
        return kvs::RGBAColor( 44, 44, 46, 1.0f );
    case Light:
        return kvs::RGBAColor( 255, 255, 255, 1.0f );
    default:
        return kvs::RGBAColor( 255, 255, 255, 1.0f );
    }
}

kvs::RGBAColor UIColor::GroupedBackground()
{
    switch ( m_mode )
    {
    case Dark:
        return kvs::RGBAColor( 0, 0, 0, 1.0f );
    case Light:
        return kvs::RGBAColor( 242, 242, 247, 1.0f );
    default:
        return kvs::RGBAColor( 242, 242, 247, 1.0f );
    }
}

kvs::RGBAColor UIColor::SecondaryGroupedBackground()
{
    switch ( m_mode )
    {
    case Dark:
        return kvs::RGBAColor( 28, 28, 30, 1.0f );
    case Light:
        return kvs::RGBAColor( 255, 255, 255, 1.0f );
    default:
        return kvs::RGBAColor( 255, 255, 255, 1.0f );
    }
}

kvs::RGBAColor UIColor::TertiaryGroupedBackground()
{
    switch ( m_mode )
    {
    case Dark:
        return kvs::RGBAColor( 44, 44, 46, 1.0f );
    case Light:
        return kvs::RGBAColor( 242, 242, 247, 1.0f );
    default:
        return kvs::RGBAColor( 242, 242, 247, 1.0f );
    }
}

kvs::RGBAColor UIColor::Separator()
{
    switch ( m_mode )
    {
    case Dark:
        return kvs::RGBAColor( 84, 84, 88, 0.6f );
    case Light:
        return kvs::RGBAColor( 60, 60, 67, 0.29f );
    default:
        return kvs::RGBAColor( 60, 60, 67, 0.29f );
    }
}

kvs::RGBAColor UIColor::OpaqueSeparator()
{
    switch ( m_mode )
    {
    case Dark:
        return kvs::RGBAColor( 56, 56, 58, 1.0f );
    case Light:
        return kvs::RGBAColor( 198, 198, 200, 1.0f );
    default:
        return kvs::RGBAColor( 198, 198, 200, 1.0f );
    }
}

kvs::RGBAColor UIColor::Link()
{
    switch ( m_mode )
    {
    case Dark:
        return kvs::RGBAColor( 9, 132, 255, 1.0f );
    case Light:
        return kvs::RGBAColor( 0, 122, 255, 1.0f );
    default:
        return kvs::RGBAColor( 0, 122, 255, 1.0f );
    }
}

kvs::RGBAColor UIColor::DarkText()
{
    return kvs::RGBAColor( 0, 0, 0, 1.0f );
}

kvs::RGBAColor UIColor::LightText()
{
    return kvs::RGBAColor( 255, 255, 255, 0.6f );
}

} // end of namespace kvs
