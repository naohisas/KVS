/*****************************************************************************/
/**
 *  @file   CoordTag.h
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
 *  @brief  Tag class for <Coord>
 */
/*===========================================================================*/
class CoordTag : public kvs::kvsml::TagBase
{
public:
    using BaseClass = kvs::kvsml::TagBase;

public:
    CoordTag();
};

} // end of namespace kvsml

} // end of namespace kvs
