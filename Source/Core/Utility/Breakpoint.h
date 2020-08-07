/****************************************************************************/
/**
 *  @file   Breakpoint.h
 *  @author Naohisa Sakamoto
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
#  elif defined ( KVS_PLATFORM_CPU_ARM )
#    define KVS_BREAKPOINT { __asm__( "bkpt #0" ); }
#  elif defined ( KVS_PLATFORM_CPU_ARM64 )
#    define KVS_BREAKPOINT { __asm__( "brk #0" ); }
#  else
#    pragma message("Breakpoint.h: Unknown breakpoint code for the CPU architecture.")
#    define KVS_BREAKPOINT { }
#  endif

// Unknown compiler
#else
#  pragma message("Breakpoint.h: Unknown breakpoint code for the compiler.")
#  define KVS_BREAKPOINT { }
#endif
