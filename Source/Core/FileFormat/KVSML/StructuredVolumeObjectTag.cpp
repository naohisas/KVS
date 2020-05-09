/*****************************************************************************/
/**
 *  @file   StructuredVolumeObjectTag.cpp
 */
/*----------------------------------------------------------------------------
 *
 *  Copyright (c) Visualization Laboratory, Kyoto University.
 *  All rights reserved.
 *  See http://www.viz.media.kyoto-u.ac.jp/kvs/copyright/ for details.
 *
 *  $Id: StructuredVolumeObjectTag.cpp 1345 2012-11-07 15:29:14Z s.yamada0808@gmail.com $
 */
/*****************************************************************************/
#include "StructuredVolumeObjectTag.h"
#include <kvs/XMLNode>
#include <kvs/XMLElement>
#include <kvs/Tokenizer>
#include <kvs/String>
#include <sstream>


namespace
{

std::string Vec3ToString( const kvs::Vec3u& v )
{
    std::ostringstream s;
    s << v[0] << " " << v[1] << " " << v[2];
    return s.str();
}

}

namespace kvs
{

namespace kvsml
{

/*===========================================================================*/
/**
 *  @brief  Constructs a new structured volume object tag class.
 */
/*===========================================================================*/
StructuredVolumeObjectTag::StructuredVolumeObjectTag():
    kvs::kvsml::TagBase( "StructuredVolumeObject" ),
    m_has_grid_type( false ),
    m_grid_type( "" ),
    m_has_resolution( false ),
    m_resolution( 0, 0, 0 )
{
}

/*===========================================================================*/
/**
 *  @brief  Tests whether the 'grid_type' is specified or not.
 *  @return true, if the 'grid_type' is specified
 */
/*===========================================================================*/
bool StructuredVolumeObjectTag::hasGridType() const
{
    return m_has_grid_type;
}

/*===========================================================================*/
/**
 *  @brief  Returns a grid type as string.
 *  @return grid type
 */
/*===========================================================================*/
const std::string& StructuredVolumeObjectTag::gridType() const
{
    return m_grid_type;
}

/*===========================================================================*/
/**
 *  @brief  Tests whether the 'resolution' is specified or not.
 *  @return true, if the 'resolution' is specified
 */
/*===========================================================================*/
bool StructuredVolumeObjectTag::hasResolution() const
{
    return m_has_resolution;
}

/*===========================================================================*/
/**
 *  @brief  Returns a grid resulution.
 *  @return grid resolution
 */
/*===========================================================================*/
const kvs::Vector3ui& StructuredVolumeObjectTag::resolution() const
{
    return m_resolution;
}

/*===========================================================================*/
/**
 *  @brief  Sets a grid type as string.
 *  @param  grid_type [in] grid type
 */
/*===========================================================================*/
void StructuredVolumeObjectTag::setGridType( const std::string& grid_type )
{
    m_has_grid_type = true;
    m_grid_type = grid_type;
}

/*===========================================================================*/
/**
 *  @brief  Sets a grid resolution.
 *  @param  resolution [in] grid resolution
 */
/*===========================================================================*/
void StructuredVolumeObjectTag::setResolution( const kvs::Vector3ui& resolution )
{
    m_has_resolution = true;
    m_resolution = resolution;
}

/*===========================================================================*/
/**
 *  @brief  Reads the structured volume object tag.
 *  @param  parent [in] pointer to the parent node
 *  @return true, if the reading process is done successfully
 */
/*===========================================================================*/
bool StructuredVolumeObjectTag::read( const kvs::XMLNode::SuperClass* parent )
{
    BaseClass::read( parent );

    // Element
    const kvs::XMLElement::SuperClass* element = kvs::XMLNode::ToElement( BaseClass::m_node );

    // grid_type="xxx"
    const std::string grid_type = kvs::XMLElement::AttributeValue( element, "grid_type" );
    if ( grid_type != "" )
    {
        m_has_grid_type = true;
        m_grid_type = grid_type;
    }

    // resolution="xxx xxx xxx"
    const std::string resolution = kvs::XMLElement::AttributeValue( element, "resolution" );
    if ( resolution != "" )
    {
        const std::string delim(" \n");
        kvs::Tokenizer t( resolution, delim );

        unsigned int values[3];
        for ( size_t i = 0; i < 3; i++ )
        {
            if ( t.isLast() )
            {
                kvsMessageError( "3 components are required for 'resolution' in <%s>", this->name().c_str() );
                return false;
            }

            values[i] = static_cast<unsigned int>( atoi( t.token().c_str() ) );
        }

        m_has_resolution = true;
        m_resolution = kvs::Vec3u( values[0], values[1], values[2] );
    }

    return true;
}

/*===========================================================================*/
/**
 *  @brief  Writes the structured volume object tag.
 *  @param  parent [in] pointer to the parent node
 *  @return true, if the writing process is done successfully
 */
/*===========================================================================*/
bool StructuredVolumeObjectTag::write( kvs::XMLNode::SuperClass* parent )
{
    kvs::XMLElement element( BaseClass::name() );

    if ( m_has_grid_type )
    {
        element.setAttribute( "grid_type", m_grid_type );
    }
    else
    {
        kvsMessageError( "'grid_type' is not specified in <%s>.", BaseClass::name().c_str() );
        return false;
    }

    if ( m_has_resolution )
    {
        const std::string resolution = ::Vec3ToString( m_resolution );
        element.setAttribute( "resolution", resolution );
    }
    else
    {
        kvsMessageError( "'resolution' is not specified in <%s>.", BaseClass::name().c_str() );
        return false;
    }

    return BaseClass::write_with_element( parent, element );
}

} // end of namespace kvsml

} // end of namespace kvs
