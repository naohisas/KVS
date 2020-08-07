/****************************************************************************/
/**
 *  @file   Assert.h
 *  @author Naohisa Sakamoto
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
