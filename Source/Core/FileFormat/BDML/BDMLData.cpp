/*****************************************************************************/
/**
 *  @file   BDMLData.cpp
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
#include "BDMLData.h"
#include "XML.h"
#include <kvs/IgnoreUnusedVariable>
#include <kvs/File>


namespace kvs
{

bool BDMLData::CheckExtension( const std::string& filename )
{
    const kvs::File file( filename );
    if ( file.extension() == "xml" )
    {
        return true;
    }

    return false;
}

BDMLData::BDMLData( const std::string& filename )
{
    this->read( filename );
}

void BDMLData::print( std::ostream& os, const kvs::Indent& indent ) const
{
    os << indent << "Filename: " << BaseClass::filename() << std::endl;
    os << indent << "BDML:" << std::endl;
    m_bdml.print( os, indent.nextIndent() );
    os << indent << "Information:" << std::endl;
    m_info.print( os, indent.nextIndent() );
    os << indent << "Data:" << std::endl;
    m_data.print( os, indent.nextIndent() );
}

bool BDMLData::read( const std::string& filename )
{
    BaseClass::setFilename( filename );
    BaseClass::setSuccess( true );

    kvs::XMLDocument document;
    if ( !document.read( filename ) )
    {
        kvsMessageError( "%s", document.ErrorDesc().c_str() );
        BaseClass::setSuccess( false );
        return false;
    }

    if ( !m_bdml.read( &document ) )
    {
        kvsMessageError( "Cannot read <%s>.", m_bdml.name().c_str() );
        BaseClass::setSuccess( false );
        return false;
    }

    if ( !m_info.read( m_bdml.node() ) )
    {
        kvsMessageError( "Cannot read <%s>.", m_info.name().c_str() );
        BaseClass::setSuccess( false );
        return false;
    }

    if ( !m_data.read( m_bdml.node() ) )
    {
        kvsMessageError( "Cannot read <%s>.", m_data.name().c_str() );
        BaseClass::setSuccess( false );
        return false;
    }

    return true;
}

bool BDMLData::write( const std::string& filename )
{
    kvs::IgnoreUnusedVariable( filename );
    return true;
}

} // end of namespace kvs
