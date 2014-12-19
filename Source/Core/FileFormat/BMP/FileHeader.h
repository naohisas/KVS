/****************************************************************************/
/**
 *  @file FileHeader.h
 */
/*----------------------------------------------------------------------------
 *
 *  Copyright (c) Visualization Laboratory, Kyoto University.
 *  All rights reserved.
 *  See http://www.viz.media.kyoto-u.ac.jp/kvs/copyright/ for details.
 *
 *  $Id: FileHeader.h 1312 2012-09-18 15:33:43Z naohisa.sakamoto@gmail.com $
 */
/****************************************************************************/
#ifndef KVS__BMP__FILE_HEADER_H_INCLUDE
#define KVS__BMP__FILE_HEADER_H_INCLUDE

#include "HeaderBase.h"
#include <kvs/Type>
#include <kvs/Indent>


namespace kvs { class Bmp; }

namespace kvs
{

namespace bmp
{

/*==========================================================================*/
/**
 *  Bitmap file header class
 */
/*==========================================================================*/
class FileHeader : public kvs::bmp::HeaderBase
{
    friend class kvs::Bmp;

public:

    typedef kvs::bmp::HeaderBase BaseClass;

private:

    kvs::UInt16 m_type; ///< the file type
    kvs::UInt32 m_size; ///< the file size
    kvs::UInt16 m_reserved1; ///< reserved value 1 (always 0)
    kvs::UInt16 m_reserved2; ///< reserved value 2 (always 0)
    kvs::UInt32 m_offset; ///< starting position of image data, in bytes (54)

public:

    FileHeader();
    FileHeader( std::ifstream& ifs );

    kvs::UInt16 type() const;
    kvs::UInt32 size() const;
    kvs::UInt16 reserved1() const;
    kvs::UInt16 reserved2() const;
    kvs::UInt32 offset() const;

    bool isBM();
    void print( std::ostream& os, const kvs::Indent& indent = kvs::Indent(0) ) const;
    void read( std::ifstream& ifs );
    void write( std::ofstream& ofs );

private:

    void swap_bytes();
};

} // end of namespace bmp

} // end of namespace kvs

#endif // KVS__BMP__FILE_HEADER_H_INCLUDE
