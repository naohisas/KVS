/****************************************************************************/
/**
 *  @file Pgm.h
 */
/*----------------------------------------------------------------------------
 *
 *  Copyright (c) Visualization Laboratory, Kyoto University.
 *  All rights reserved.
 *  See http://www.viz.media.kyoto-u.ac.jp/kvs/copyright/ for details.
 *
 *  $Id: Pgm.h 1402 2012-12-07 01:32:08Z naohisa.sakamoto@gmail.com $
 */
/****************************************************************************/
#ifndef KVS__PGM_H_INCLUDE
#define KVS__PGM_H_INCLUDE

#include <string>
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
 *  PGM image class.
 */
/*==========================================================================*/
class Pgm : public kvs::FileFormatBase
{
public:

    typedef kvs::FileFormatBase BaseClass;
    typedef kvs::pnm::Header Header;

private:

    Pgm::Header m_header; ///< header information
    size_t m_width; ///< width
    size_t m_height; ///< height
    kvs::ValueArray<kvs::UInt8> m_pixels; ///< pixel data

public:

    static bool CheckExtension( const std::string& filename );

public:

    Pgm();
    Pgm( const size_t width, const size_t height, const kvs::ValueArray<kvs::UInt8>& pixels );
    Pgm( const std::string& filename );

    const Pgm::Header& header() const;
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

#endif // KVS__PGM_H_INCLUDE
