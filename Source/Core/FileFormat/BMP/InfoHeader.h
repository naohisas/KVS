/****************************************************************************/
/**
 *  @file InfoHeader.h
 */
/*----------------------------------------------------------------------------
 *
 *  Copyright (c) Visualization Laboratory, Kyoto University.
 *  All rights reserved.
 *  See http://www.viz.media.kyoto-u.ac.jp/kvs/copyright/ for details.
 *
 *  $Id: InfoHeader.h 1312 2012-09-18 15:33:43Z naohisa.sakamoto@gmail.com $
 */
/****************************************************************************/
#ifndef KVS__BMP__INFO_HEADER_H_INCLUDE
#define KVS__BMP__INFO_HEADER_H_INCLUDE

#include "HeaderBase.h"
#include <kvs/Type>
#include <kvs/Indent>
#include <iostream>
#include <fstream>


namespace kvs { class Bmp; }

namespace kvs
{

namespace bmp
{

/*==========================================================================*/
/**
 *  Bitmap info header class
 */
/*==========================================================================*/
class InfoHeader : public kvs::bmp::HeaderBase
{
    friend class kvs::Bmp;

public:

    typedef kvs::bmp::HeaderBase BaseClass;

private:

    kvs::UInt32 m_size; ///< size of this structure (40 bytes)
    kvs::UInt32 m_width; ///< image width
    kvs::UInt32 m_height; ///< image height
    kvs::UInt16 m_nplanes; ///< number of color planes (always 1)
    kvs::UInt16 m_bpp; ///< bit per pixel (1, 4, 8, 16 or 24)
    kvs::UInt32 m_compression; ///< compression type (0, 1, 2 or 3)
    kvs::UInt32 m_bitmapsize; ///< size of bitmap in bytes (0 if uncompressed)
    kvs::UInt32 m_hresolution; ///< pixels per meter (can be zero)
    kvs::UInt32 m_vresolution; ///< pixels per meter (can be zero)
    kvs::UInt32 m_colsused; ///< number of colors in pallete (can be zero)
    kvs::UInt32 m_colsimportant; ///< min number of important colors (can be zero)

public:

    InfoHeader();
    InfoHeader( std::ifstream& ifs );

    kvs::UInt32 size() const;
    kvs::UInt32 width() const;
    kvs::UInt32 height() const;
    kvs::UInt16 nplanes() const;
    kvs::UInt16 bpp() const;
    kvs::UInt32 compression() const;
    kvs::UInt32 bitmapsize() const;
    kvs::UInt32 hresolution() const;
    kvs::UInt32 vresolution() const;
    kvs::UInt32 colsused() const;
    kvs::UInt32 colsimportant() const;

    void print( std::ostream& os, const kvs::Indent& indent = kvs::Indent(0) ) const;
    void read( std::ifstream& ifs );
    void write( std::ofstream& ofs );

private:

    void swap_bytes();
};

} // end of namespace bmp

} // end of namespace kvs

#endif // KVS__BMP__INFO_HEADER_H_INCLUDE
