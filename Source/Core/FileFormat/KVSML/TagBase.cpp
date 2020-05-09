/*****************************************************************************/
/**
 *  @file   TagBase.cpp
 */
/*----------------------------------------------------------------------------
 *
 *  Copyright (c) Visualization Laboratory, Kyoto University.
 *  All rights reserved.
 *  See http://www.viz.media.kyoto-u.ac.jp/kvs/copyright/ for details.
 *
 *  $Id: TagBase.cpp 1345 2012-11-07 15:29:14Z s.yamada0808@gmail.com $
 */
/*****************************************************************************/
#include "TagBase.h"
#include <kvs/XMLElement>
#include <kvs/Exception>


namespace kvs
{

namespace kvsml
{

/*===========================================================================*/
/**
 *  @brief  Constructs a new TagBase class.
 *  @param  name [in] tag name
 */
/*===========================================================================*/
TagBase::TagBase( const std::string& name ):
    m_name( name ),
    m_node( NULL )
{
}

/*===========================================================================*/
/**
 *  @brief  Returns a tag name.
 *  @return tag name
 */
/*===========================================================================*/
const std::string& TagBase::name() const
{
    return m_name;
}

/*===========================================================================*/
/**
 *  @brief  Returns a pointer to the node without 'const'.
 *  @return pointer to the node
 */
/*===========================================================================*/
kvs::XMLNode::SuperClass* TagBase::node()
{
    return m_node;
}

/*===========================================================================*/
/**
 *  @brief  Returns a pointer to the node with 'const'.
 *  @return pointer to the node
 */
/*===========================================================================*/
const kvs::XMLNode::SuperClass* TagBase::node() const
{
    return m_node;
}

/*===========================================================================*/
/**
 *  @brief  Check whether this node is existed or not in the parent node.
 *  @param  parent [in] pointer to the parent node
 *  @return true, if the node is existed
 */
/*===========================================================================*/
bool TagBase::isExisted( const kvs::XMLNode::SuperClass* parent ) const
{
    const std::string tag_name = this->name();
    return kvs::XMLNode::FindChildNode( parent, tag_name ) != NULL;
}

/*===========================================================================*/
/**
 *  @brief  Reads the coord tag.
 *  @param  parent [in] pointer to the parent node
 *  @return true, if the reading process is done successfully
 */
/*===========================================================================*/
bool TagBase::read( const kvs::XMLNode::SuperClass* parent )
{
    m_node = kvs::XMLNode::FindChildNode( parent, m_name );
    if ( !m_node )
    {
        KVS_THROW( kvs::FileReadFaultException, "Cannot find <" + m_name + "> in <" + parent->Value() + ">." );
    }

    return true;
}

/*===========================================================================*/
/**
 *  @brief  Writes the coord tag.
 *  @param  parent [in] pointer to the parent node
 *  @return true, if the writing process is done successfully
 */
/*===========================================================================*/
bool TagBase::write( kvs::XMLNode::SuperClass* parent )
{
    kvs::XMLElement element( m_name );
    return this->write_with_element( parent, element );
}

bool TagBase::write_with_element( kvs::XMLNode::SuperClass* parent, const kvs::XMLElement& element )
{
    m_node = parent->InsertEndChild( element );
    if ( !m_node )
    {
        KVS_THROW( kvs::FileWriteFaultException, "Cannot insert <" + m_name + "> to <" + parent->Value() + ">." );
    }

    return true;
}

} // end of namespace kvsml

} // end of namespace kvs
