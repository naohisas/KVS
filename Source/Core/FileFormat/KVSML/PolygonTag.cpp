/*****************************************************************************/
/**
 *  @file   PolygonTag.cpp
 */
/*----------------------------------------------------------------------------
 *
 *  Copyright (c) Visualization Laboratory, Kyoto University.
 *  All rights reserved.
 *  See http://www.viz.media.kyoto-u.ac.jp/kvs/copyright/ for details.
 *
 *  $Id: PolygonTag.cpp 1345 2012-11-07 15:29:14Z s.yamada0808@gmail.com $
 */
/*****************************************************************************/
#include "PolygonTag.h"
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
 *  @brief  Constructs a new polygon tag class.
 */
/*===========================================================================*/
PolygonTag::PolygonTag():
    kvs::kvsml::TagBase( "Polygon" ),
    m_has_npolygons( false ),
    m_npolygons( 0 )
{
}

/*===========================================================================*/
/**
 *  @brief  Tests whether the polygon tag has the 'npolygons' attribute value.
 *  @return true, if the polygon tag has the 'npolygons' attribute value
 */
/*===========================================================================*/
bool PolygonTag::hasNPolygons() const
{
    return m_has_npolygons;
}

/*===========================================================================*/
/**
 *  @brief  Returns a number of polygons.
 *  @return number of polygons
 */
/*===========================================================================*/
size_t PolygonTag::npolygons() const
{
    return m_npolygons;
}

/*===========================================================================*/
/**
 *  @brief  Sets a number of polygons.
 *  @param  npolygons [in] number of polygons
 */
/*===========================================================================*/
void PolygonTag::setNPolygons( const size_t npolygons )
{
    m_has_npolygons = true;
    m_npolygons = npolygons;
}

/*===========================================================================*/
/**
 *  @brief  Reads the polygon tag.
 *  @param  parent [in] pointer to the parent node
 *  @return true, if the reading process is done successfully
 */
/*===========================================================================*/
bool PolygonTag::read( const kvs::XMLNode::SuperClass* parent )
{
    BaseClass::read( parent );

    // Element
    const kvs::XMLElement::SuperClass* element = kvs::XMLNode::ToElement( BaseClass::m_node );

    // npolygons="xxx"
    const std::string npolygons = kvs::XMLElement::AttributeValue( element, "npolygons" );
    if ( npolygons != "" )
    {
        m_has_npolygons = true;
        m_npolygons = static_cast<size_t>( atoi( npolygons.c_str() ) );
    }

    return true;
}

/*===========================================================================*/
/**
 *  @brief  Writes the polygon tag.
 *  @param  parent [in] pointer to the parent node
 *  @return true, if the writing process is done successfully
 */
/*===========================================================================*/
bool PolygonTag::write( kvs::XMLNode::SuperClass* parent )
{
    kvs::XMLElement element( BaseClass::name() );

    if ( m_has_npolygons )
    {
        element.setAttribute( "npolygons", m_npolygons );
    }

    return BaseClass::write_with_element( parent, element );
}

} // end of namespace kvsml

} // end of namespace kvs
