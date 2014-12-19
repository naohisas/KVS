/*****************************************************************************/
/**
 *  @file   Vars.cpp
 *  @author Naohisa Sakamoto
 */
/*----------------------------------------------------------------------------
 *
 *  Copyright (c) Visualization Laboratory, Kyoto University.
 *  All rights reserved.
 *  See http://www.viz.media.kyoto-u.ac.jp/kvs/copyright/ for details.
 *
 *  $Id: Vars.cpp 1603 2013-06-25 01:29:12Z naohisa.sakamoto@gmail.com $
 */
/*****************************************************************************/
#include "Vars.h"
#include <kvs/Tokenizer>
#include <cstdlib>
#include <iostream>


namespace kvs
{

namespace grads
{

/*===========================================================================*/
/**
 *  @brief  Return index of the given variable.
 *  @param  varname [in] variable name
 *  @return index
 */
/*===========================================================================*/
int Vars::indexOf( const std::string& varname ) const
{
    std::list<Var>::const_iterator var = values.begin();
    std::list<Var>::const_iterator last = values.end();
    int counter = 0;
    while ( var != last )
    {
        if ( var->varname == varname ) { return counter; }
        counter++;
        var++;
    }

    return -1; // not found
}

/*===========================================================================*/
/**
 *  @brief  Reads VARS entry.
 *  @param  line [in] read line
 *  @param  ifs [in] input file stream
 *  @return true, if the reading process is done successfully
 */
/*===========================================================================*/
bool Vars::read( std::string line, std::ifstream& ifs )
{
    kvs::Tokenizer t( line, " \t\n" );
    t.token(); // VARS

    // varnum
    if ( t.isLast() ) return false;
    const size_t varnum = static_cast<size_t>( atoi( t.token().c_str() ) );

    this->values.clear();
    for ( size_t i = 0; i < varnum; i++ )
    {
        std::getline( ifs, line );
        t = kvs::Tokenizer( line, " \t" );

        Var var;

        // varname
        if ( t.isLast() ) return false;
        var.varname = t.token();

        // levs
        if ( t.isLast() ) return false;
        var.levs = atoi( t.token().c_str() );

        // units
        if ( t.isLast() ) return false;
        var.units = static_cast<float>( atof( t.token().c_str() ) );

        // description
        if ( t.isLast() ) return false;
        var.description = t.token();
        while ( !t.isLast() ) { var.description += " " + t.token(); }

        this->values.push_back( var );
    }

    return true;
}

} // end of namespace grads

} // end of namespace kvs
