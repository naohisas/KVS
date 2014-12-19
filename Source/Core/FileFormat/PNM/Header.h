/****************************************************************************/
/**
 *  @file Header.h
 */
/*----------------------------------------------------------------------------
 *
 *  Copyright (c) Visualization Laboratory, Kyoto University.
 *  All rights reserved.
 *  See http://www.viz.media.kyoto-u.ac.jp/kvs/copyright/ for details.
 *
 *  $Id: Header.h 1402 2012-12-07 01:32:08Z naohisa.sakamoto@gmail.com $
 */
/****************************************************************************/
#ifndef KVS__PNM_HEADER_H_INCLUDE
#define KVS__PNM_HEADER_H_INCLUDE

#include <iostream>
#include <fstream>
#include <string>
#include <kvs/Indent>
#include <kvs/Deprecated>


namespace kvs
{

namespace pnm
{

/*==========================================================================*/
/**
 *  @brief  PNM image header class.
 */
/*==========================================================================*/
class Header
{
private:

    std::string m_magic; ///< magic number
    size_t m_width; ///< image width
    size_t m_height; ///< image height
    size_t m_max; ///< max color/gray value
    size_t m_bpp; ///< bit per pixel (1:bit, 8:gray, 24:color)
    size_t m_bpl; ///< byte per line
    size_t m_size; ///< data size [byte]
    std::streampos m_offset; ///< starting position of image data, in bytes.

public:

    Header();
    Header( std::ifstream& ifs );
    virtual ~Header();

    std::string magic() const;
    size_t width() const;
    size_t height() const;
    size_t maxValue() const;
    size_t bpp() const;
    size_t bpl() const;
    size_t size() const;
    std::streampos offset() const;

    void set( const std::string& magic, const size_t width, const size_t height );

    bool isP1() const;
    bool isP2() const;
    bool isP3() const;
    bool isP4() const;
    bool isP5() const;
    bool isP6() const;
    void print( std::ostream& os, const kvs::Indent& indent = kvs::Indent(0) ) const;
    void read( std::ifstream& ifs );
    void write( std::ofstream& ofs ) const;

private:

    size_t bit_per_pixel() const;
    size_t byte_per_line() const;
    size_t data_size() const;
    void skip_header( std::ifstream& ifs );
    void skip_comment_line( std::ifstream& ifs );
    void next_line( std::ifstream& ifs );

public:
    KVS_DEPRECATED( friend std::ostream& operator << ( std::ostream& os, const Header& header ) );
};

} // end of namespace pnm

} // end of namespace kvs

#endif // KVS__PNM_HEADER_H_INCLUDE
