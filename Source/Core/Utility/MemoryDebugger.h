/*****************************************************************************/
/**
 *  @file   MemoryDebugger.h
 *  @author Naohisa Sakamoto
 */
/*----------------------------------------------------------------------------
 *
 *  Copyright (c) Visualization Laboratory, Kyoto University.
 *  All rights reserved.
 *  See http://www.viz.media.kyoto-u.ac.jp/kvs/copyright/ for details.
 *
 *  $Id: MemoryDebugger.h 630 2010-10-10 02:05:28Z naohisa.sakamoto $
 */
/*****************************************************************************/
#ifndef KVS__MEMORY_DEBUGGER_H_INCLUDE
#define KVS__MEMORY_DEBUGGER_H_INCLUDE

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

#endif // KVS__MEMORY_DEBUGGER_H_INCLUDE
