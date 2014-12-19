/*****************************************************************************/
/**
 *  @file   DebugNew.h
 *  @author Naohisa Sakamoto
 */
/*----------------------------------------------------------------------------
 *
 *  Copyright (c) Visualization Laboratory, Kyoto University.
 *  All rights reserved.
 *  See http://www.viz.media.kyoto-u.ac.jp/kvs/copyright/ for details.
 *
 *  $Id: DebugNew.h 630 2010-10-10 02:05:28Z naohisa.sakamoto $
 */
/*****************************************************************************/
#ifndef KVS__DEBUG_NEW_H_INCLUDE
#define KVS__DEBUG_NEW_H_INCLUDE

#if defined ( KVS_ENABLE_MEM_DEBUG )

#include <kvs/MemoryTracer>

#define new new( __FILE__, __LINE__ )
#define delete kvs::MemoryTracer::DeleteAt( __FILE__, __LINE__ ), delete
#define malloc(s) kvs::MemoryTracer::Allocate( s, __FILE__, __LINE__, kvs::MemoryTracer::Malloc )
#define calloc(n,s) kvs::MemoryTracer::Allocate( s*n, __FILE__, __LINE__, kvs::MemoryTracer::Calloc )
#define realloc(p,s) kvs::MemoryTracer::Allocate( s, __FILE__, __LINE__, kvs::MemoryTracer::Realloc, p )
#define free(p) kvs::MemoryTracer::Deallocate( p, __FILE__, __LINE__, kvs::MemoryTracer::Free )

#endif

#endif // KVS__DEBUG_NEW_H_INCLUDE
