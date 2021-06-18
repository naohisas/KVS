/****************************************************************************/
/**
 *  @file   ImageFileDirectory.h
 *  @author Naohisa Sakamoto
 */
/****************************************************************************/
#pragma once

#include "Entry.h"
#include <kvs/Type>
#include <kvs/Indent>
#include <iostream>
#include <fstream>
#include <list>


namespace kvs
{

namespace tiff
{

/*===========================================================================*/
/**
 *  @brief  Image file directory class.
 */
/*===========================================================================*/
class ImageFileDirectory
{
public:

    typedef std::list<kvs::tiff::Entry> EntryList;

private:

    EntryList m_entry_list; ///< entry list
    kvs::UInt32 m_offset; ///< bytes offset

public:

    ImageFileDirectory();
    ImageFileDirectory( std::ifstream& ifs );

    const std::list<tiff::Entry>& entryList() const;
    kvs::UInt32 offset() const;

    void print( std::ostream& os, const kvs::Indent& indent = kvs::Indent(0) ) const;
    bool read( std::ifstream& ifs );
};

} // end of namespace tiff

} // end of namespace kvs
