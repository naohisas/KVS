/*****************************************************************************/
/**
 *  @file   KVSMLTag.h
 */
/*----------------------------------------------------------------------------
 *
 *  Copyright (c) Visualization Laboratory, Kyoto University.
 *  All rights reserved.
 *  See http://www.viz.media.kyoto-u.ac.jp/kvs/copyright/ for details.
 *
 *  $Id: KVSMLTag.h 1812 2014-09-11 07:34:35Z naohisa.sakamoto@gmail.com $
 */
/*****************************************************************************/
#pragma once
#include <string>
#include <kvs/XMLDocument>
#include <kvs/XMLNode>
#include "TagBase.h"


namespace kvs
{

namespace kvsml
{

/*===========================================================================*/
/**
 *  @brief  Tag class for <KVSML>.
 */
/*===========================================================================*/
class KVSMLTag : public kvs::kvsml::TagBase
{
public:
    typedef kvs::kvsml::TagBase BaseClass;

private:
    TagAttribute<std::string> m_version; ///< KVSML version

public:
    KVSMLTag();

    bool hasVersion() const { return m_version.hasValue(); }
    const TagAttribute<std::string>& version() const { return m_version; }

    void setVersion( const std::string& version ) { m_version = version; }

    bool read( const kvs::XMLDocument* document );
    bool write( kvs::XMLDocument* document );

private:

    bool read( const kvs::XMLNode::SuperClass* parent );
    bool write( kvs::XMLNode::SuperClass* parent );
};

} // end of namespace kvsml

} // end of namespace kvs
