/*****************************************************************************/
/**
 *  @file   ScaleUnitTag.cpp
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
#include "ScaleUnitTag.h"
#include "XML.h"


namespace kvs
{

namespace bdml
{

ScaleUnitTag::ScaleUnitTag() : Tag("scaleUnit")
{
    m_x_scale = 0.0f;
    m_y_scale = 0.0f;
    m_z_scale = 0.0f;
    m_xyz_unit = "";
    m_t_scale = 0.0f;
    m_t_unit = "";
}

void ScaleUnitTag::print( std::ostream& os, const kvs::Indent& indent ) const
{
    os << indent << "X scale: " << m_x_scale << std::endl;
    os << indent << "Y scale: " << m_y_scale << std::endl;
    os << indent << "Z scale: " << m_z_scale << std::endl;
    os << indent << "XYZ unit: " << m_xyz_unit << std::endl;
}

bool ScaleUnitTag::read( const Node* parent )
{
    if ( !Tag::read( parent ) ) { return false; }

    Tag xScale_tag("xScale");
    if ( xScale_tag.read( this->node() ) ) { m_x_scale = NodeValue<float>( xScale_tag.node() ); }

    Tag yScale_tag("yScale");
    if ( yScale_tag.read( this->node() ) ) { m_y_scale = NodeValue<float>( yScale_tag.node() ); }

    Tag zScale_tag("zScale");
    if ( zScale_tag.read( this->node() ) ) { m_z_scale = NodeValue<float>( zScale_tag.node() ); }

    Tag xyzUnit_tag("xyzUnit");
    if ( xyzUnit_tag.read( this->node() ) ) { m_xyz_unit = NodeValue<std::string>( xyzUnit_tag.node() ); }

    Tag tScale_tag("tScale");
    if ( tScale_tag.read( this->node() ) ) { m_t_scale = NodeValue<float>( tScale_tag.node() ); }

    Tag tUnit_tag("tUnit");
    if ( tUnit_tag.read( this->node() ) ) { m_t_unit = NodeValue<std::string>( tUnit_tag.node() ); }

    return true;
}

} // end of namespace bdml

} // end of namespace kvs
