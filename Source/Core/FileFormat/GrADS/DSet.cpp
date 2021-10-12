/*****************************************************************************/
/**
 *  @file   DSet.cpp
 *  @author Naohisa Sakamoto
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
