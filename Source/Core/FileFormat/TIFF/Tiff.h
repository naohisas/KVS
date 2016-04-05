/****************************************************************************/
/**
 *  @file   Tiff.h
 *  @author Naohisa Sakamoto
 */
/*----------------------------------------------------------------------------
 *
 *  Copyright (c) Visualization Laboratory, Kyoto University.
 *  All rights reserved.
 *  See http://www.viz.media.kyoto-u.ac.jp/kvs/copyright/ for details.
 *
 *  $Id: Tiff.h 1313 2012-09-19 08:19:01Z naohisa.sakamoto@gmail.com $
 */
/****************************************************************************/
#pragma once

#include <kvs/FileFormatBase>
#include <kvs/Type>
#include <kvs/Message>
#include <kvs/AnyValueArray>
#include <kvs/Indent>
#include <iostream>
#include "Header.h"
#include "ImageFileDirectory.h"


namespace kvs
{

/*===========================================================================*/
/**
 *  @brief  Tiff class.
 */
/*===========================================================================*/
class Tiff : public kvs::FileFormatBase
{
public:

    typedef kvs::FileFormatBase BaseClass;
    typedef kvs::tiff::Header Header;
    typedef kvs::tiff::ImageFileDirectory IFD;

public:

    enum ColorMode
    {
        UnknownColorMode = 0,
        Gray8,
        Gray16,
        Color24
    };

private:

    Tiff::Header m_header; ///< header information
    Tiff::IFD m_ifd; ///< 0-th IFD
    size_t m_width; ///< width
    size_t m_height; ///< height
    size_t m_bits_per_sample; ///< bits per channel (sample)
    ColorMode m_color_mode; ///< color mode
    kvs::AnyValueArray m_raw_data; ///< raw data

public:

    static bool CheckExtension( const std::string& filename );

public:

    Tiff();
    Tiff( const std::string& filename );
    virtual ~Tiff();

    const Tiff::Header& header() const { return m_header; }
    const Tiff::IFD& ifd() const { return m_ifd; }
    size_t width() const { return m_width; }
    size_t height() const { return m_height; }
    size_t bitsPerSample() const { return m_bits_per_sample; }
    ColorMode colorMode() const { return m_color_mode; }
    const kvs::AnyValueArray& rawData() const { return m_raw_data; }
    bool isSupported() const;

    void print( std::ostream& os, const kvs::Indent& indent = kvs::Indent(0) ) const;
    bool read( const std::string& filename );

private:

    bool write( const std::string& filename );
    size_t get_width() const;
    size_t get_height() const;
    size_t get_bits_per_sample() const;
    size_t get_samples_per_pixel() const;
    size_t get_photometirc_interpretation() const;
    size_t get_rows_per_strip() const;
    size_t get_compression_mode() const;
    kvs::AnyValueArray get_strip_offsets() const;
    kvs::AnyValueArray get_strip_bytes() const;
    ColorMode get_color_mode() const;
    kvs::AnyValueArray get_raw_data( std::ifstream& ifs ) const;
};

} // end of namespace kvs
