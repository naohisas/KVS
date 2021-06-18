/*****************************************************************************/
/**
 *  @file   OpacityTag.h
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
 *  @brief  Tag class for <Opacity>
 */
/*===========================================================================*/
class OpacityTag : public kvs::kvsml::TagBase
{
public:
    typedef kvs::kvsml::TagBase BaseClass;

public:
    OpacityTag();

};

} // end of namespace kvsml

} // end of namespace kvs
