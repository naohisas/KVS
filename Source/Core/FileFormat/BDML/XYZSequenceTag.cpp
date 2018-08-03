/*****************************************************************************/
/**
 *  @file   XYZSequenceTag.cpp
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
#include "XYZSequenceTag.h"
#include "XML.h"
#include "XYZTag.h"
#include <vector>


namespace kvs
{

namespace bdml
{

XYZSequenceTag::XYZSequenceTag():
    Tag("xyzSequence")
{
}

void XYZSequenceTag::print( std::ostream& os, const kvs::Indent& indent ) const
{
    os << indent << "Number of xyz: " << m_xyz.size() / 3 << std::endl;
}

bool XYZSequenceTag::read( const Node* node )
{
    std::vector<float> data;

    XYZTag tag;
    Node* this_node = FindNode( node, tag.name() );
    while ( this_node != NULL )
    {
        tag.read( this_node );
        data.push_back( tag.x );
        data.push_back( tag.y );
        data.push_back( tag.z );
//        this_node = this->node()->IterateChildren( tag.name(), this_node );
        this_node = this_node->NextSibling( tag.name() );
    }

    m_xyz = kvs::ValueArray<float>( data );

    return true;
}

} // end of namespace bdml

} // end of namespace kvs
