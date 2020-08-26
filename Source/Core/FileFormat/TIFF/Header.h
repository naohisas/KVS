/****************************************************************************/
/**
 *  @file   Header.h
 *  @author Naohisa Sakamoto
 */
/****************************************************************************/
#pragma once

#include <kvs/Type>
#include <kvs/Indent>
#include <iostream>
#include <fstream>


namespace kvs
{

namespace tiff
{

/*===========================================================================*/
/**
 *  @brief  Header class.
 */
/*===========================================================================*/
class Header
{
private:

    kvs::UInt16 m_magic; ///< magic number
    kvs::UInt16 m_version; ///< TIFF version
    kvs::UInt32 m_offset; ///< byte offset to first IDF

public:

    Header();
    Header( std::ifstream& ifs );

public:

    friend std::ostream& operator << ( std::ostream& os, const Header& header );

public:

    kvs::UInt16 magic() const { return m_magic; }
    kvs::UInt16 version() const { return m_version; }
    kvs::UInt32 offset() const { return m_offset; }

    void print( std::ostream& os, const kvs::Indent& indent = kvs::Indent(0) ) const;
    bool read( std::ifstream& ifs );
};

} // end of namesapce tiff

} // end of namespace kvs
