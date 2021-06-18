/*****************************************************************************/
/**
 *  @file   Png.h
 *  @author Naohisa Sakamoto
 */
/*****************************************************************************/
#pragma once
#include <string>
#include <iostream>
#include <kvs/FileFormatBase>
#include <kvs/ValueArray>
#include <kvs/Indent>


namespace kvs
{

/*===========================================================================*/
/**
 *  @brief  PNG image class.
 */
/*===========================================================================*/
class Png : public kvs::FileFormatBase
{
public:
    using BaseClass = kvs::FileFormatBase;

private:
    size_t m_width; ///< image width
    size_t m_height; ///< image height
    int m_bpp; ///< bytes per pixel
    int m_compression_level; ///< compression level
    kvs::ValueArray<kvs::UInt8> m_pixels;

public:
    static bool CheckExtension( const std::string& filename );

public:
    Png(): m_width(0), m_height(0), m_bpp(0), m_compression_level(8) {}
    Png( const size_t width, const size_t height, const kvs::ValueArray<kvs::UInt8>& pixels );
    Png( const std::string& filename );

    size_t width() const { return m_width; }
    size_t height() const { return m_height; }
    int bytesPerPixel() const { return m_bpp; }
    int compressionLevel() const { return m_compression_level; }
    const kvs::ValueArray<kvs::UInt8>& pixels() const { return m_pixels; }

    void setCompressionLevel( const int level ) { m_compression_level = level; }

    void print( std::ostream& os, const kvs::Indent& indent = kvs::Indent(0) ) const;
    bool read( const std::string& filename );
    bool write( const std::string& filename );
};

} // end of namespace kvs
