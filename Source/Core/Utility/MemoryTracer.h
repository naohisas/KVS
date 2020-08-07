/*****************************************************************************/
/**
 *  @file   MemoryTracer.h
 *  @author Naohisa Sakamoto
 */
/*****************************************************************************/
#pragma once
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
    int m_lock_counter; ///< lock counter
    Map m_map; ///< memory map

public:
    static int ArgumentCount; ///< argument count
    static char** ArgumentValues; ///< argument values
    static char* LogFileName; ///< name of log file
    static bool IsConstructed; ///< check flag whether this is constructed or not

public:
    MemoryTracer();
    MemoryTracer( char* filename );
    ~MemoryTracer();

    void insert( void* address, size_t size, char const* file, int line, AllocationType type );
    void remove( void* address, char const* file, int line, AllocationType type );
    void dump( std::ostream& os );

public:
    static void* Allocate( size_t size, char const* file, int line, AllocationType type, void* address = NULL );
    static void Deallocate( void* address, char const* file, int line, AllocationType type );
    static void DeleteAt( char const* file, int line );

private:
    void lock();
    void unlock();
    size_t leaked_memory_size() const;
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
    ~Lock();
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
    Node( );
    Node( size_t size, char const* name, int line, MemoryTracer::AllocationType type );

    size_t size() const;
    const char* name() const;
    int line() const;
    MemoryTracer::AllocationType type() const;
};

} // end of namespace kvs
