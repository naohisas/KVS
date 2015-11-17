/****************************************************************************/
/**
 *  @file   Bmp.h
 *  @author Naohisa Sakamoto
 */
/*----------------------------------------------------------------------------
 *
 *  Copyright (c) Visualization Laboratory, Kyoto University.
 *  All rights reserved.
 *  See http://www.viz.media.kyoto-u.ac.jp/kvs/copyright/ for details.
 *
 *  $Id: Bmp.h 1402 2012-12-07 01:32:08Z naohisa.sakamoto@gmail.com $
 */
/****************************************************************************/
#ifndef KVS__BMP_H_INCLUDE
#define KVS__BMP_H_INCLUDE

#include <iostream>
#include <fstream>
#include <kvs/FileFormatBase>
#include <kvs/ClassName>
#include <kvs/Type>
#include <kvs/ValueArray>
#include <kvs/Indent>
#include <kvs/Deprecated>
#include "FileHeader.h"
#include "InfoHeader.h"


namespace kvs
{

/*==========================================================================*/
/**
 *  Bitmap image class.
 */
/*==========================================================================*/
class Bmp : public kvs::FileFormatBase
{
public:

    typedef kvs::FileFormatBase BaseClass;
    typedef kvs::bmp::FileHeader FileHeader;
    typedef kvs::bmp::InfoHeader InfoHeader;

private:

    Bmp::FileHeader m_file_header; ///< bitmap file header
    Bmp::InfoHeader m_info_header; ///< bitmap information header
    size_t m_width; ///< width
    size_t m_height; ///< height
    size_t m_bpp; ///< bits per pixel
    kvs::ValueArray<kvs::UInt8> m_pixels; ///< pixel data

public:

    static bool CheckExtension( const std::string& filename );

public:

    Bmp() {}
    Bmp( const size_t width, const size_t height, const kvs::ValueArray<kvs::UInt8>& pixels );
    Bmp( const std::string& filename );

    const Bmp::FileHeader& fileHeader() const { return m_file_header; }
    const Bmp::InfoHeader& infoHeader() const { return m_info_header; }
    size_t width() const { return m_width; }
    size_t height() const { return m_height; }
    size_t bitsPerPixel() const { return m_bpp; }
    const kvs::ValueArray<kvs::UInt8>& pixels() const { return m_pixels; }

    void print( std::ostream& os, const kvs::Indent& indent = kvs::Indent(0) ) const;
    bool read( const std::string& filename );
    bool write( const std::string& filename );

private:

    void set_header();
    void skip_header_and_pallete( std::ifstream& ifs );

public:
    KVS_DEPRECATED( kvs::ValueArray<kvs::UInt8> data() const ) { return this->pixels(); }
};

} // end of namespace kvs

#endif // KVS__BMP_H_INCLUDE
