/****************************************************************************/
/**
 *  @file   TagDictionary.h
 *  @author Naohisa Sakamoto
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
