/*****************************************************************************/
/**
 *  @file   Title.h
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
 *  @brief  TITLE entry.
 */
/*===========================================================================*/
struct Title
{
    std::string name = ""; ///< name

    bool read( std::string line, std::ifstream& ifs );
};

} // end of namespace grads

} // end of namespace kvs
