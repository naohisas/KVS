/*****************************************************************************/
/**
 *  @file   DebugNew.h
 *  @author Naohisa Sakamoto
 */
/*****************************************************************************/
#pragma once
#if defined ( KVS_ENABLE_MEM_DEBUG )

#include <kvs/MemoryTracer>

#define new new( __FILE__, __LINE__ )
#define delete kvs::MemoryTracer::DeleteAt( __FILE__, __LINE__ ), delete
#define malloc(s) kvs::MemoryTracer::Allocate( s, __FILE__, __LINE__, kvs::MemoryTracer::Malloc )
#define calloc(n,s) kvs::MemoryTracer::Allocate( s*n, __FILE__, __LINE__, kvs::MemoryTracer::Calloc )
#define realloc(p,s) kvs::MemoryTracer::Allocate( s, __FILE__, __LINE__, kvs::MemoryTracer::Realloc, p )
#define free(p) kvs::MemoryTracer::Deallocate( p, __FILE__, __LINE__, kvs::MemoryTracer::Free )

#endif
