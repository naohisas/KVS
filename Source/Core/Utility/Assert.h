/****************************************************************************/
/**
 *  @file Assert.h
 */
/*----------------------------------------------------------------------------
 *
 *  Copyright (c) Visualization Laboratory, Kyoto University.
 *  All rights reserved.
 *  See http://www.viz.media.kyoto-u.ac.jp/kvs/copyright/ for details.
 *
 *  $Id: Assert.h 631 2010-10-10 02:15:35Z naohisa.sakamoto $
 */
/****************************************************************************/
#pragma once
#include <kvs/Message>

#if defined ( KVS_ENABLE_DEBUG )
#define KVS_ASSERT( exp )                                               \
    kvs::Message( KVS_MACRO_FILE, KVS_MACRO_LINE, KVS_MACRO_FUNC ).assertion( exp, "The conditional expression of '" #exp "' must be true." )
#else
#define KVS_ASSERT( exp )
#endif
