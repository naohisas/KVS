/*****************************************************************************/
/**
 *  @file   SizeTag.h
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
 *  @brief  Tag class for <Size>
 */
/*===========================================================================*/
class SizeTag : public kvs::kvsml::TagBase
{
public:
    typedef kvs::kvsml::TagBase BaseClass;

public:
    SizeTag();

};

} // end of namespace kvsml

} // end of namespace kvs
