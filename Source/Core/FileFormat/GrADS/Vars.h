/*****************************************************************************/
/**
 *  @file   Vars.h
 *  @author Naohisa Sakamoto
 */
/*----------------------------------------------------------------------------
 *
 *  Copyright (c) Visualization Laboratory, Kyoto University.
 *  All rights reserved.
 *  See http://www.viz.media.kyoto-u.ac.jp/kvs/copyright/ for details.
 *
 *  $Id: Vars.h 1603 2013-06-25 01:29:12Z naohisa.sakamoto@gmail.com $
 */
/*****************************************************************************/
#ifndef KVS__GRADS__VARS_H_INCLUDE
#define KVS__GRADS__VARS_H_INCLUDE

#include <list>
#include <string>
#include <fstream>


namespace kvs
{

namespace grads
{

/*===========================================================================*/
/**
 *  @brief  VARS entry.
 */
/*===========================================================================*/
struct Vars
{
    struct Var
    {
        std::string varname; ///< variable name
        int levs; ///< levels
        float units; ///< unit
        std::string description; ///< description
    };

    std::list<Var> values; ///< value list

    int indexOf( const std::string& varname ) const;
    bool read( std::string line, std::ifstream& ifs );
};

} // end of namespace grads

} // end of namespace kvs

#endif // KVS__GRADS__VARS_H_INCLUDE
