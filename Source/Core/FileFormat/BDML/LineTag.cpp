/*****************************************************************************/
/**
 *  @file   LineTag.cpp
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
#include "LineTag.h"
#include "XML.h"


namespace kvs
{

namespace bdml
{

LineTag::LineTag():
    Tag("line")
{
}

void LineTag::print( std::ostream& os, const kvs::Indent& indent ) const
{
    os << indent << "Number of xyz sequences: " << m_xyzSequences.size() << std::endl;
    for ( size_t i = 0; i < m_xyzSequences.size(); i++ )
    {
        os << indent << "XYZ sequence[" << i << "]:" << std::endl;
        m_xyzSequences[i].print( os, indent.nextIndent() );
    }
}

bool LineTag::read( const Node* parent )
{
    if ( !Tag::read( parent ) ) { return false; }

    Node* this_node = FindNode( this->node(), XYZSequenceTag().name() );
    while ( this_node )
    {
        XYZSequenceTag tag;
        tag.read( this_node );
        m_xyzSequences.push_back( tag );
//        this_node = this->node()->IterateChildren( tag.name(), this_node );
        this_node = this_node->NextSibling( tag.name() );
    }

    return true;
}

} // end of namespace bdml

} // end of namespace kvs
