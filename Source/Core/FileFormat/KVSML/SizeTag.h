/*****************************************************************************/
/**
 *  @file   SizeTag.h
 */
/*----------------------------------------------------------------------------
 *
 *  Copyright (c) Visualization Laboratory, Kyoto University.
 *  All rights reserved.
 *  See http://www.viz.media.kyoto-u.ac.jp/kvs/copyright/ for details.
 *
 *  $Id: SizeTag.h 1342 2012-11-07 13:47:24Z s.yamada0808@gmail.com $
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
