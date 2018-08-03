/*****************************************************************************/
/**
 *  @file   Tag.cpp
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
#include "Tag.h"


namespace kvs
{

namespace bdml
{

Tag::Tag( const std::string& name )
{
    m_name = name;
    m_node = NULL;
}

Tag::Tag( const Node* parent, const std::string& name )
{
    m_name = name;
    this->read( parent );
}

bool Tag::read( const Node* parent )
{
    m_node = FindNode( parent, m_name );
    return m_node != NULL;
}

bool Tag::exists( const Node* parent ) const
{
    return FindNode( parent, m_name ) != NULL;
}

bool Tag::exists() const
{
    return m_node != NULL;
}

} // end of namespace bdml

} // end of namespace kvs
