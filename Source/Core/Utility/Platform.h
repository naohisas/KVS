/****************************************************************************/
/**
 *  @file   Platform.h
 *  @author Naohisa Sakamoto
 */
/*----------------------------------------------------------------------------
 *
 *  Copyright (c) Visualization Laboratory, Kyoto University.
 *  All rights reserved.
 *  See http://www.viz.media.kyoto-u.ac.jp/kvs/copyright/ for details.
 *
 *  $Id: Platform.h 1109 2012-04-21 11:55:54Z s.yamada0808@gmail.com $
 */
/****************************************************************************/
#pragma once


/*----------------------------------------------------------------------------
 * Platform name.
 *----------------------------------------------------------------------------*/

// Linux
#if defined ( __linux__ ) || defined ( __linux )
#define KVS_PLATFORM_LINUX
#define KVS_PLATFORM_NAME "Linux"

// Mac OS X
#elif defined ( __APPLE__ ) || defined ( __MACOSX__ ) || defined ( macosx )
#define KVS_PLATFORM_MACOSX
#define KVS_PLATFORM_NAME "Mac OS X"

// Windows
#elif defined ( __WIN32__ ) || defined ( _WIN32 ) || defined ( WIN32 ) || \
      defined ( __WIN64__ ) || defined ( _WIN64 ) || defined ( WIN64 )
#define KVS_PLATFORM_WINDOWS
#define KVS_PLATFORM_NAME "Windows"

// Cygwin
#elif defined ( __CYGWIN__ ) || defined ( __CYGWIN32__ )
#define KVS_PLATFORM_CYGWIN
#define KVS_PLATFORM_NAME "Cygwin"

// FreeBSD
#elif defined ( __FreeBSD__ )
#define KVS_PLATFORM_FREEBSD
#define KVS_PLATFORM_NAME "FreeBSD"

// Solaris
#elif defined ( __sun__ ) || defined ( __sun ) || defined ( sun ) || \
      defined ( __solaris__ )
#define KVS_PLATFORM_SOLARIS
#define KVS_PLATFORM_NAME "Solaris"

// SGI Irix
#elif defined ( __sgi__ )
#define KVS_PLATFORM_IRIX
#define KVS_PLATFORM_NAME "Irix"

// HP-UX
#elif defined ( __hpux__ ) || defined ( __hpux )
#define KVS_PLATFORM_HPUX
#define KVS_PLATFORM_NAME "HP-UX"

// AIX
#elif defined ( __AIX ) || defined ( _AIX )
#define KVS_PLATFORM_AIX
#define KVS_PLATFORM_NAME "AIX"

#else
#pragma message("Platform.h: Unknown platform.")
#define KVS_PLATFORM_UNKNOWN
#define KVS_PLATFORM_NAME "Unknown"
#endif


/*----------------------------------------------------------------------------
 * CPU architecture.
 *----------------------------------------------------------------------------*/
#define KVS_PLATFORM_CPU_UNKNOWN
#define KVS_PLATFORM_CPU_NAME "Unknown"

// i386
#if defined ( __i386__ ) || defined ( __i386 ) || defined ( i386 )
#undef  KVS_PLATFORM_CPU_UNKNOWN
#undef  KVS_PLATFORM_CPU_NAME
#define KVS_PLATFORM_CPU_I386
#define KVS_PLATFORM_CPU_NAME "i386"
#endif

// x86
#if defined ( __x86__ ) || defined ( _X86_ ) || defined ( _M_IX86 )
#undef  KVS_PLATFORM_CPU_UNKNOWN
#undef  KVS_PLATFORM_CPU_NAME
#define KVS_PLATFORM_CPU_X86
#define KVS_PLATFORM_CPU_NAME "x86"
#endif

// x86 64
#if defined ( __x86_64__ )
#undef  KVS_PLATFORM_CPU_UNKNOWN
#undef  KVS_PLATFORM_CPU_NAME
#define KVS_PLATFORM_CPU_X86_64
#define KVS_PLATFORM_CPU_NAME "x86-64"
#endif

// AMD64
#if defined ( __amd64__ ) || defined ( _M_AMD64 )
#undef  KVS_PLATFORM_CPU_UNKNOWN
#undef  KVS_PLATFORM_CPU_NAME
#define KVS_PLATFORM_CPU_AMD64
#define KVS_PLATFORM_CPU_NAME "AMD64"
#endif

