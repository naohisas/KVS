/*****************************************************************************/
/**
 *  @file   MemoryTracer.cpp
 *  @author Naohisa Sakamoto
 */
/*----------------------------------------------------------------------------
 *
 *  Copyright (c) Visualization Laboratory, Kyoto University.
 *  All rights reserved.
 *  See http://www.viz.media.kyoto-u.ac.jp/kvs/copyright/ for details.
 *
 *  $Id: MemoryTracer.cpp 1154 2012-06-06 01:35:35Z naohisa.sakamoto@gmail.com $
 */
/*****************************************************************************/
#include "MemoryTracer.h"
#include <iostream>
#include <fstream>
#include <exception>
#include <cstdio>
#include <cstring>
#include <string>


namespace
{
#if defined ( KVS_ENABLE_MEM_DEBUG )

char const* DeleteAtFile = "unknown";

int DeleteAtLine = 0;

#endif

const char* AllocationName[] =
{
    "unknown",
    "new",
    "new[]",
    "delete",
    "delete[]",
    "malloc",
    "calloc",
    "realloc",
    "free"
};

/*===========================================================================*/
/**
 *  @brief  Returns value string with commas.
 *  @param  n [in] value to insert commas
 *  @return value string with commas
 */
/*===========================================================================*/
const std::string Comma( const size_t n )
{
    char N[30]; memset( N, 0, 30 );

    sprintf( N, "%lu", n );
    if ( strlen(N) > 3 )
    {
        memmove( &N[strlen(N)-3], &N[strlen(N)-4], 4 );
        N[strlen(N) - 4] = ',';
    }

    if ( strlen(N) > 7 )
    {
        memmove( &N[strlen(N)-7], &N[strlen(N)-8], 8 );
        N[strlen(N) - 8] = ',';
    }

    if ( strlen(N) > 11 )
    {
        memmove( &N[strlen(N)-11], &N[strlen(N)-12], 12 );
        N[strlen(N) - 12] = ',';
    }

    return( std::string(N) );
}

} // end of namespace

// Static member parameters.
int kvs::MemoryTracer::ArgumentCount = 0;
char** kvs::MemoryTracer::ArgumentValues = NULL;
char* kvs::MemoryTracer::LogFileName = NULL;
bool kvs::MemoryTracer::IsConstructed = false;


#if defined ( KVS_ENABLE_MEM_DEBUG )

// Memory tracer
extern kvs::MemoryTracer kvsMemoryTracer;

/*===========================================================================*/
/**
 *  @brief  Tracable new operator.
 *  @param  size [in] allocation size [byte]
 *  @param  file [in] filename of source code
 *  @param  line [in] line number of source code
 *  @return allocated memory address
 */
/*===========================================================================*/
void* operator new ( size_t size, char const* file, int line )
{
    kvs::MemoryTracer::AllocationType type = kvs::MemoryTracer::New;
    return( kvs::MemoryTracer::Allocate( size, file, line, type ) );
}

/*===========================================================================*/
/**
 *  @brief  Tracable delete operator.
 *  @param  address [in] address of the deallocating memory region
 */
/*===========================================================================*/
void operator delete ( void* address )
{
    kvs::MemoryTracer::AllocationType type = kvs::MemoryTracer::Delete;
    kvs::MemoryTracer::Deallocate( address, ::DeleteAtFile, ::DeleteAtLine, type );
}

/*===========================================================================*/
/**
 *  @brief  Tracable new[] operator.
 *  @param  size [in] allocation size [byte]
 *  @param  file [in] filename of source code
 *  @param  line [in] line number of source code
 *  @return allocated memory address
 */
/*===========================================================================*/
void* operator new [] ( size_t size, char const* file, int line )
{
    kvs::MemoryTracer::AllocationType type = kvs::MemoryTracer::NewArray;
    return( kvs::MemoryTracer::Allocate( size, file, line, type ) );
}

/*===========================================================================*/
/**
 *  @brief  Tracable delete[] operator.
 *  @param  address [in] address of the deallocating memory region
 */
