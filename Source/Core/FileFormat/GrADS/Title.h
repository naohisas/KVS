/*****************************************************************************/
/**
 *  @file   Title.h
 *  @author Naohisa Sakamoto
 */
/*----------------------------------------------------------------------------
 *
 *  Copyright (c) Visualization Laboratory, Kyoto University.
 *  All rights reserved.
 *  See http://www.viz.media.kyoto-u.ac.jp/kvs/copyright/ for details.
 *
 *  $Id: Title.h 1303 2012-09-14 11:26:37Z naohisa.sakamoto@gmail.com $
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
