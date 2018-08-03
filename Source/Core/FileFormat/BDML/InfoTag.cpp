/*****************************************************************************/
/**
 *  @file   InfoTag.cpp
 *  @author Naohisa Sakamoto
 */
/*----------------------------------------------------------------------------
 *
 *  Copyright (c) Visualization Laboratory, Kyoto University.
 *  All rights reserved.
 *  See http://www.viz.media.kyoto-u.ac.jp/kvs/copyright/ for details.
 *
 *  $Id$
 */
/*****************************************************************************/
#include "InfoTag.h"
#include "XML.h"


namespace kvs
{

namespace bdml
{

InfoTag::InfoTag() : Tag("info")
{
    m_bdmlID = "";
    m_title = "";
    m_license = "";
    m_release = "";
    m_version = 0;
}

void InfoTag::print( std::ostream& os, const kvs::Indent& indent ) const
{
    os << indent << "BDML ID: " << m_bdmlID << std::endl;
    os << indent << "Title: " << m_title << std::endl;
    os << indent << "License: " << m_license << std::endl;
    os << indent << "Release: " << m_release << std::endl;
    os << indent << "Version: " << m_version << std::endl;
}

bool InfoTag::read( const Node* parent )
{
    if ( !Tag::read( parent ) ) { return false; }

    Tag bdmlID_tag("bdmlID");
    if ( bdmlID_tag.read( this->node() ) ) { m_bdmlID = NodeValue<std::string>( bdmlID_tag.node() ); }

    Tag title_tag("title");
    if ( title_tag.read( this->node() ) ) { m_title = NodeValue<std::string>( title_tag.node() ); }

    Tag license_tag("license");
    if ( license_tag.read( this->node() ) ) { m_license = NodeValue<std::string>( license_tag.node() ); }

    Tag release_tag("release");
    if ( release_tag.read( this->node() ) ) { m_release = NodeValue<std::string>( release_tag.node() ); }

    Tag version_tag("version");
    if ( version_tag.read( this->node() ) ) { m_version = NodeValue<float>( version_tag.node() ); }

    return true;
}

} // end of namespace bdml

} // end of namespace kvs
