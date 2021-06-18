/*****************************************************************************/
/**
 *  @file   ColorTag.h
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
 *  @brief  Tag class for <Color>
 */
/*===========================================================================*/
class ColorTag : public kvs::kvsml::TagBase
{
public:
    using BaseClass = kvs::kvsml::TagBase;

public:
    ColorTag();
};

} // end of namespace kvsml

} // end of namespace kvs
