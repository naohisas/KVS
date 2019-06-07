/****************************************************************************/
/**
 *  @file   Breakpoint.h
 *  @author Naohisa Sakamoto
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
#pragma once
#include <kvs/Compiler>
#include <kvs/Platform>


// Microsoft Visual C++
#if defined ( KVS_COMPILER_VC )
#  if KVS_COMPILER_VERSION_GREATER_THAN( 8, 0 )
#    define KVS_BREAKPOINT { __debugbreak(); }
#  else
#    define KVS_BREAKPOINT { __asm { int 3 } }
#  endif

// GNU GCC
#elif defined ( KVS_COMPILER_GCC )
#  if defined ( KVS_PLATFORM_CPU_X86 ) || defined ( KVS_PLATFORM_CPU_I386 )
#    define KVS_BREAKPOINT { __asm__( "int $3" ); }
#  elif defined ( KVS_PLATFORM_CPU_X86_64 ) || defined ( KVS_PLATFORM_CPU_AMD64 )
#    define KVS_BREAKPOINT { __asm__( "int $3" ); }
#  elif defined ( KVS_PLATFORM_CPU_POWERPC )
#    define KVS_BREAKPOINT { __asm__( ".long 0x7d821008" ); }
#  elif defined ( KVS_PLATFORM_CPU_SPARC )
#    define KVS_BREAKPOINT { __asm__( "unimp 0" ); }
#  elif defined ( KVS_PLATFORM_CPU_SPARC64 )
#    define KVS_BREAKPOINT { __asm__( "illtrap 0" ); }
#  else
#    pragma message("Breakpoint.h: Unknown breakpoint code for the CPU architecture.")
#    define KVS_BREAKPOINT { }
#  endif

// Unknown compiler
#else
#  pragma message("Breakpoint.h: Unknown breakpoint code for the compiler.")
#  define KVS_BREAKPOINT { }
#endif
