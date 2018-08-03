/*****************************************************************************/
/**
 *  @file   NodeTag.cpp
 *  @author Naohisa Sakamoto
 */
/*----------------------------------------------------------------------------
 *
 *  Copyright (c) Visualization Laboratory, Kyoto University.
 *  All rights reserved.
 *  See http://www.viz.media.kyoto-u.ac.jp/kvs/copyright/ for details.
 *
 *  $Id: NodeTag.cpp 1812 2014-09-11 07:34:35Z naohisa.sakamoto@gmail.com $
 */
/*****************************************************************************/
#include "NodeTag.h"
#include <kvs/Message>
#include <kvs/String>
#include <kvs/XMLNode>
#include <kvs/XMLElement>


namespace kvs
{

namespace kvsml
{

/*===========================================================================*/
/**
 *  @brief  Constructs a new node tag class.
 */
/*===========================================================================*/
NodeTag::NodeTag():
    kvs::kvsml::TagBase( "Node" ),
    m_nnodes( "nnodes" )
{
}

/*===========================================================================*/
/**
 *  @brief  Reads the node tag.
 *  @param  parent [in] pointer to the parent node
 *  @return true, if the reading process is done successfully
 */
/*===========================================================================*/
bool NodeTag::read( const kvs::XMLNode::SuperClass* parent )
{
    BaseClass::read( parent );

    // Element
    const kvs::XMLElement::SuperClass* element = kvs::XMLNode::ToElement( BaseClass::m_node );

    // nnodes="xxx"
    const std::string nnodes = kvs::XMLElement::AttributeValue( element, "nnodes" );
    if ( nnodes != "" ) { m_nnodes = static_cast<size_t>( atoi( nnodes.c_str() ) ); }

    return true;
}

/*===========================================================================*/
/**
 *  @brief  Writes the node tag.
 *  @param  parent [in] pointer to the parent node
 *  @return true, if the writing process is done successfully
 */
/*===========================================================================*/
bool NodeTag::write( kvs::XMLNode::SuperClass* parent )
{
    kvs::XMLElement element( BaseClass::name() );

    if ( m_nnodes.hasValue() ) { element.setAttribute( "nnodes", m_nnodes ); }

    return BaseClass::write_with_element( parent, element );
}

} // end of namespace kvsml

} // end of namespace kvs
