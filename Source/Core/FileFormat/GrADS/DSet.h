/*****************************************************************************/
/**
 *  @file   DSet.h
 *  @author Naohisa Sakamoto
 */
/*****************************************************************************/
#pragma once
#include <string>
#include <fstream>


namespace kvs
{

namespace grads
{

/*===========================================================================*/
/**
 *  @brief  DSET entry.
 */
/*===========================================================================*/
struct DSet
{
    std::string name = ""; ///< data filename
    bool read( std::string line, std::ifstream& ifs );
};

} // end of namespace grads

} // end of namespace kvs
