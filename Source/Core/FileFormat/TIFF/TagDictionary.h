/****************************************************************************/
/**
 *  @file TagDictionary.h
 */
/*----------------------------------------------------------------------------
 *
 *  Copyright (c) Visualization Laboratory, Kyoto University.
 *  All rights reserved.
 *  See http://www.viz.media.kyoto-u.ac.jp/kvs/copyright/ for details.
 *
 *  $Id: TagDictionary.h 1303 2012-09-14 11:26:37Z naohisa.sakamoto@gmail.com $
 */
/****************************************************************************/
#ifndef KVS__TIFF__TAG_DICTIONARY_H_INCLUDE
#define KVS__TIFF__TAG_DICTIONARY_H_INCLUDE

#include <kvs/Type>
#include <map>
#include "Tag.h"


namespace kvs
{

namespace tiff
{

class TagDictionary
{
public:

    typedef std::map<kvs::UInt16,std::string> Container;

protected:

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

#endif // KVS__TIFF__TAG_DICTIONARY_H_INCLUDE
