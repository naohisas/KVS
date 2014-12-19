/*****************************************************************************/
/**
 *  @file   NormalTag.cpp
 *  @author Naohisa Sakamoto
 */
/*----------------------------------------------------------------------------
 *
 *  Copyright (c) Visualization Laboratory, Kyoto University.
 *  All rights reserved.
 *  See http://www.viz.media.kyoto-u.ac.jp/kvs/copyright/ for details.
 *
 *  $Id: NormalTag.cpp 1812 2014-09-11 07:34:35Z naohisa.sakamoto@gmail.com $
 */
/*****************************************************************************/
#include "NormalTag.h"


namespace kvs
{

namespace kvsml
{

/*===========================================================================*/
/**
 *  @brief  Constructs a new normal tag class.
 */
/*===========================================================================*/
NormalTag::NormalTag():
    kvs::kvsml::TagBase( "Normal" )
{
}

} // end of namespace kvsml

} // end of namespace kvs
