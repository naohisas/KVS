/*****************************************************************************/
/**
 *  @file   TransferFunctionTag.cpp
 */
/*----------------------------------------------------------------------------
 *
 *  Copyright (c) Visualization Laboratory, Kyoto University.
 *  All rights reserved.
 *  See http://www.viz.media.kyoto-u.ac.jp/kvs/copyright/ for details.
 *
 *  $Id: TransferFunctionTag.cpp 1345 2012-11-07 15:29:14Z s.yamada0808@gmail.com $
 */
/*****************************************************************************/
#include "TransferFunctionTag.h"
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
 *  @brief  Constructs a new TransferFunctionTag class.
 */
/*===========================================================================*/
TransferFunctionTag::TransferFunctionTag():
    kvs::kvsml::TagBase( "TransferFunction" ),
    m_has_file( false ),
    m_file( "" ),
    m_has_resolution( false ),
    m_resolution( 0 ),
    m_has_min_value( false ),
    m_min_value( 0 ),
    m_has_max_value( false ),
    m_max_value( 0 )
{
}

/*===========================================================================*/
/**
 *  @brief  Tests whether the file is specified or not.
 *  @return true, if the file is specified
 */
/*===========================================================================*/
bool TransferFunctionTag::hasFile() const
{
    return m_has_file;
}

/*===========================================================================*/
/**
 *  @brief  Returns a name of external file.
 *  @return filename
 */
/*===========================================================================*/
const std::string& TransferFunctionTag::file() const
{
    return m_file;
}

/*===========================================================================*/
/**
 *  @brief  Tests whether the resolution is specified or not.
 *  @return true, if the resolution is specified
 */
/*===========================================================================*/
bool TransferFunctionTag::hasResolution() const
{
    return m_has_resolution;
}

/*===========================================================================*/
/**
 *  @brief  Returns a resolution.
 *  @return resolution
 */
/*===========================================================================*/
size_t TransferFunctionTag::resolution() const
{
    return m_resolution;
}

/*===========================================================================*/
/**
 *  @brief  Tests whether the min_value is specified or not.
 *  @return true, if the min_value is specified
 */
/*===========================================================================*/
bool TransferFunctionTag::hasMinValue() const
{
    return m_has_min_value;
}

/*===========================================================================*/
/**
 *  @brief  Returns the min. scalar value.
 *  @return min. scalar value
 */
/*===========================================================================*/
float TransferFunctionTag::minValue() const
{
    return m_min_value;
}

/*===========================================================================*/
/**
 *  @brief  Tests whether the max_value is specified or not.
 *  @return true, if the max_value is specified
 */
/*===========================================================================*/
bool TransferFunctionTag::hasMaxValue() const
{
    return m_has_max_value;
}

/*===========================================================================*/
/**
 *  @brief  Returns the max. scalar value.
 *  @return max. scalar value
 */
/*===========================================================================*/
float TransferFunctionTag::maxValue() const
{
    return m_max_value;
}

/*===========================================================================*/
/**
 *  @brief  Sets a name of external file for 'argb' data.
 *  @param  file [in] filename
 */
/*===========================================================================*/
void TransferFunctionTag::setFile( const std::string& file )
{
    m_file = file;
}

/*===========================================================================*/
/**
 *  @brief  Sets a resolution.
 *  @param  resolution [in] resolution
 */
/*===========================================================================*/
void TransferFunctionTag::setResolution( const size_t resolution )
{
    m_has_resolution = true;
    m_resolution = resolution;
}

/*===========================================================================*/
/**
 *  @brief  Sets a min. scalar value.
 *  @param  min_value [in] min. scalar value
 */
/*===========================================================================*/
void TransferFunctionTag::setMinValue( const float min_value )
{
    m_has_min_value = true;
    m_min_value = min_value;
}

/*===========================================================================*/
/**
 *  @brief  Sets a max. scalar value.
 *  @param  max_value [in] max. scalar value
 */
/*===========================================================================*/
void TransferFunctionTag::setMaxValue( const float max_value )
{
    m_has_max_value = true;
    m_max_value = max_value;
}

/*===========================================================================*/
/**
 *  @brief  Reads a transfer function tag.
 *  @param  parent [in] pointer to the parent node
 *  @return true, if the reading process is done successfully
 */
/*===========================================================================*/
bool TransferFunctionTag::read( const kvs::XMLNode::SuperClass* parent )
{
    BaseClass::read( parent );

    // Element
    const kvs::XMLElement::SuperClass* element = kvs::XMLNode::ToElement( BaseClass::m_node );

    // resolution="xxx"
    const std::string resolution = kvs::XMLElement::AttributeValue( element, "resolution" );
    if ( resolution != "" )
    {
        m_has_resolution = true;
        m_resolution = static_cast<size_t>( atoi( resolution.c_str() ) );
    }

    // min_value="xxx"
    const std::string min_value = kvs::XMLElement::AttributeValue( element, "min_value" );
    if ( min_value != "" )
    {
        m_has_min_value = true;
        m_min_value = static_cast<float>( atof( min_value.c_str() ) );
    }

    // max_value="xxx"
    const std::string max_value = kvs::XMLElement::AttributeValue( element, "max_value" );
    if ( max_value != "" )
    {
        m_has_max_value = true;
        m_max_value = static_cast<float>( atof( max_value.c_str() ) );
    }

    // file="xxx"
    const std::string file = kvs::XMLElement::AttributeValue( element, "file" );
    if ( file != "" )
    {
        m_has_file = true;
        m_file = file;
    }

    return true;
}

/*===========================================================================*/
/**
 *  @brief  Writes a transfer function tag.
 *  @param  parent [in] pointer to the parent node
 *  @return true, if the writing process is done successfully
 */
/*===========================================================================*/
bool TransferFunctionTag::write( kvs::XMLNode::SuperClass* parent )
{
    kvs::XMLElement element( BaseClass::name() );

    // resolution="xxx"
    if ( m_has_resolution )
    {
        element.setAttribute( "resolution", m_resolution );
    }

    // min_value="xxx"
    if ( m_has_min_value )
    {
        element.setAttribute( "min_value", m_min_value );
    }

    // max_value="xxx"
    if ( m_has_max_value )
    {
        element.setAttribute( "max_value", m_max_value );
    }

    // file="xxx"
    if ( m_has_file )
    {
        element.setAttribute( "file", m_file );
    }

    return BaseClass::write_with_element( parent, element );
}

} // end of namespace kvsml

} // end of namespace kvs
