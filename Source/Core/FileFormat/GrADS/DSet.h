/*****************************************************************************/
/**
 *  @file   DSet.h
 *  @author Naohisa Sakamoto
 */
/*****************************************************************************/
#ifndef KVS__GRADS__DSET_H_INCLUDE
#define KVS__GRADS__DSET_H_INCLUDE

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
    std::string name; ///< data filename

    bool read( std::string line, std::ifstream& ifs );
};

} // end of namespace grads

} // end of namespace kvs

#endif // KVS__GRADS__DSET_H_INCLUDE
