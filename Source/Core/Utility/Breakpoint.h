/****************************************************************************/
/**
 *  @file Breakpoint.h
 */
/*----------------------------------------------------------------------------
 *
 *  Copyright (c) Visualization Laboratory, Kyoto University.
 *  All rights reserved.
 *  See http://www.viz.media.kyoto-u.ac.jp/kvs/copyright/ for details.
 *
 *  $Id: Breakpoint.h 631 2010-10-10 02:15:35Z naohisa.sakamoto $
 */
/****************************************************************************/
#ifndef KVS__BREAKPOINT_H_INCLUDE
#define KVS__BREAKPOINT_H_INCLUDE

#include <kvs/Compiler>
#include <kvs/Platform>

// Microsoft Visual C++
#if defined ( KVS_COMPILER_VC )
#if KVS_COMPILER_VERSION_GREATER_THAN( 8, 0 )
#define KVS_BREAKPOINT { __debugbreak(); }
#else
#define KVS_BREAKPOINT { __asm { int 3 } }
#endif

// GNU GCC
#elif defined ( KVS_COMPILER_GCC )

#if defined ( KVS_PLATFORM_CPU_X86 ) || defined ( KVS_PLATFORM_CPU_I386 )
#define KVS_BREAKPOINT { __asm__( " int $3 " ); }

#elif defined ( KVS_PLATFORM_CPU_X86_64 ) || defined ( KVS_PLATFORM_CPU_AMD64 )
#define KVS_BREAKPOINT { __asm__( " int $3 " ); }

#elif defined ( KVS_PLATFORM_CPU_POWERPC )
#define KVS_BREAKPOINT { __asm__( " .long 0x7d821008 " ); }

#else
#pragma message("Breakpoint.h: Unknown breakpoint code.")
#define KVS_BREAKPOINT { } // Insert breakpoint code for your CPU here.
#endif

// Unknown compiler
#else
#error Unknown compiler. KVS supports GNU C++ compiler and
#error Microsoft Visual C++ compiler only.
#endif

#endif // KVS__BREAKPOINT_H_INCLUDE
