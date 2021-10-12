/*****************************************************************************/
/**
 *  @file   Undef.h
 *  @author Naohisa Sakamoto
 */
/*****************************************************************************/
#pragma once
#include <string>
#include <fstream>
#include <kvs/Type>


namespace kvs
{

namespace grads
{

/*===========================================================================*/
/**
 *  @brief  UNDEF entry.
 */
/*===========================================================================*/
struct Undef
{
    kvs::Real32 value = 0.0f; ///< undefined value

    bool read( std::string line, std::ifstream& ifs );
};

} // end of namespace grads

} // end of namespace kvs
