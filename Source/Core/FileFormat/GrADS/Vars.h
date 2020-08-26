/*****************************************************************************/
/**
 *  @file   Vars.h
 *  @author Naohisa Sakamoto
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
