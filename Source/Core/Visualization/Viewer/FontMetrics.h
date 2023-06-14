/*****************************************************************************/
/**
 *  @file   FontMetrics.h
 *  @author Naohisa Sakamoto
 */
/*****************************************************************************/
#pragma once
#include <kvs/PaintDevice>
#include <kvs/Font>
#include <string>


namespace kvs
{

/*===========================================================================*/
/**
 *  @brief  Font metric class.
 */
/*===========================================================================*/
class FontMetrics
{
private:
    const kvs::Font& m_font; ///< reference to the font data
    kvs::PaintDevice* m_device = nullptr; ///< pointer to the paint device

public:
    FontMetrics( const kvs::Font& font, kvs::PaintDevice* device );

    int height() const;
    int width( const std::string& text ) const;
};

} // end of namespace kvs
