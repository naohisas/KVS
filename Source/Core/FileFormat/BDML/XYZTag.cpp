/*****************************************************************************/
/**
 *  @file   XYZTag.cpp
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
#include "XYZTag.h"
#include "XML.h"


namespace kvs
{

namespace bdml
{

XYZTag::XYZTag() : Tag("xyz")
{
    x = 0.0f;
    y = 0.0f;
    z = 0.0f;
}

void XYZTag::print( std::ostream& os, const kvs::Indent& indent ) const
{
    os << indent << "x: " << x << std::endl;
    os << indent << "y: " << y << std::endl;
    os << indent << "z: " << z << std::endl;
}

bool XYZTag::read( const Node* node )
{
    if ( !node ) { return false; }

    Tag x_tag("x");
    if ( x_tag.read( node ) ) { x = NodeValue<float>( x_tag.node() ); }

    Tag y_tag("y");
    if ( y_tag.read( node ) ) { y = NodeValue<float>( y_tag.node() ); }

    Tag z_tag("z");
    if ( z_tag.read( node ) ) { z = NodeValue<float>( z_tag.node() ); }

    return true;
}

} // end of namespace bdml

} // end of namespace kvs
