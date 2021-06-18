/*****************************************************************************/
/**
 *  @file   PointObjectTag.h
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
 *  @brief  Tag class for <PointObject>
 */
/*===========================================================================*/
class PointObjectTag : public kvs::kvsml::TagBase
{
public:
    typedef kvs::kvsml::TagBase BaseClass;

public:
    PointObjectTag();

};

} // end of namespace kvsml

} // end of namespace kvs
