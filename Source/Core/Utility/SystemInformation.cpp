/****************************************************************************/
/**
 *  @file SystemInformation.cpp
 */
/*----------------------------------------------------------------------------
 *
 *  Copyright (c) Visualization Laboratory, Kyoto University.
 *  All rights reserved.
 *  See http://www.viz.media.kyoto-u.ac.jp/kvs/copyright/ for details.
 *
 *  $Id: SystemInformation.cpp 1698 2014-01-16 10:49:03Z naohisa.sakamoto@gmail.com $
 */
/****************************************************************************/
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

/*==========================================================================*/
/**
 *  Get warning message.
 *  @param number [in] error number (errno)
 *  @param message [in] pointer to the message
 *  @return warning message
 */
/*==========================================================================*/
#if defined ( KVS_ENABLE_DEBUG )
const char* GetWarningMessage( int number, const char* message )
{
    return number == EINVAL ? strerror( number ) : message;
}
#endif
}


namespace kvs
{

/*==========================================================================*/
/**
 *  Get number of processors.
 *  @return number of processors
 */
/*==========================================================================*/
size_t SystemInformation::NumberOfProcessors()
{
#if defined ( KVS_PLATFORM_WINDOWS )
    SYSTEM_INFO sysinfo;
    GetSystemInfo( &sysinfo );
    return sysinfo.dwNumberOfProcessors;

#elif defined ( KVS_PLATFORM_LINUX ) || defined ( KVS_PLATFORM_CYGWIN )
    int nprocessors = sysconf( _SC_NPROCESSORS_ONLN );
    kvsMessageWarning( nprocessors != -1,
                       ::GetWarningMessage( errno, "_SC_NPROCESSORS_ONLN is not supported." ) );
    return nprocessors;

#elif defined ( KVS_PLATFORM_MACOSX )
    int nprocessors = 0;
    int mib[2] = { CTL_HW, HW_NCPU };
    size_t length = sizeof( nprocessors );
    int ret = 0;
    ret = sysctl( mib, 2, &nprocessors, &length, NULL, 0 );
    kvsMessageWarning( ret != -1, ::GetWarningMessage( errno, strerror( errno ) ) );
    return nprocessors;
#endif
}

/*==========================================================================*/
/**
 *  Get total memory size in bytes.
 *  @return total memory size
 */
/*==========================================================================*/
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
    kvsMessageWarning( phys_page_size != -1,
                       ::GetWarningMessage( errno, "_SC_PHYS_PAGES is not supported." ) );

    long page_size = sysconf( _SC_PAGESIZE );
    kvsMessageWarning( page_size != -1,
                       ::GetWarningMessage( errno, "_SC_PAGESIZE is not supported." ) );

    return phys_page_size * page_size;

// Mac OS X
#elif defined ( KVS_PLATFORM_MACOSX )
#if defined ( KVS_PLATFORM_CPU_64 )
    uint64_t memory_size = 0;
    size_t length = sizeof( memory_size );
    int ret = 0;
    ret = sysctlbyname( "hw.memsize", &memory_size, &length, NULL, 0 );
    kvsMessageWarning( ret != -1, strerror( errno ) );
    return memory_size;
#else
    uint32_t memory_size = 0;
    size_t length = sizeof( memory_size );
    int ret = 0;
    ret = sysctlbyname( "hw.physmem", &memory_size, &length, NULL, 0 );
    kvsMessageWarning( ret != -1, strerror( errno ) );
    return memory_size;
#endif
#endif
}

/*==========================================================================*/
/**
 *  Get free memory size in bytes.
 *  @return free memory size
 */
/*==========================================================================*/
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
    kvsMessageWarning( avphys_page_size != -1,
                       ::GetWarningMessage( errno, "_SC_AVPHYS_PAGES is not supported." ) );

    long page_size = sysconf( _SC_PAGESIZE );
    kvsMessageWarning( page_size != -1,
                       ::GetWarningMessage( errno, "_SC_PAGESIZE is not supported." ) );

    return avphys_page_size * page_size;

// Mac OS X
#elif defined ( KVS_PLATFORM_MACOSX )
    kern_return_t kr;

    vm_size_t page_size = 0;
    kr = host_page_size( mach_host_self(), &page_size );
    kvsMessageWarning( kr != KERN_SUCCESS, "Failure to get page size." );

    vm_statistics_data_t page_info;
    mach_msg_type_number_t count = HOST_VM_INFO_COUNT;
    kr = host_statistics( mach_host_self(), HOST_VM_INFO, (host_info_t)&page_info, &count );
    kvsMessageWarning( kr != KERN_SUCCESS, "Failure to get page info." );

    return page_info.free_count * page_size;
#endif

}

} // end of namespace kvs
