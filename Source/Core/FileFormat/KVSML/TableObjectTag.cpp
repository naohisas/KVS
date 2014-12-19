/*****************************************************************************/
/**
 *  @file   TableObjectTag.cpp
 *  @author Naohisa Sakamoto
 */
/*----------------------------------------------------------------------------
 *
 *  Copyright (c) Visualization Laboratory, Kyoto University.
 *  All rights reserved.
 *  See http://www.viz.media.kyoto-u.ac.jp/kvs/copyright/ for details.
 *
 *  $Id: TableObjectTag.cpp 1345 2012-11-07 15:29:14Z s.yamada0808@gmail.com $
 */
/*****************************************************************************/
#include "TableObjectTag.h"
#include <kvs/Message>
#include <kvs/String>
#include <kvs/XMLElement>


namespace kvs
{

namespace kvsml
{

/*===========================================================================*/
/**
 *  @brief  Constructs a new TagBase class.
 */
/*===========================================================================*/
TableObjectTag::TableObjectTag():
    kvs::kvsml::TagBase( "TableObject" ),
    m_has_nrows( false ),
    m_nrows( 0 ),
    m_has_ncolumns( false ),
    m_ncolumns( 0 )
{
}

/*===========================================================================*/
/**
 *  @brief  Returns true if the nrows has been specified.
 *  @return true if the nrows has been specified
 */
/*===========================================================================*/
bool TableObjectTag::hasNRows() const
{
    return m_has_nrows;
}

/*===========================================================================*/
/**
 *  @brief  Returns number of rows.
 *  @return number of rows
 */
/*===========================================================================*/
size_t TableObjectTag::nrows() const
{
    return m_nrows;
}

/*===========================================================================*/
/**
 *  @brief  Returns true if the ncolumns has been specified.
 *  @return true if the ncolumns has been specified
 */
/*===========================================================================*/
bool TableObjectTag::hasNColumns() const
{
    return m_has_ncolumns;
}

/*===========================================================================*/
/**
 *  @brief  Returns number of columns.
 *  @return number of columns
 */
/*===========================================================================*/
size_t TableObjectTag::ncolumns() const
{
    return m_ncolumns;
}

/*===========================================================================*/
/**
 *  @brief  Sets a number of rows.
 *  @param  nrows [in] number of rows
 */
/*===========================================================================*/
void TableObjectTag::setNRows( const size_t nrows )
{
    m_has_nrows = true;
    m_nrows = nrows;
}

/*===========================================================================*/
/**
 *  @brief  Sets a number of columns.
 *  @param  nrows [in] number of columns
 */
/*===========================================================================*/
void TableObjectTag::setNColumns( const size_t ncolumns )
{
    m_has_ncolumns = true;
    m_ncolumns = ncolumns;
}

/*===========================================================================*/
/**
 *  @brief  Reads the table data.
 *  @param  parent [in] pointer to the parent node
 *  @return true, if the reading process is done successfully
 */
/*===========================================================================*/
bool TableObjectTag::read( const kvs::XMLNode::SuperClass* parent )
{
    BaseClass::read( parent );

    // Element
    const kvs::XMLElement::SuperClass* element = kvs::XMLNode::ToElement( BaseClass::m_node );

    // nrows="xxx"
    const std::string nrows = kvs::XMLElement::AttributeValue( element, "nrows" );
    if ( nrows != "" )
    {
        m_has_nrows = true;
        m_nrows = static_cast<size_t>( atoi( nrows.c_str() ) );
    }

    // ncolumns="xxx"
    const std::string ncolumns = kvs::XMLElement::AttributeValue( element, "ncolumns" );
    if ( ncolumns != "" )
    {
        m_has_ncolumns = true;
        m_ncolumns = static_cast<size_t>( atoi( ncolumns.c_str() ) );
    }

    return true;
}

/*===========================================================================*/
/**
 *  @brief  Writes the table data.
 *  @param  parent [in] pointer to the parent node
 *  @return true, if the writting process is done successfully
 */
/*===========================================================================*/
bool TableObjectTag::write( kvs::XMLNode::SuperClass* parent )
{
    kvs::XMLElement element( BaseClass::name() );

    if ( m_has_nrows )
    {
        element.setAttribute( "nrows", m_nrows );
    }
    else
    {
        kvsMessageError( "'nrows' is not specified in <%s>.", BaseClass::name().c_str() );
        return false;
    }

    if ( m_has_ncolumns )
    {
        element.setAttribute( "ncolumns", m_ncolumns );
    }
    else
    {
        kvsMessageError( "'ncolumns' is not specified in <%s>.", BaseClass::name().c_str() );
        return false;
    }

    return BaseClass::write_with_element( parent, element );
}

} // end of namespace kvsml

} // end of namespace kvs
