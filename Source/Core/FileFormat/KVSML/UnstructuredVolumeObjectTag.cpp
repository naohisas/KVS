/*****************************************************************************/
/**
 *  @file   UnstructuredVolumeObjectTag.cpp
 */
/*----------------------------------------------------------------------------
 *
 *  Copyright (c) Visualization Laboratory, Kyoto University.
 *  All rights reserved.
 *  See http://www.viz.media.kyoto-u.ac.jp/kvs/copyright/ for details.
 *
 *  $Id: UnstructuredVolumeObjectTag.cpp 1345 2012-11-07 15:29:14Z s.yamada0808@gmail.com $
 */
/*****************************************************************************/
#include "UnstructuredVolumeObjectTag.h"
#include <kvs/XMLNode>
#include <kvs/XMLElement>
#include <kvs/Tokenizer>
#include <kvs/String>


namespace kvs
{

namespace kvsml
{

/*===========================================================================*/
/**
 *  @brief  Constructs a new unstructured volume object tag class.
 */
/*===========================================================================*/
UnstructuredVolumeObjectTag::UnstructuredVolumeObjectTag():
    kvs::kvsml::TagBase( "UnstructuredVolumeObject" ),
    m_cell_type( "cell_type" )
{
}

/*===========================================================================*/
/**
 *  @brief  Reads the unstructured volume object tag.
 *  @param  parent [in] pointer to the parent node
 *  @return true, if the reading process is done successfully
 */
/*===========================================================================*/
bool UnstructuredVolumeObjectTag::read( const kvs::XMLNode::SuperClass* parent )
{
    BaseClass::read( parent );

    // Element
    const kvs::XMLElement::SuperClass* element = kvs::XMLNode::ToElement( BaseClass::m_node );

    // cell_type="xxx" (required)
    const std::string cell_type = kvs::XMLElement::AttributeValue( element, "cell_type" );
    if ( cell_type != "" ) { m_cell_type = cell_type; }
    else
    {
        kvsMessageError( "'cell_type' is not specified in <%s>.", BaseClass::name().c_str() );
        return false;
    }

    return true;
}

/*===========================================================================*/
/**
 *  @brief  Writes the unstructured volume object tag.
 *  @param  parent [in] pointer to the parent node
 *  @return true, if the writing process is done successfully
 */
/*===========================================================================*/
bool UnstructuredVolumeObjectTag::write( kvs::XMLNode::SuperClass* parent )
{
    // Element
    kvs::XMLElement element( BaseClass::name() );

    // cell_type="xxx" (required)
    if ( this->hasCellType() ) { element.setAttribute( "cell_type", m_cell_type ); }
    else
    {
        kvsMessageError( "'cell_type' is not specified in <%s>.", BaseClass::name().c_str() );
        return false;
    }

    return BaseClass::write_with_element( parent, element );
}

} // end of namespace kvsml

} // end of namespace kvs
