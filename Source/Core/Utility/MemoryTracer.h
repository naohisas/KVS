/*****************************************************************************/
/**
 *  @file   MemoryTracer.h
 *  @author Naohisa Sakamoto
 */
/*----------------------------------------------------------------------------
 *
 *  Copyright (c) Visualization Laboratory, Kyoto University.
 *  All rights reserved.
 *  See http://www.viz.media.kyoto-u.ac.jp/kvs/copyright/ for details.
 *
 *  $Id: MemoryTracer.h 1707 2014-01-27 07:37:04Z naohisa.sakamoto@gmail.com $
 */
/*****************************************************************************/
#ifndef KVS__MEMORY_TRACER_H_INCLUDE
#define KVS__MEMORY_TRACER_H_INCLUDE

#include <map>
#include <ostream>
#include <kvs/Platform>


#if defined ( KVS_ENABLE_MEM_DEBUG )

void* operator new ( size_t size, char const* file, int line );

void operator delete ( void* address );

void* operator new [] ( size_t size, char const* file, int line );

void operator delete [] ( void* address );

#endif


namespace kvs
{

/*===========================================================================*/
/**
 *  @brief  MemoryTracer class.
 */
/*===========================================================================*/
class MemoryTracer
{
public:

    class Lock;
    class Node;

    typedef void* Address;
    typedef std::map<Address,Node> Map;

public:

    enum AllocationType
    {
        Unknown = 0,
        New,
        NewArray,
        Delete,
        DeleteArray,
        Malloc,
        Calloc,
        Realloc,
        Free
    };

private:

    size_t m_nallocations; ///< currently number of allocations
    size_t m_total_nallocations; ///< total number of allocations
    size_t m_total_ndeallocations; ///< total number of deallocations
    size_t m_allocated_memory; ///< allocated memory size in byte
    size_t m_peak_allocated_memory; ///< peak allocated memory size in byte
    int    m_lock_counter; ///< lock counter
    Map    m_map; ///< memory map

public:

    static int ArgumentCount; ///< argument count
    static char** ArgumentValues; ///< argument values
    static char* LogFileName; ///< name of log file
    static bool IsConstructed; ///< check flag whether this is constructed or not

public:

    MemoryTracer( void );

    MemoryTracer( char* filename );

    ~MemoryTracer( void );

public:

    void insert( void* address, size_t size, char const* file, int line, AllocationType type );

    void remove( void* address, char const* file, int line, AllocationType type );

    void dump( std::ostream& os );

public:

    static void* Allocate( size_t size, char const* file, int line, AllocationType type, void* address = NULL );

    static void Deallocate( void* address, char const* file, int line, AllocationType type );

    static void DeleteAt( char const* file, int line );

private:

    void lock( void );

    void unlock( void );

    size_t leaked_memory_size( void ) const;
};

/*===========================================================================*/
/**
 *  @brief  Lock class for MemoryTracer.
 */
/*===========================================================================*/
class MemoryTracer::Lock
{
private:

    MemoryTracer* m_tracer; ///< pointer to the memory tracer

public:

    Lock( MemoryTracer* tracer );

    ~Lock( void );
};

/*===========================================================================*/
/**
 *  @brief  Node class for MemoryTracer.
 */
/*===========================================================================*/
class MemoryTracer::Node
{
private:

    size_t m_size; ///< byte size
    char const* m_name; ///< filename
    int m_line; ///< line number
    MemoryTracer::AllocationType m_type; ///< allocation type

public:

    Node( void );

    Node( size_t size, char const* name, int line, MemoryTracer::AllocationType type );

public:

    size_t size( void ) const;

    const char* name( void ) const;

    int line( void ) const;

    MemoryTracer::AllocationType type( void ) const;
};

} // end of namespace kvs

#endif // KVS__MEMORY_TRACER_H_INCLUDE
