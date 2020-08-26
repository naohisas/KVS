/****************************************************************************/
/**
 *  @file   Ppm.h
 *  @author Naohisa Sakamoto
 */
/****************************************************************************/
#ifndef KVS__PPM_H_INCLUDE
#define KVS__PPM_H_INCLUDE

#include <string>
#include <iostream>
#include <kvs/FileFormatBase>
#include <kvs/ValueArray>
#include <kvs/Type>
#include <kvs/Indent>
#include <kvs/Deprecated>
#include "Header.h"


namespace kvs
{

/*==========================================================================*/
/**
 *  PPM image class.
 */
/*==========================================================================*/
class Ppm : public kvs::FileFormatBase
{
public:

    typedef kvs::FileFormatBase BaseClass;
    typedef kvs::pnm::Header Header;

private:

    Ppm::Header m_header; ///< header information
    size_t m_width; ///< width
    size_t m_height; ///< height
    kvs::ValueArray<kvs::UInt8> m_pixels; ///< pixel data

public:

    static bool CheckExtension( const std::string& filename );

public:

    Ppm();
    Ppm( const size_t width, const size_t height, const kvs::ValueArray<kvs::UInt8>& data );
    Ppm( const std::string& filename );

    const Ppm::Header& header() const;
    size_t width() const;
    size_t height() const;
    const kvs::ValueArray<kvs::UInt8>& pixels() const;

    void print( std::ostream& os, const kvs::Indent& indent = kvs::Indent(0) ) const;
    bool read( const std::string& filename );
    bool write( const std::string& filename );

private:

    void set_header();

public:
    KVS_DEPRECATED( const kvs::ValueArray<kvs::UInt8>& data() const ) { return this->pixels(); }
};

} // end of namespace kvs

#endif // KVS__PPM_H_INCLUDE
