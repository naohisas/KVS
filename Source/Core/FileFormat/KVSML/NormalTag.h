/*****************************************************************************/
/**
 *  @file   NormalTag.h
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
 *  @brief  Tag class for <Normal>
 */
/*===========================================================================*/
class NormalTag : public kvs::kvsml::TagBase
{
public:
    typedef kvs::kvsml::TagBase BaseClass;

public:
    NormalTag();

};

} // end of namespace kvsml

} // end of namespace kvs
