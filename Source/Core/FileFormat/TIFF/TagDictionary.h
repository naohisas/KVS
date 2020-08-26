/****************************************************************************/
/**
 *  @file   TagDictionary.h
 *  @author Naohisa Sakamoto
 */
/****************************************************************************/
#pragma once

#include <kvs/Type>
#include <map>
#include "Tag.h"


namespace kvs
{

namespace tiff
{

/*===========================================================================*/
/**
 *  @brief  Tag directory class.
 */
/*===========================================================================*/
class TagDictionary
{
public:

    typedef std::map<kvs::UInt16,std::string> Container;

private:

    Container m_container;

public:

    TagDictionary();

public:

    kvs::tiff::Tag find( const kvs::UInt16 tag_id ) const;

private:

    void create_dictionary();
};

} // end of namespace tiff

} // end of namespace kvs
