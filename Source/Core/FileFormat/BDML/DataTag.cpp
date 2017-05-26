/*****************************************************************************/
/**
 *  @file   DataTag.cpp
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
#include "DataTag.h"
#include "XML.h"


namespace kvs
{

namespace bdml
{

DataTag::DataTag() : Tag("data")
{
}

void DataTag::print( std::ostream& os, const kvs::Indent& indent ) const
{
    if ( m_scale_unit.exists() )
    {
        os << indent << "Scale unit:" << std::endl;
        m_scale_unit.print( os, indent.nextIndent() );
    }

    if ( m_feature.exists() )
    {
        os << indent << "Feature:" << std::endl;
        m_feature.print( os, indent.nextIndent() );
    }

    os << indent << "Number of components: " << m_components.size() << std::endl;
    for ( size_t i = 0; i < m_components.size(); i++ )
    {
        os << indent << "Component[" << i << "]:" << std::endl;
        m_components[i].print( os, indent.nextIndent() );
    }
}

bool DataTag::read( const Node* parent )
{
    if ( !Tag::read( parent ) ) { return false; }

    if ( m_scale_unit.exists( this->node() ) ) { if ( !m_scale_unit.read( this->node() ) ) { return false; } }
    if ( m_feature.exists( this->node() ) ) { if ( !m_feature.read( this->node() ) ) { return false; } }

    Node* this_node = FindNode( this->node(), ComponentTag().name() );
    while ( this_node )
    {
        ComponentTag tag;
        tag.read( this_node );
        m_components.push_back( tag );
//        this_node = this->node()->IterateChildren( tag.name(), this_node );
        this_node = this_node->NextSibling( tag.name() );
    }

    return true;
}

} // end of namespace bdml

} // end of namespace kvs
