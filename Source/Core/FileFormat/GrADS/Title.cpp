/*****************************************************************************/
/**
 *  @file   Title.cpp
 *  @author Naohisa Sakamoto
 */
/*----------------------------------------------------------------------------
 *
 *  Copyright (c) Visualization Laboratory, Kyoto University.
 *  All rights reserved.
 *  See http://www.viz.media.kyoto-u.ac.jp/kvs/copyright/ for details.
 *
 *  $Id: Title.cpp 1346 2012-11-08 11:57:55Z s.yamada0808@gmail.com $
 */
/*****************************************************************************/
#include "Title.h"
#include <kvs/Tokenizer>


namespace kvs
{

namespace grads
{

/*===========================================================================*/
/**
 *  @brief  Reads TITLE entry.
 *  @param  line [in] read line
 *  @param  ifs [in] input file stream
 *  @return true, if the reading process is done successfully
 */
/*===========================================================================*/
bool Title::read( std::string line, std::ifstream& )
{
    kvs::Tokenizer t( line, " \t\n" );
    t.token(); // TITLE

    this->name = t.token();

    return true;
}

} // end of namespace grads

} // end of namespace kvs
