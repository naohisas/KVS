/*****************************************************************************/
/**
 *  @file   DataDescriptorFile.cpp
 *  @author Naohisa Sakamoto
 */
/*----------------------------------------------------------------------------
 *
 *  Copyright (c) Visualization Laboratory, Kyoto University.
 *  All rights reserved.
 *  See http://www.viz.media.kyoto-u.ac.jp/kvs/copyright/ for details.
 *
 *  $Id: DataDescriptorFile.cpp 1346 2012-11-08 11:57:55Z s.yamada0808@gmail.com $
 */
/*****************************************************************************/
#include "DataDescriptorFile.h"
#include <string>
#include <sstream>
#include <iostream>
#include <kvs/Message>
#include <kvs/Tokenizer>


namespace
{

bool GetLine( std::istream & in, std::string & str )
{
    if ( !in ) return( false );

    char ch;
    str = "";

    // Windows:CRLF(\r\n), Unix:LF(\n), Mac:CR(\r)
    while ( in.get( ch ) )
    {
        if ( ch == '\0' ) { break; }
        if ( ch == '\n' ) { break; }
        if ( ch == '\r' ) { ch = static_cast<char>( in.peek() ); if ( ch == '\n' ) in.get( ch ); break; }
        str += ch;
    }

    return( true );
}

}


namespace kvs
{

namespace grads
{

/*===========================================================================*/
/**
 *  @brief  Constructs a new DataDescriptorFile class.
 */
/*===========================================================================*/
DataDescriptorFile::DataDescriptorFile( void )
{
}

/*===========================================================================*/
/**
 *  @brief  Returns DSET entry information.
 *  @return DSET entry information
 */
/*===========================================================================*/
const kvs::grads::DSet& DataDescriptorFile::dset( void ) const
{
    return( m_dset );
}

/*===========================================================================*/
/**
 *  @brief  Returns UNDEF entry information.
 *  @return UNDEF entry information
 */
/*===========================================================================*/
const kvs::grads::Undef& DataDescriptorFile::undef( void ) const
{
    return( m_undef );
}

/*===========================================================================*/
/**
 *  @brief  Returns TITLE entry information.
 *  @return TITLE entry information
 */
/*===========================================================================*/
const kvs::grads::Title& DataDescriptorFile::title( void ) const
{
    return( m_title );
}

/*===========================================================================*/
/**
 *  @brief  Returns OPTIONS entry information.
 *  @return OPTIONS entry information
 */
/*===========================================================================*/
const kvs::grads::Options& DataDescriptorFile::options( void ) const
{
    return( m_options );
}

/*===========================================================================*/
/**
 *  @brief  Returns XDEF entry information.
 *  @return XDEF entry information
 */
/*===========================================================================*/
const kvs::grads::XDef& DataDescriptorFile::xdef( void ) const
{
    return( m_xdef );
}

/*===========================================================================*/
/**
 *  @brief  Returns YDEF entry information.
 *  @return YDEF entry information
 */
/*===========================================================================*/
const kvs::grads::YDef& DataDescriptorFile::ydef( void ) const
{
    return( m_ydef );
}

/*===========================================================================*/
/**
 *  @brief  Returns ZDEF entry information.
 *  @return ZDEF entry information
 */
/*===========================================================================*/
const kvs::grads::ZDef& DataDescriptorFile::zdef( void ) const
{
    return( m_zdef );
}

/*===========================================================================*/
/**
 *  @brief  Returns TDEF entry information.
 *  @return TDEF entry information
 */
/*===========================================================================*/
const kvs::grads::TDef& DataDescriptorFile::tdef( void ) const
{
    return( m_tdef );
}

/*===========================================================================*/
/**
 *  @brief  Returns VARS entry information.
 *  @return VARS entry information
 */
/*===========================================================================*/
const kvs::grads::Vars& DataDescriptorFile::vars( void ) const
{
    return( m_vars );
}

void DataDescriptorFile::print( std::ostream& os, const kvs::Indent& indent ) const
{
    os << indent << "Data filename (DSET) : " << m_dset.name << std::endl;
    os << indent << "Undefined value (UNDEF) : " << m_undef.value << std::endl;
    os << indent << "Title (TITLE) : " << m_title.name << std::endl;
    os << indent << "Number of grid points for the X direction (XDEF) : " << m_xdef.num << std::endl;
    os << indent << "Number of grid points for the Y direction (YDEF) : " << m_ydef.num << std::endl;
    os << indent << "Number of grid points for the Z direction (ZDEF) : " << m_zdef.num << std::endl;
    os << indent << "Number of time steps (TDEF) : " << m_tdef.num << std::endl;
    os << indent << "Number of variables (VARS) : " << m_vars.values.size() << std::endl;
    std::list<kvs::grads::Vars::Var>::const_iterator var = m_vars.values.begin();
    while ( var != m_vars.values.end() )
    {
        os << indent.nextIndent() << var->varname << ", " << var->description << std::endl;
        var++;
    }
    os << std::endl;
}

/*===========================================================================*/
/**
 *  @brief  Read data descriptor file.
 *  @param  ifs [in] input file stream
 *  @return true, if the reading process is done successfully
 */
/*===========================================================================*/
bool DataDescriptorFile::read( std::ifstream& ifs )
{
    std::string line;
//    while ( std::getline( ifs, line ) )
    while ( ::GetLine( ifs, line ) )
    {
        // Trim white space.
        std::string blank(" ");
        line.erase( 0, line.find_first_not_of( blank ) );

        // Comment.
        if ( line[0] == '@' ) continue;
        if ( line[0] == '*' ) continue;

        // Check entry.
        kvs::Tokenizer t( line, " \t\n" );
        std::string entry = t.token();

        if ( entry == "DSET" || entry == "dset" )
        {
            if ( !m_dset.read( line, ifs ) ) { kvsMessageError("Cannot read DSET."); return false; }
        }

        if ( entry == "UNDEF" || entry == "undef" )
        {
            if ( !m_undef.read( line, ifs ) ) { kvsMessageError("Cannot read UNDEF."); return false; }
        }

        if ( entry == "TITLE" || entry == "title" )
        {
            if ( !m_title.read( line, ifs ) ) { kvsMessageError("Cannot read TITLE."); return false; }
        }

        if ( entry == "OPTIONS" || entry == "options" )
        {
            if ( !m_options.read( line, ifs ) ) { kvsMessageError("Cannot read OPTIONS."); return false; }
        }

        if ( entry == "XDEF" || entry == "xdef" )
        {
            if ( !m_xdef.read( line, ifs ) ) { kvsMessageError("Cannot read XDEF."); return false; }
        }

        if ( entry == "YDEF" || entry == "ydef" )
        {
            if ( !m_ydef.read( line, ifs ) ) { kvsMessageError("Cannot read YDEF."); return false; }
        }

        if ( entry == "ZDEF" || entry == "zdef" )
        {
            if ( !m_zdef.read( line, ifs ) ) { kvsMessageError("Cannot read ZDEF."); return false; }
        }

        if ( entry == "TDEF" || entry == "tdef" )
        {
            if ( !m_tdef.read( line, ifs ) ) { kvsMessageError("Cannot read TDEF."); return false; }
        }

        if ( entry == "VARS" || entry == "vars" )
        {
            if ( !m_vars.read( line, ifs ) ) { kvsMessageError("Cannot read VARS."); return false; }
        }
    }

    return true;
}

} // end of namespace grads

} // end of namespace kvs