/*===========================================================================*/
void operator delete [] ( void* address )
{
    kvs::MemoryTracer::AllocationType type = kvs::MemoryTracer::DeleteArray;
    kvs::MemoryTracer::Deallocate( address, ::DeleteAtFile, ::DeleteAtLine, type );
}

#endif


namespace kvs
{

/*===========================================================================*/
/**
 *  @brief  Constructs a new MemoryTracer class.
 */
/*===========================================================================*/
MemoryTracer::MemoryTracer( void ):
    m_nallocations( 0 ),
    m_total_nallocations( 0 ),
    m_total_ndeallocations( 0 ),
    m_allocated_memory( 0 ),
    m_peak_allocated_memory( 0 ),
    m_lock_counter( 0 )
{
    kvs::MemoryTracer::IsConstructed = true;
}

/*===========================================================================*/
/**
 *  @brief  Constructs a new MemoryTracer class.
 *  @param  filename [in] name of log file
 */
/*===========================================================================*/
MemoryTracer::MemoryTracer( char* filename ):
    m_nallocations( 0 ),
    m_total_nallocations( 0 ),
    m_total_ndeallocations( 0 ),
    m_allocated_memory( 0 ),
    m_peak_allocated_memory( 0 ),
    m_lock_counter( 0 )
{
    kvs::MemoryTracer::IsConstructed = true;
    kvs::MemoryTracer::LogFileName = filename;
}

/*===========================================================================*/
/**
 *  @brief  Destroys the MemoryTracer class.
 */
/*===========================================================================*/
MemoryTracer::~MemoryTracer( void )
{
    kvs::MemoryTracer::IsConstructed = false;
    if ( kvs::MemoryTracer::LogFileName )
    {
        std::ofstream ofs( kvs::MemoryTracer::LogFileName );
        this->dump( ofs );
    }
    else
    {
        this->dump( std::cout );
    }
}

/*===========================================================================*/
/**
 *  @brief  Inserts the information of allocated memory into the memory map.
 *  @param  address [in] memory address
 *  @param  size [in] allocating memory size [byte]
 *  @param  file [in] filename of source code
 *  @param  line [in] line number of source code
 *  @param  type [in] allocation (deallocation) type
 */
/*===========================================================================*/
void MemoryTracer::insert( void* address, size_t size, char const* file, int line, MemoryTracer::AllocationType type )
{
    if ( m_lock_counter > 0 ) return;

    MemoryTracer::Lock lock( this );
    m_map[ address ] = Node( size, file, line, type );
    m_nallocations++;
    m_total_nallocations++;
    m_allocated_memory += size;
//    m_peak_allocated_memory = kvs::Math::Max( m_peak_allocated_memory, m_allocated_memory );
    m_peak_allocated_memory = m_peak_allocated_memory > m_allocated_memory ? m_peak_allocated_memory : m_allocated_memory;
}

/*===========================================================================*/
/**
 *  @brief  Removes the information of allocated memory from the memory map.
 *  @param  address [in] memory address
 *  @param  file [in] filename of source code
 *  @param  line [in] line number of source code
 *  @param  type [in] allocation (deallocation) type
 */
/*===========================================================================*/
void MemoryTracer::remove( void* address, char const* file, int line, MemoryTracer::AllocationType type )
{
    if ( m_lock_counter > 0 ) return;

    MemoryTracer::Lock lock( this );
    Map::iterator node = m_map.find( address );
    if ( node != m_map.end() )
    {
        if ( ( ( type == MemoryTracer::Delete      ) && ( node->second.type() == MemoryTracer::New      ) ) ||
             ( ( type == MemoryTracer::DeleteArray ) && ( node->second.type() == MemoryTracer::NewArray ) ) ||
             ( ( type == MemoryTracer::Free        ) && ( node->second.type() == MemoryTracer::Malloc   ) ) ||
             ( ( type == MemoryTracer::Free        ) && ( node->second.type() == MemoryTracer::Calloc   ) ) ||
             ( ( type == MemoryTracer::Free        ) && ( node->second.type() == MemoryTracer::Realloc  ) ) ||
             ( ( type == MemoryTracer::Realloc     ) && ( node->second.type() == MemoryTracer::Malloc   ) ) ||
             ( ( type == MemoryTracer::Realloc     ) && ( node->second.type() == MemoryTracer::Calloc   ) ) ||
             ( ( type == MemoryTracer::Realloc     ) && ( node->second.type() == MemoryTracer::Realloc  ) ) )
        {
            m_nallocations--;
            m_total_ndeallocations++;
            m_allocated_memory -= node->second.size();
            m_map.erase( node );
        }
    }
}

/*===========================================================================*/
/**
 *  @brief  Dumps the memory map for detecting memory leaks.
 *  @param  os [in] output stream
 */
/*===========================================================================*/
void MemoryTracer::dump( std::ostream& os )
{
    std::string command("");
    std::string args("");
    if ( kvs::MemoryTracer::ArgumentCount > 0 )
    {
        command = std::string( kvs::MemoryTracer::ArgumentValues[0] );
        for ( int i = 1; i < kvs::MemoryTracer::ArgumentCount; i++ )
        {
            args += std::string( kvs::MemoryTracer::ArgumentValues[i] ) + " ";
        }
    }

    os << "K V S  M E M O R Y  D E B U G  R E P O R T" << std::endl;
//    os << "Time: " << kvs::Time() << std::endl;
    os << "Time: " << __TIME__ << std::endl;
//    os << "Date: " << kvs::Date() << std::endl;
    os << "Date: " << __DATE__ << std::endl;
    os << "Command: " << command << std::endl;
    os << "Argument: " << args << std::endl;
    os << std::endl;

    os << "M E M O R Y  U S A G E" << std::endl;
    os << "Current number of allocations: " << m_nallocations << std::endl;
    os << "Total number of memory allocations: " << m_total_nallocations << std::endl;
    os << "Total number of memory deallocations: " << m_total_ndeallocations << std::endl;
    os << "Peak amount of allocated memory: " << ::Comma(m_peak_allocated_memory) << " bytes" << std::endl;
    os << std::endl;

    const size_t nleaks = m_map.size();
    const size_t leak_size = this->leaked_memory_size();
    os << "M E M O R Y  L E A K S" << std::endl;
    os << "Number of memory leaks: " << nleaks << std::endl;
    os << "Total amount of leaked memory: " << ::Comma(leak_size) << " bytes" << std::endl;
    os << std::endl;

    if ( m_map.size() != 0 )
    {
        os << "L E A K E D  M E M O R Y" << std::endl;
        Map::const_iterator mem = m_map.begin();
        Map::const_iterator end = m_map.end();
        while ( mem != end )
        {
            const void* address = mem->first;
            const char* name = mem->second.name();
            const int line = mem->second.line();
            const size_t size = mem->second.size();
            const std::string type = ::AllocationName[ mem->second.type() ];
            os << std::hex << address << ": " << std::dec
               << name << "("
               << line << "), "
               << ::Comma(size) << " bytes, "
               << type << ""
               << std::endl;
            ++mem;
        }
    }
}

/*===========================================================================*/
/**
 *  @brief  Lock for insert/remove process.
 */
/*===========================================================================*/
void MemoryTracer::lock( void )
{
    m_lock_counter++;
}

/*===========================================================================*/
/**
 *  @brief  Unlock for insert/remove process.
 */
/*===========================================================================*/
void MemoryTracer::unlock( void )
{
    m_lock_counter--;
}

/*===========================================================================*/
/**
 *  @brief  Returns the leaked memory size.
 *  @return leaked memory size
 */
/*===========================================================================*/
size_t MemoryTracer::leaked_memory_size( void ) const
{
    int bytes = 0;
    Map::const_iterator mem = m_map.begin();
    Map::const_iterator end = m_map.end();
    while ( mem != end )
    {
        bytes += mem->second.size();
        ++mem;
    }

    return( bytes );
}

#if defined ( KVS_ENABLE_MEM_DEBUG )

/*===========================================================================*/
/**
 *  @brief  Allocated memry.
 *  @param  size [in] allocating memory size [byte]
 *  @param  file [in] filename of source code
 *  @param  line [in] line number of source code
 *  @param  type [in] allocation (deallocation) type
 *  @param  address [in] memory address for 'realloc'
 *  @return address of allocated memory
 */
/*===========================================================================*/
void* MemoryTracer::Allocate( size_t size, char const* file, int line, MemoryTracer::AllocationType type, void* address )
{
    if ( type == MemoryTracer::Realloc )
    {
        if ( kvs::MemoryTracer::IsConstructed )
        {
            kvsMemoryTracer.remove( address, file, line, type );
        }

        address = realloc( address, size );
        if ( !address ) throw std::bad_alloc();

        if ( kvs::MemoryTracer::IsConstructed )
        {
            kvsMemoryTracer.insert( address, size, file, line, type );
        }
    }
    else
    {
        address = malloc( size );
        if ( !address ) throw std::bad_alloc();

        if ( kvs::MemoryTracer::IsConstructed )
        {
            kvsMemoryTracer.insert( address, size, file, line, type );
        }
    }

    return( address );
}

/*===========================================================================*/
/**
 *  @brief  Deallocated memry.
 *  @param  file [in] filename of source code
 *  @param  line [in] line number of source code
 *  @param  type [in] allocation (deallocation) type
 */
/*===========================================================================*/
void MemoryTracer::Deallocate( void* address, char const* file, int line, MemoryTracer::AllocationType type )
{
    if ( !address ) return;

    if ( kvs::MemoryTracer::IsConstructed )
    {
        kvsMemoryTracer.remove( address, file, line, type );
    }

    free( address );
}

/*===========================================================================*/
/**
 *  @brief  Set filename and line to the global parameters.
 *  @param  file [in] name of source code
 *  @param  line [in] line
 */
/*===========================================================================*/
void MemoryTracer::DeleteAt( char const* file, int line )
{
    ::DeleteAtFile = file;
    ::DeleteAtLine = line;
}

#endif

/*===========================================================================*/
/**
 *  @brief  Constructs a new Lock class.
 *  @param  tracer [in] pointer to the memory tracer
 */
/*===========================================================================*/
MemoryTracer::Lock::Lock( MemoryTracer* tracer ):
    m_tracer( tracer )
{
    m_tracer->lock();
}

/*===========================================================================*/
/**
 *  @brief  Destructs the Lock class.
 */
/*===========================================================================*/
MemoryTracer::Lock::~Lock( void )
{
    m_tracer->unlock();
}

/*===========================================================================*/
/**
 *  @brief  Constructs a new Node class.
 */
/*===========================================================================*/
MemoryTracer::Node::Node( void ):
    m_size( 0 ),
    m_name( 0 ),
    m_line( 0 ),
    m_type( MemoryTracer::Unknown )
{
}

/*===========================================================================*/
/**
 *  @brief  Constructs a new Node class.
 *  @param  size [in] memory size in byte
 *  @param  name [in] filename of source code
 *  @param  line [in] line number
 *  @param  type [in] allocation type
 */
/*===========================================================================*/
MemoryTracer::Node::Node( size_t size, char const* name, int line, MemoryTracer::AllocationType type ):
    m_size( size ),
    m_name( name ),
    m_line( line ),
    m_type( type )
{
}

/*===========================================================================*/
/**
 *  @brief  Returns memory size in byte.
 *  @return memory size
 */
/*===========================================================================*/
size_t MemoryTracer::Node::size( void ) const
{
    return( m_size );
}

/*===========================================================================*/
/**
 *  @brief  Returns filename of source code.
 *  @return filename
 */
/*===========================================================================*/
const char* MemoryTracer::Node::name( void ) const
{
    return( m_name );
}

/*===========================================================================*/
/**
 *  @brief  Returns line number within the file
 *  @return line number
 */
/*===========================================================================*/
int MemoryTracer::Node::line( void ) const
{
    return( m_line );
}

/*===========================================================================*/
/**
 *  @brief  Returns allocation type.
 *  @return allocation type
 */
/*===========================================================================*/
MemoryTracer::AllocationType MemoryTracer::Node::type( void ) const
{
    return( m_type );
}

} // end of namespace kvs
