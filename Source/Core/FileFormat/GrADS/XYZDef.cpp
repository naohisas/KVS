/*****************************************************************************/
/**
 *  @file   XYZDef.cpp
 *  @author Naohisa Sakamoto
 */
/*----------------------------------------------------------------------------
 *
 *  Copyright (c) Visualization Laboratory, Kyoto University.
 *  All rights reserved.
 *  See http://www.viz.media.kyoto-u.ac.jp/kvs/copyright/ for details.
 *
 *  $Id: XYZDef.cpp 1303 2012-09-14 11:26:37Z naohisa.sakamoto@gmail.com $
 */
/*****************************************************************************/
#include "XYZDef.h"
#include <fstream>
#include <string>
#include <cstdlib>
#include <kvs/Tokenizer>


namespace kvs
{

namespace grads
{

/*===========================================================================*/
/**
 *  @brief  Reads XDEF/YDEF/ZDEF entry.
 *  @param  line [in] read line
 *  @param  ifs [in] input file stream
 *  @return true, if the reading process is done successfully
 */
/*===========================================================================*/
bool XYZDef::read( std::string line, std::ifstream& ifs )
{
    kvs::Tokenizer t( line, " \t\n" );
    t.token(); // XDEF, YDEF or ZDEF

    // xnum
    if ( t.isLast() )
    {
        kvsMessageError("Cannot read num.");
        return false;
    }
    this->num = static_cast<size_t>( atoi( t.token().c_str() ) );

    // mapping
    if ( t.isLast() )
    {
        kvsMessageError("Cannot read mapping.");
        return false;
    }
    const std::string mapping = t.token();

    if ( mapping == "LINEAR" || mapping == "linear" )
    {
        this->mapping = Linear;
        this->values.allocate( 2 );
        this->values[0] = static_cast<kvs::Real32>( atof( t.token().c_str() ) );
        this->values[1] = static_cast<kvs::Real32>( atof( t.token().c_str() ) );
    }
    else if ( mapping == "LEVELS" || mapping == "levels" )
    {
        this->mapping = Levels;
        this->values.allocate( this->num );

        size_t counter = 0;
        if ( !t.isLast() )
        {
            while ( !t.isLast() )
            {
                const std::string value = t.token();
                if ( !value.empty() )
                {
                    this->values[counter++] = static_cast<kvs::Real32>( atof( value.c_str() ) );
                }
                if ( counter >= this->num ) break;
            }
        }

        if ( counter < this->num )
        {
            while ( std::getline( ifs, line ) )
            {
                t = kvs::Tokenizer( line, " \t" );
                while ( !t.isLast() )
                {
                    const std::string value = t.token();
                    if ( !value.empty() || value != " " )
                    {
                        this->values[counter++] = static_cast<kvs::Real32>( atof( value.c_str() ) );
                    }
                    if ( counter >= this->num ) break;
                }
                if ( counter >= this->num ) break;
            }
        }
    }

    return true;
}

} // end of namespace grads

} // end of namespace kvs
