/*****************************************************************************/
/**
 *  @file   Vars.h
 *  @author Naohisa Sakamoto
 */
/*****************************************************************************/
#pragma once
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
        std::string varname = ""; ///< variable name
        int levs = 0; ///< levels
        float units = 0.0f; ///< unit
        std::string description = ""; ///< description
    };

    std::list<Var> values{}; ///< value list

    int indexOf( const std::string& varname ) const;
    bool read( std::string line, std::ifstream& ifs );
};

} // end of namespace grads

} // end of namespace kvs
