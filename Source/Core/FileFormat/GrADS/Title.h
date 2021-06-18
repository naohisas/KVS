/*****************************************************************************/
/**
 *  @file   Title.h
 *  @author Naohisa Sakamoto
 */
/*****************************************************************************/
#ifndef KVS__GRADS__TITLE_H_INCLUDE
#define KVS__GRADS__TITLE_H_INCLUDE

#include <string>
#include <fstream>


namespace kvs
{

namespace grads
{

/*===========================================================================*/
/**
 *  @brief  TITLE entry.
 */
/*===========================================================================*/
struct Title
{
    std::string name; ///< name

    bool read( std::string line, std::ifstream& ifs );
};

} // end of namespace grads

} // end of namespace kvs

#endif // KVS__GRADS__TITLE_H_INCLUDE
