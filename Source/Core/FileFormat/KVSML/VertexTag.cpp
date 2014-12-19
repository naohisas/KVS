/*****************************************************************************/
/**
 *  @file   VertexTag.cpp
 */
/*----------------------------------------------------------------------------
 *
 *  Copyright (c) Visualization Laboratory, Kyoto University.
 *  All rights reserved.
 *  See http://www.viz.media.kyoto-u.ac.jp/kvs/copyright/ for details.
 *
 *  $Id: VertexTag.cpp 1345 2012-11-07 15:29:14Z s.yamada0808@gmail.com $
 */
/*****************************************************************************/
#include "VertexTag.h"
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
 *  @brief  Constructs a new vertex tag class.
 */
/*===========================================================================*/
VertexTag::VertexTag():
    kvs::kvsml::TagBase( "Vertex" ),
    m_has_nvertices( false ),
    m_nvertices( 0 )
{
}

/*===========================================================================*/
/**
 *  @brief  Tests whether the vertex tag has the 'nvertices' attribute value.
 *  @return true, if the vertex tag has the 'nvertices' attribute value
 */
/*===========================================================================*/
bool VertexTag::hasNVertices() const
{
    return m_has_nvertices;
}

/*===========================================================================*/
/**
 *  @brief  Returns a number of vertices.
 *  @return number of vertices
 */
/*===========================================================================*/
size_t VertexTag::nvertices() const
{
    return m_nvertices;
}

/*===========================================================================*/
/**
 *  @brief  Sets a number of vertices.
 *  @param  nvertices [in] number of vertices
 */
/*===========================================================================*/
void VertexTag::setNVertices( const size_t nvertices )
{
    m_has_nvertices = true;
    m_nvertices = nvertices;
}

/*===========================================================================*/
/**
 *  @brief  Reads the vertex tag.
 *  @param  parent [in] pointer to the parent node
 *  @return true, if the reading process is done successfully
 */
/*===========================================================================*/
bool VertexTag::read( const kvs::XMLNode::SuperClass* parent )
{
    BaseClass::read( parent );

    // Element
    const kvs::XMLElement::SuperClass* element = kvs::XMLNode::ToElement( BaseClass::m_node );

    // nvertices="xxx"
    const std::string nvertices = kvs::XMLElement::AttributeValue( element, "nvertices" );
    if ( nvertices != "" )
    {
        m_has_nvertices = true;
        m_nvertices = static_cast<size_t>( atoi( nvertices.c_str() ) );
    }

    return true;
}

/*===========================================================================*/
/**
 *  @brief  Writes the vertex tag.
 *  @param  parent [in] pointer to the parent node
 *  @return true, if the writing process is done successfully
 */
/*===========================================================================*/
bool VertexTag::write( kvs::XMLNode::SuperClass* parent )
{
    kvs::XMLElement element( BaseClass::name() );

    if ( m_has_nvertices )
    {
        element.setAttribute( "nvertices", m_nvertices );
    }

    return BaseClass::write_with_element( parent, element );
}

} // end of namespace kvsml

} // end of namespace kvs
