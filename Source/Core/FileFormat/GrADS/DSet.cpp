/*****************************************************************************/
/**
 *  @file   DSet.cpp
 *  @author Naohisa Sakamoto
 */
/*----------------------------------------------------------------------------
 *
 *  Copyright (c) Visualization Laboratory, Kyoto University.
 *  All rights reserved.
 *  See http://www.viz.media.kyoto-u.ac.jp/kvs/copyright/ for details.
 *
 *  $Id: DSet.cpp 1346 2012-11-08 11:57:55Z s.yamada0808@gmail.com $
 */
/*****************************************************************************/
#include "DSet.h"
#include <kvs/Tokenizer>


namespace kvs
{

namespace grads
{

/*===========================================================================*/
/**
 *  @brief  Reads DSET entry.
 *  @param  line [in] read line
 *  @param  ifs [in] input file stream
 *  @return true, if the reading process is done successfully
 */
/*===========================================================================*/
bool DSet::read( std::string line, std::ifstream& )
{
    kvs::Tokenizer t( line, " \t\n" );
    t.token(); // DSET

    this->name = t.token();

    return true;
}

} // end of namespace grads

} // end of namespace kvs
