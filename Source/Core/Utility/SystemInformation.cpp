/*****************************************************************************/
/**
 *  @file   SystemInformation.cpp
 *  @author Naohisa Sakamoto
 *  @brief  
 */
/*----------------------------------------------------------------------------
 *
 *  Copyright (c) Visualization Laboratory, Kyoto University.
 *  All rights reserved.
 *  See http://www.viz.media.kyoto-u.ac.jp/kvs/copyright/ for details.
 *
 *  $Id$
 */
/*****************************************************************************/
#include "SystemInformation.h"
#include <cstdio>
#include <cerrno>
#include <cstring>
#include <kvs/Platform>
#if   defined ( KVS_PLATFORM_WINDOWS )
#include <windows.h>
#elif defined ( KVS_PLATFORM_LINUX ) || defined ( KVS_PLATFORM_CYGWIN )
#include <unistd.h>
#elif defined ( KVS_PLATFORM_MACOSX )
#include <mach/mach.h>
#include <mach/machine.h>
#include <mach/mach_host.h>
#include <mach/host_info.h>
#include <sys/sysctl.h>
#include <sys/utsname.h>
#endif
#include <kvs/Message>


namespace
{

/*===========================================================================*/
/**
 *  @brief  Returns warning message.
 *  @param  number [in] error number (errno)
 *  @param  message [in] pointer to message
 *  @return warning message
 */
/*===========================================================================*/
const char* GetWarningMessage( int number, const char* message )
{
    return number == EINVAL ? strerror( number ) : message;
}

}


namespace kvs
{

/*===========================================================================*/
/**
 *  @brief  Returns number of processors.
 *  @return number of processors
 */
/*===========================================================================*/
size_t SystemInformation::NumberOfProcessors()
{
// Windows
#if defined ( KVS_PLATFORM_WINDOWS )
    SYSTEM_INFO sysinfo;
    GetSystemInfo( &sysinfo );
    return sysinfo.dwNumberOfProcessors;

// Linux
#elif defined ( KVS_PLATFORM_LINUX ) || defined ( KVS_PLATFORM_CYGWIN )
    int nprocessors = sysconf( _SC_NPROCESSORS_ONLN );
    if ( nprocessors == -1 )
    {
        const char* message = "_SC_NPROCESSORS_ONLN is not supported.";
        kvsMessageWarning( ::GetWarningMessage( errno, message ) );
    }
    return nprocessors;

// Mac OS X
#elif defined ( KVS_PLATFORM_MACOSX )
    int nprocessors = 0;
    int mib[2] = { CTL_HW, HW_NCPU };
    size_t length = sizeof( nprocessors );
    int ret = sysctl( mib, 2, &nprocessors, &length, NULL, 0 );
    if ( ret == -1 )
    {
        const char* message = strerror( errno );
        kvsMessageWarning( ::GetWarningMessage( errno, message ) );
    }
    return nprocessors;
#endif
}

/*===========================================================================*/
/**
 *  @brief  Returns total memory size in bytes.
 *  @return total memory size
 */
/*===========================================================================*/
size_t SystemInformation::TotalMemorySize()
{
// Windows
#if defined ( KVS_PLATFORM_WINDOWS )
#if defined ( KVS_PLATFORM_CPU_64 )
    MEMORYSTATUSEX memstat;
    GlobalMemoryStatusEx( &memstat );
    return memstat.ullTotalPhys;
#else
    MEMORYSTATUS memstat;
    GlobalMemoryStatus( &memstat );
    return memstat.dwTotalPhys;
#endif

// Linux
#elif defined ( KVS_PLATFORM_LINUX ) || defined ( KVS_PLATFORM_CYGWIN )
    long phys_page_size = sysconf( _SC_PHYS_PAGES );
    if ( phys_page_size == -1 )
    {
        const char* message = "_SC_PHYS_PAGES is not supported.";
        kvsMessageWarning( ::GetWarningMessage( errno, message ) );
    }

    long page_size = sysconf( _SC_PAGESIZE );
    if ( page_size == -1 )
    {
        const char* message = "_SC_PAGESIZE is not supported.";
        kvsMessageWarning( ::GetWarningMessage( errno, message ) );
    }

    return phys_page_size * page_size;

// Mac OS X
#elif defined ( KVS_PLATFORM_MACOSX )
#if defined ( KVS_PLATFORM_CPU_64 )
    uint64_t memory_size = 0;
    size_t length = sizeof( memory_size );
    int ret = sysctlbyname( "hw.memsize", &memory_size, &length, NULL, 0 );
    if ( ret == -1 )
    {
        const char* message = strerror( errno );
        kvsMessageWarning( message );
    }
    return memory_size;
#else
    uint32_t memory_size = 0;
    size_t length = sizeof( memory_size );
    int ret = sysctlbyname( "hw.physmem", &memory_size, &length, NULL, 0 );
    if ( ret == -1 )
    {
        const char* message = strerror( errno );
        kvsMessageWarning( message );
    }
    return memory_size;
#endif
#endif
}

/*===========================================================================*/
/**
 *  @brief  Returns free memory size in bytes.
 *  @return free memory size
 */
/*===========================================================================*/
size_t SystemInformation::FreeMemorySize()
{
// Windows
#if defined ( KVS_PLATFORM_WINDOWS )
#if defined ( KVS_PLATFORM_CPU_64 )
    MEMORYSTATUSEX memstat;
    GlobalMemoryStatusEx( &memstat );
    return memstat.ullAvailPhys;
#else
    MEMORYSTATUS memstat;
    GlobalMemoryStatus( &memstat );
    return memstat.dwAvailPhys;
#endif

// Linux
#elif defined ( KVS_PLATFORM_LINUX ) || defined ( KVS_PLATFORM_CYGWIN )
    long avphys_page_size = sysconf( _SC_AVPHYS_PAGES );
    if ( avphys_page_size == -1 )
    {
        const char* message = "_SC_AVPHYS_PAGES is not supported.";
        kvsMessageWarning( ::GetWarningMessage( errno, message ) );
    }

    long page_size = sysconf( _SC_PAGESIZE );
    if ( page_size == -1 )
    {
        const char* message = "_SC_PAGESIZE is not supported.";
        kvsMessageWarning( ::GetWarningMessage( errno, message ) );
    }

    return avphys_page_size * page_size;

// Mac OS X
#elif defined ( KVS_PLATFORM_MACOSX )
    vm_size_t page_size = 0;
    {
        kern_return_t kr = host_page_size( mach_host_self(), &page_size );
        if ( kr != KERN_SUCCESS )
        {
            kvsMessageWarning( "Failure to get page size." );
        }
    }

    vm_statistics_data_t page_info;
    mach_msg_type_number_t count = HOST_VM_INFO_COUNT;
    {
        kern_return_t kr = host_statistics( mach_host_self(), HOST_VM_INFO, (host_info_t)&page_info, &count );
        if ( kr != KERN_SUCCESS )
        {
            kvsMessageWarning( "Failure to get page info." );
        }
    }

    return page_info.free_count * page_size;
#endif
}

} // end of namespace kvs
