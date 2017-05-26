/*****************************************************************************/
/**
 *  @file   MeasurementTag.cpp
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
#include "MeasurementTag.h"
#include "XML.h"


namespace kvs
{

namespace bdml
{

MeasurementTag::MeasurementTag():
    Tag("measurement")
{
    m_objectRef = "";
}

void MeasurementTag::print( std::ostream& os, const kvs::Indent& indent ) const
{
    os << indent << "Object ref.: " << m_objectRef << std::endl;

    if ( m_line.exists() )
    {
        os << indent << "Line entity:" << std::endl;
        m_line.print( os, indent.nextIndent() );
    }

    if ( m_property.exists() )
    {
        os << indent << "Property:" << std::endl;
        m_property.print( os, indent.nextIndent() );
    }
}

bool MeasurementTag::read( const Node* parent )
{
    if ( !Tag::read( parent ) ) { return false; }

    Tag objectRef_tag("objectRef");
    if ( objectRef_tag.read( this->node() ) ) { m_objectRef = NodeValue<std::string>( objectRef_tag.node() ); }

    if ( m_line.exists( this->node() ) ) { if ( !m_line.read( this->node() ) ) { return false; } }
    if ( m_property.exists( this->node() ) ) { if ( !m_property.read( this->node() ) ) { return false; } }

    return true;
}

} // end of namespace bdml

} // end of namespace kvs
