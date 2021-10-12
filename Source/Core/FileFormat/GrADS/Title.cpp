/*****************************************************************************/
/**
 *  @file   Title.cpp
 *  @author Naohisa Sakamoto
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
