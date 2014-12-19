/*****************************************************************************/
/**
 *  @file   DSet.h
 *  @author Naohisa Sakamoto
 */
/*----------------------------------------------------------------------------
 *
 *  Copyright (c) Visualization Laboratory, Kyoto University.
 *  All rights reserved.
 *  See http://www.viz.media.kyoto-u.ac.jp/kvs/copyright/ for details.
 *
 *  $Id: DSet.h 1303 2012-09-14 11:26:37Z naohisa.sakamoto@gmail.com $
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
