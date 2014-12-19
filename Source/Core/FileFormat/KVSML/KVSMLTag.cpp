/*****************************************************************************/
/**
 *  @file   KVSMLTag.cpp
 */
/*----------------------------------------------------------------------------
 *
 *  Copyright (c) Visualization Laboratory, Kyoto University.
 *  All rights reserved.
 *  See http://www.viz.media.kyoto-u.ac.jp/kvs/copyright/ for details.
 *
 *  $Id: KVSMLTag.cpp 1812 2014-09-11 07:34:35Z naohisa.sakamoto@gmail.com $
 */
/*****************************************************************************/
#include "KVSMLTag.h"
#include <kvs/Message>
#include <kvs/XMLDocument>
#include <kvs/XMLNode>
#include <kvs/XMLElement>
#include <kvs/IgnoreUnusedVariable>


namespace kvs
{

namespace kvsml
{

/*===========================================================================*/
/**
 *  @brief  Constructs a new KVSML tag class.
 */
/*===========================================================================*/
KVSMLTag::KVSMLTag():
    kvs::kvsml::TagBase( "KVSML" ),
    m_version( "version" )
{
}

/*===========================================================================*/
/**
 *  @brief  Reads the KVSML tag.
 *  @param  document [in] pointer to the XML document
 *  @return true, if the reading process is done successfully.
 */
/*===========================================================================*/
bool KVSMLTag::read( const kvs::XMLDocument* document )
{
    BaseClass::read( document );

    // Element
    const kvs::XMLElement::SuperClass* element = kvs::XMLNode::ToElement( BaseClass::m_node );

    // version="xxx"
    const std::string version = kvs::XMLElement::AttributeValue( element, "version" );
    if ( !version.empty() )
    {
        m_version = version;
    }

    return true;
}

/*===========================================================================*/
/**
 *  @brief  Writes the KVSML tag.
 *  @param  document [in] pointer to the XML document
 *  @return true, if the writing process is done successfully.
 */
/*===========================================================================*/
bool KVSMLTag::write( kvs::XMLDocument* document )
{
    kvs::XMLElement element( BaseClass::name() );

    if ( m_version.hasValue() )
    {
        element.setAttribute( "version", m_version );
    }

    BaseClass::write_with_element( document, element );
    return true;
}

} // end of namespace kvsml

} // end of namespace kvs