// Intel IA64
#if defined ( __ia64__ ) || defined ( __ia64 ) || defined ( _M_IA64 )
#undef  KVS_PLATFORM_CPU_UNKNOWN
#undef  KVS_PLATFORM_CPU_NAME
#define KVS_PLATFORM_CPU_IA64
#define KVS_PLATFORM_CPU_NAME "IA64"
#endif

// PowerPC
#if defined ( __POWERPC__ ) || defined ( __powerpc__ ) || defined ( _M_MPPC ) || \
    defined ( __powerpc64__ )
#undef  KVS_PLATFORM_CPU_UNKNOWN
#undef  KVS_PLATFORM_CPU_NAME
#define KVS_PLATFORM_CPU_POWERPC
#define KVS_PLATFORM_CPU_NAME "PowerPC"
#endif

// Sun SPARC
#if defined ( __sparc__ ) || defined ( __sparc )
#undef  KVS_PLATFORM_CPU_UNKNOWN
#undef  KVS_PLATFORM_CPU_NAME
#if defined ( __arch64__ ) || defined ( __sparc_v9 ) || defined ( __sparcv9 )
#define KVS_PLATFORM_CPU_SPARC64
#define KVS_PLATFORM_CPU_NAME "Sun SPARC 64"
#else
#define KVS_PLATFORM_CPU_SPARC
#define KVS_PLATFORM_CPU_NAME "Sun SPARC"
#endif
#endif

// MIPS
#if defined ( __mips__ ) || defined ( __MIPS__ ) || defined ( _M_MRX000 )
#undef  KVS_PLATFORM_CPU_UNKNOWN
#undef  KVS_PLATFORM_CPU_NAME
#define KVS_PLATFORM_CPU_MIPS
#define KVS_PLATFORM_CPU_NAME "MIPS"
#endif

#if defined ( KVS_PLATFORM_CPU_UNKNOWN )
#pragma message("Platform.h: Unknown CPU architecture.")
#endif

/*----------------------------------------------------------------------------
 * Number of bits of CPU architechture.
 *----------------------------------------------------------------------------*/

#if defined ( __x86_64__ ) ||                                           \
    defined ( _WIN64 ) || defined ( _M_X64 ) ||                         \
    defined ( __amd64__ ) || defined( _M_AMD64 ) ||                     \
    defined ( __ia64__ ) || defined ( __ia64 ) || defined ( _M_IA64 ) || \
    defined ( __powerpc64__ ) ||                                        \
    defined ( __arch64__ )
#define KVS_PLATFORM_CPU_64
#else
#define KVS_PLATFORM_CPU_32
#endif

/*----------------------------------------------------------------------------
 * Byte-order.
 *----------------------------------------------------------------------------*/

#if defined ( __LITTLE_ENDIAN__ )
#define KVS_PLATFORM_LITTLE_ENDIAN

#elif defined ( __BIG_ENDIAN__ )
#define KVS_PLATFORM_BIG_ENDIAN

#else
#if defined ( KVS_PLATFORM_CPU_I386 )
#define KVS_PLATFORM_LITTLE_ENDIAN

#elif defined ( KVS_PLATFORM_CPU_X86 )
#define KVS_PLATFORM_LITTLE_ENDIAN

#elif defined ( KVS_PLATFORM_CPU_X86_64 )
#define KVS_PLATFORM_LITTLE_ENDIAN

#elif defined ( KVS_PLATFORM_CPU_AMD64 )
#define KVS_PLATFORM_LITTLE_ENDIAN

#elif defined ( KVS_PLATFORM_CPU_IA64 )
#define KVS_PLATFORM_LITTLE_ENDIAN

#elif defined ( KVS_PLATFORM_CPU_POWERPC )
#define KVS_PLATFORM_BIG_ENDIAN

#elif defined ( KVS_PLATFORM_CPU_SPARC64 )
#define KVS_PLATFORM_BIG_ENDIAN

#elif defined ( KVS_PLATFORM_CPU_SPARC )
#define KVS_PLATFORM_BIG_ENDIAN

#elif defined ( KVS_PLATFORM_CPU_MIPS )
#define KVS_PLATFORM_BIG_ENDIAN

#else
//#warning Unknown byte-order of the platform.
#pragma message("Platform.h: Unknown byte-order.")
#endif

#endif


namespace kvs
{

class Platform
{
public:
    static const char* Name() { return KVS_PLATFORM_NAME; }
    static const char* CPUName() { return KVS_PLATFORM_CPU_NAME; }

private:
    Platform();
};

} // end of namespace kvs
