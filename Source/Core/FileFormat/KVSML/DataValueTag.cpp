/*****************************************************************************/
/**
 *  @file   DataValueTag.cpp
 *  @author Naohisa Sakamoto
 */
/*****************************************************************************/
#include "DataValueTag.h"
#include <kvs/IgnoreUnusedVariable>


namespace kvs
{

namespace kvsml
{

/*===========================================================================*/
/**
 *  @brief  Constructs a new data value class.
 */
/*===========================================================================*/
DataValueTag::DataValueTag():
    kvs::kvsml::TagBase( "DataValue" )
{
}

} // end of namespace kvsml

} // end of namespace kvs
