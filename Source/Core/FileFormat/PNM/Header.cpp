/****************************************************************************/
/**
 *  @file Header.cpp
 */
/*----------------------------------------------------------------------------
 *
 *  Copyright (c) Visualization Laboratory, Kyoto University.
 *  All rights reserved.
 *  See http://www.viz.media.kyoto-u.ac.jp/kvs/copyright/ for details.
 *
 *  $Id: Header.cpp 1402 2012-12-07 01:32:08Z naohisa.sakamoto@gmail.com $
 */
/****************************************************************************/
#include "Header.h"


namespace
{
const size_t MaxLineLength = 256;
} // end of namespace


namespace kvs
{

namespace pnm
{

/*===========================================================================*/
/**
 *  @brief  Constructs a new Header class.
 */
/*===========================================================================*/
Header::Header()
{
}

/*===========================================================================*/
/**
 *  @brief  Constructs a new Header class.
 *  @param  ifs [in] input file stream
 */
/*===========================================================================*/
Header::Header( std::ifstream& ifs )
{
    this->read( ifs );
}

/*===========================================================================*/
/**
 *  @brief  Destroys the Header class.
 */
/*===========================================================================*/
Header::~Header()
{
}

/*===========================================================================*/
/**
 *  @brief  Returns the magic number.
 *  @return magic number
 */
/*===========================================================================*/
std::string Header::magic() const
{
    return m_magic;
}

/*===========================================================================*/
/**
 *  @brief  Returns the image width.
 *  @return image width
 */
/*===========================================================================*/
size_t Header::width() const
{
    return m_width;
}

/*===========================================================================*/
/**
 *  @brief  Returns the image height.
 *  @return image height
 */
/*===========================================================================*/
size_t Header::height() const
{
    return m_height;
}

/*===========================================================================*/
/**
 *  @brief  Returns the max. value of pixels.
 *  @return max. value
 */
/*===========================================================================*/
size_t Header::maxValue() const
{
    return m_max;
}

/*===========================================================================*/
/**
 *  @brief  Returns the bits per pixel.
 *  @return bits per pixel
 */
/*===========================================================================*/
size_t Header::bpp() const
{
    return m_bpp;
}

/*===========================================================================*/
/**
 *  @brief  Returns the bytes per line.
 *  @return bytes per line
 */
/*===========================================================================*/
size_t Header::bpl() const
{
    return m_bpl;
}

/*===========================================================================*/
/**
 *  @brief  Returns the data size.
 *  @return data size
 */
/*===========================================================================*/
size_t Header::size() const
{
    return m_size;
}

/*===========================================================================*/
/**
 *  @brief  Returns the offset bytes.
 *  @return offset bytes
 */
/*===========================================================================*/
std::streampos Header::offset() const
{
    return m_offset;
}

/*===========================================================================*/
/**
 *  @brief  Sets information to the header.
 *  @param  magic [in] magic word
 *  @param  width [in] image width
 *  @param  height [in] image height
 */
/*===========================================================================*/
void Header::set( const std::string& magic, const size_t width, const size_t height )
{
    m_magic = magic;
    m_width = width;
    m_height = height;
    m_max = ( this->isP1() || this->isP4() ) ? 0 : 255;
    m_bpp = this->bit_per_pixel();
    m_bpl = this->byte_per_line();
    m_size = this->data_size();
}

/*===========================================================================*/
/**
 *  @brief  Checks whether this file is 'P1' or not.
 *  @return true if this file is P1
 */
/*===========================================================================*/
bool Header::isP1() const
{
    return m_magic == "p1" || m_magic == "P1";
}

/*===========================================================================*/
/**
 *  @brief  Checks whether this file is 'P2' or not.
 *  @return true if this file is P2
 */
/*===========================================================================*/
bool Header::isP2() const
{
    return m_magic == "p2" || m_magic == "P2";
}

/*===========================================================================*/
/**
 *  @brief  Checks whether this file is 'P3' or not.
 *  @return true if this file is P3
 */
/*===========================================================================*/
bool Header::isP3() const
{
    return m_magic == "p3" || m_magic == "P3";
}

/*===========================================================================*/
/**
 *  @brief  Checks whether this file is 'P4' or not.
 *  @return true if this file is P4
 */
/*===========================================================================*/
bool Header::isP4() const
{
    return m_magic == "p4" || m_magic == "P4";
}

/*===========================================================================*/
/**
 *  @brief  Checks whether this file is 'P5' or not.
 *  @return true if this file is P5
 */
/*===========================================================================*/
bool Header::isP5() const
{
    return m_magic == "p5" || m_magic == "P5";
}

/*===========================================================================*/
/**
 *  @brief  Checks whether this file is 'P6' or not.
 *  @return true if this file is P6
 */
/*===========================================================================*/
bool Header::isP6() const
{
    return m_magic == "p6" || m_magic == "P6";
}

/*===========================================================================*/
/**
 *  @brief  Prints header information.
 *  @param  os [in] output stream
 *  @param  indent [in] indent
 */
/*===========================================================================*/
void Header::print( std::ostream& os, const kvs::Indent& indent ) const
{
    os << indent << "Magic number : " << m_magic  << std::endl;
    os << indent << "Width : " << m_width  << std::endl;
    os << indent << "Height : " << m_height << std::endl;
    os << indent << "Max : " << m_max << std::endl;
}

/*===========================================================================*/
/**
 *  @brief  Reads header information.
 *  @param  ifs [in] input file stream
 */
/*===========================================================================*/
void Header::read( std::ifstream& ifs )
{
    ifs >> m_magic;
    this->skip_comment_line( ifs );

    ifs >> m_width >> m_height;
    this->skip_comment_line( ifs );

    if( isP1() || isP4() ) m_max = 0;
    else                   ifs >> m_max;
    this->skip_comment_line( ifs );

    m_offset = ifs.tellg();
    m_bpp = this->bit_per_pixel();
    m_bpl = this->byte_per_line();
    m_size = this->data_size();

    this->skip_header( ifs );
}

/*===========================================================================*/
/**
 *  @brief  Writes header information.
 *  @param  ofs [in] output file stream
 */
/*===========================================================================*/
void Header::write( std::ofstream& ofs ) const
{
    ofs << m_magic << std::endl;
    ofs << m_width << " " << m_height << std::endl;
    if( m_max > 0 ) ofs << m_max << std::endl;
}

/*===========================================================================*/
/**
 *  @brief  Returns the bits per pixel.
 *  @return bits per pixel
 */
/*===========================================================================*/
size_t Header::bit_per_pixel() const
{
    return
        ( this->isP1() || this->isP4() ) ?  1 :
        ( this->isP2() || this->isP5() ) ?  8 :
        ( this->isP3() || this->isP6() ) ? 24 : 0;
}

/*===========================================================================*/
/**
 *  @brief  Returns the bytes per line.
 *  @return bytes per line
 */
/*===========================================================================*/
size_t Header::byte_per_line() const
{
    return m_bpp == 1 ? ( m_width + 7 ) >> 3 : m_width * ( m_bpp >> 3 );
}

/*===========================================================================*/
/**
 *  @brief  Returns the data size.
 *  @return data size
 */
/*===========================================================================*/
size_t Header::data_size() const
{
    return m_height * m_bpl;
}

/*===========================================================================*/
/**
 *  @brief  Skips header region.
 *  @param  ifs [in] input file stream
 */
/*===========================================================================*/
void Header::skip_header( std::ifstream& ifs )
{
    ifs.clear();
    ifs.seekg( m_offset, std::ios::beg );
}

/*===========================================================================*/
/**
 *  @brief  Skips the comment lines.
 *  @param  ifs [in] input file stream
 */
/*===========================================================================*/
void Header::skip_comment_line( std::ifstream& ifs )
{
    char buf[ ::MaxLineLength ];
    while ( ifs.peek() == '\n' ) ifs.get();
    while ( ifs.peek() == '#'  ) ifs.getline( buf, ::MaxLineLength );
}

/*===========================================================================*/
/**
 *  @brief  Seeks next line.
 *  @param  ifs [in] input file stream
 */
/*===========================================================================*/
void Header::next_line( std::ifstream& ifs )
{
    char buf[ ::MaxLineLength ];
    ifs.getline( buf, ::MaxLineLength );
}

std::ostream& operator << ( std::ostream& os, const Header& header )
{
    os << "\tmagic number : " << header.m_magic  << std::endl;
    os << "\twidth        : " << header.m_width  << std::endl;
    os << "\theight       : " << header.m_height << std::endl;
    os << "\tmax          : " << header.m_max;

    return os;
}

} // end of namespace pnm

} // end of namespace kvs
