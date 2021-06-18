/*****************************************************************************/
/**
 *  @file   MemoryDebugger.h
 *  @author Naohisa Sakamoto
 */
/*****************************************************************************/
#pragma once
#if defined ( KVS_ENABLE_MEM_DEBUG )

#include <kvs/MemoryTracer>
#include <kvs/DebugNew>

// Constructors
#define KVS_MEMORY_DEBUGGER kvs::MemoryTracer kvsMemoryTracer

#define KVS_MEMORY_DEBUGGER_LOG(filename) kvs::MemoryTracer kvsMemoryTracer(filename)

// Methods
#define KVS_MEMORY_DEBUGGER__SET_LOGFILENAME(filename)  \
    kvs::MemoryTracer::LogFileName = filename

#define KVS_MEMORY_DEBUGGER__SET_ARGUMENT(argc,argv) \
    kvs::MemoryTracer::ArgumentCount = argc;         \
    kvs::MemoryTracer::ArgumentValues = argv

#else

#define KVS_MEMORY_DEBUGGER
#define KVS_MEMORY_DEBUGGER_LOG(filename)
#define KVS_MEMORY_DEBUGGER__SET_LOGFILENAME(filename)
#define KVS_MEMORY_DEBUGGER__SET_ARGUMENT(argc,argv)

#endif
