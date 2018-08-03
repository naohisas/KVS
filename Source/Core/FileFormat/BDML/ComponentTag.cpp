/*****************************************************************************/
/**
 *  @file   ComponentTag.cpp
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
#include "ComponentTag.h"
#include "XML.h"


namespace kvs
{

namespace bdml
{

ComponentTag::ComponentTag() : Tag("component")
{
    m_componentID = "";
    m_componentName = "";
    m_time = 0.0f;
    m_prevID = "";
}

void ComponentTag::print( std::ostream& os, const kvs::Indent& indent ) const
{
    os << indent << "Component ID: " << m_componentID << std::endl;
    os << indent << "Component Name: " << m_componentName << std::endl;
    os << indent << "Time: " << m_time << std::endl;
    os << indent << "Prev. ID: " << m_prevID << std::endl;
    m_measurement.print( os, indent );
}

bool ComponentTag::read( const Node* node )
{
    Tag componentID_tag("componentID");
    if ( componentID_tag.read( node ) ) { m_componentID = NodeValue<std::string>( componentID_tag.node() ); }

    Tag componentName_tag("componentName");
    if ( componentName_tag.read( node ) ) { m_componentName = NodeValue<std::string>( componentName_tag.node() ); }

    Tag time_tag("time");
    if ( time_tag.read( node ) ) { m_time = NodeValue<float>( time_tag.node() ); }

    Tag prevID_tag("prevID");
    if ( prevID_tag.read( node ) ) { m_prevID = NodeValue<std::string>( prevID_tag.node() ); }

    if ( !m_measurement.read( node ) ) { return false; }

    return true;
}

} // end of namespace bdml

} // end of namespace kvs
