/*****************************************************************************/
/**
 *  @file   ConnectionTag.h
 *  @author Naohisa Sakamoto
 */
/*****************************************************************************/
#pragma once
#include "TagBase.h"


namespace kvs
{

namespace kvsml
{

/*===========================================================================*/
/**
 *  @brief  Tag class for <Connection>
 */
/*===========================================================================*/
class ConnectionTag : public kvs::kvsml::TagBase
{
public:
    using BaseClass = kvs::kvsml::TagBase;

public:
    ConnectionTag();
};

} // end of namespace kvsml

} // end of namespace kvs
