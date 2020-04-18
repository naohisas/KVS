/****************************************************************************/
/**
 *  @file   Compiler.h
 *  @author Naohisa Sakamoto
 */
/*----------------------------------------------------------------------------
 *
 *  Copyright (c) Visualization Laboratory, Kyoto University.
 *  All rights reserved.
 *  See http://www.viz.media.kyoto-u.ac.jp/kvs/copyright/ for details.
 *
 *  $Id: Compiler.h 1582 2013-06-03 01:31:06Z naohisa.sakamoto@gmail.com $
 */
/****************************************************************************/
#pragma once
#define KVS_COMPILER_NUMBER( x )           # x
#define KVS_COMPILER_NUMBER_TO_STRING( x ) KVS_COMPILER_NUMBER( x )

/*----------------------------------------------------------------------------
 * GNU C/C++ compiler
 *----------------------------------------------------------------------------*/
#if defined ( __GNUC__ )
#define KVS_COMPILER_GCC

#define KVS_COMPILER_NAME "GNU C/C++"
#define KVS_COMPILER_VERSION                            \
    KVS_COMPILER_NUMBER_TO_STRING( __GNUC__ ) "."       \
    KVS_COMPILER_NUMBER_TO_STRING( __GNUC_MINOR__ ) "." \
    KVS_COMPILER_NUMBER_TO_STRING( __GNUC_PATCHLEVEL__ )

#define KVS_COMPILER_VERSION_IS( major, minor ) \
    ( __GNUC__ == ( major ) && __GNUC_MINOR__ == ( minor ) )
#define KVS_COMPILER_VERSION_LESS_THAN( major, minor ) \
    ( __GNUC__ < ( major ) || ( __GNUC__ == ( major ) && __GNUC_MINOR__ < ( minor ) ) )
#define KVS_COMPILER_VERSION_LESS_OR_EQUAL( major, minor ) \
    ( __GNUC__ < ( major ) || ( __GNUC__ == ( major ) && __GNUC_MINOR__ <= ( minor ) ) )
#define KVS_COMPILER_VERSION_GREATER_THAN( major, minor ) \
    ( __GNUC__ > ( major ) || ( __GNUC__ == ( major ) && __GNUC_MINOR__ > ( minor ) ) )
#define KVS_COMPILER_VERSION_GREATER_OR_EQUAL( major, minor ) \
    ( __GNUC__ > ( major ) || ( __GNUC__ == ( major ) && __GNUC_MINOR__ >= ( minor ) ) )

/*----------------------------------------------------------------------------
 * Microsoft Visual C/C++ compiler
 *----------------------------------------------------------------------------*/
#elif defined ( _MSC_VER )
#define KVS_COMPILER_VC

#define KVS_COMPILER_NAME "Microsoft Visual C/C++"

// Visual Studio 6.0
#if   ( _MSC_VER == 1200 )
#define KVS_COMPILER_VERSION "6.0"
#define KVS_COMPILER_VERSION_IS( major, minor ) ( 6 == ( major ) && 0 == ( minor ) )
#define KVS_COMPILER_VERSION_LESS_THAN( major, minor ) \
    ( 6 < ( major ) || ( 6 == ( major ) && 0 < ( minor ) ) )
#define KVS_COMPILER_VERSION_LESS_OR_EQUAL( major, minor ) \
    ( 6 < ( major ) || ( 6 == ( major ) && 0 <= ( minor ) ) )
#define KVS_COMPILER_VERSION_GREATER_THAN( major, minor ) \
    ( 6 > ( major ) || ( 6 == ( major ) && 0 > ( minor ) ) )
#define KVS_COMPILER_VERSION_GREATER_OR_EQUAL( major, minor ) \
    ( 6 > ( major ) || ( 6 == ( major ) && 0 >= ( minor ) ) )

// Visual Studio 2002
#elif ( _MSC_VER == 1300 )
#define KVS_COMPILER_VERSION "7.0"
#define KVS_COMPILER_VERSION_IS( major, minor ) ( 7 == ( major ) && 0 == ( minor ) )
#define KVS_COMPILER_VERSION_LESS_THAN( major, minor ) \
    ( 7 < ( major ) || ( 7 == ( major ) && 0 < ( minor ) ) )
#define KVS_COMPILER_VERSION_LESS_OR_EQUAL( major, minor ) \
    ( 7 < ( major ) || ( 7 == ( major ) && 0 <= ( minor ) ) )
#define KVS_COMPILER_VERSION_GREATER_THAN( major, minor ) \
    ( 7 > ( major ) || ( 7 == ( major ) && 0 > ( minor ) ) )
#define KVS_COMPILER_VERSION_GREATER_OR_EQUAL( major, minor ) \
    ( 7 > ( major ) || ( 7 == ( major ) && 0 >= ( minor ) ) )

// Visual Studio 2003
#elif ( _MSC_VER == 1310 )
#define KVS_COMPILER_VERSION "7.1"
#define KVS_COMPILER_VERSION_IS( major, minor ) ( 7 == ( major ) && 1 == ( minor ) )
#define KVS_COMPILER_VERSION_LESS_THAN( major, minor ) \
    ( 7 < ( major ) || ( 7 == ( major ) && 1 < ( minor ) ) )
#define KVS_COMPILER_VERSION_LESS_OR_EQUAL( major, minor ) \
    ( 7 < ( major ) || ( 7 == ( major ) && 1 <= ( minor ) ) )
#define KVS_COMPILER_VERSION_GREATER_THAN( major, minor ) \
    ( 7 > ( major ) || ( 7 == ( major ) && 1 > ( minor ) ) )
#define KVS_COMPILER_VERSION_GREATER_OR_EQUAL( major, minor ) \
    ( 7 > ( major ) || ( 7 == ( major ) && 1 >= ( minor ) ) )

// Visual Studio 2005
#elif ( _MSC_VER == 1400 )
#define KVS_COMPILER_VERSION "8.0"
#define KVS_COMPILER_VERSION_IS( major, minor ) ( 8 == ( major ) && 0 == ( minor ) )
#define KVS_COMPILER_VERSION_LESS_THAN( major, minor ) \
    ( 8 < ( major ) || ( 8 == ( major ) && 0 < ( minor ) ) )
#define KVS_COMPILER_VERSION_LESS_OR_EQUAL( major, minor ) \
    ( 8 < ( major ) || ( 8 == ( major ) && 0 <= ( minor ) ) )
#define KVS_COMPILER_VERSION_GREATER_THAN( major, minor ) \
    ( 8 > ( major ) || ( 8 == ( major ) && 0 > ( minor ) ) )
#define KVS_COMPILER_VERSION_GREATER_OR_EQUAL( major, minor ) \
    ( 8 > ( major ) || ( 8 == ( major ) && 0 >= ( minor ) ) )

// Visual Studio 2008
#elif ( _MSC_VER == 1500 )
#define KVS_COMPILER_VERSION "9.0"
#define KVS_COMPILER_VERSION_IS( major, minor ) ( 9 == ( major ) && 0 == ( minor ) )
#define KVS_COMPILER_VERSION_LESS_THAN( major, minor ) \
    ( 9 < ( major ) || ( 9 == ( major ) && 0 < ( minor ) ) )
#define KVS_COMPILER_VERSION_LESS_OR_EQUAL( major, minor ) \
    ( 9 < ( major ) || ( 9 == ( major ) && 0 <= ( minor ) ) )
#define KVS_COMPILER_VERSION_GREATER_THAN( major, minor ) \
    ( 9 > ( major ) || ( 9 == ( major ) && 0 > ( minor ) ) )
#define KVS_COMPILER_VERSION_GREATER_OR_EQUAL( major, minor ) \
    ( 9 > ( major ) || ( 9 == ( major ) && 0 >= ( minor ) ) )

// Visual Studio 2010
#elif ( _MSC_VER == 1600 )
#define KVS_COMPILER_VERSION "10.0"
#define KVS_COMPILER_VERSION_IS( major, minor ) ( 10 == ( major ) && 0 == ( minor ) )
#define KVS_COMPILER_VERSION_LESS_THAN( major, minor ) \
    ( 10 < ( major ) || ( 10 == ( major ) && 0 < ( minor ) ) )
#define KVS_COMPILER_VERSION_LESS_OR_EQUAL( major, minor ) \
    ( 10 < ( major ) || ( 10 == ( major ) && 0 <= ( minor ) ) )
#define KVS_COMPILER_VERSION_GREATER_THAN( major, minor ) \
    ( 10 > ( major ) || ( 10 == ( major ) && 0 > ( minor ) ) )
#define KVS_COMPILER_VERSION_GREATER_OR_EQUAL( major, minor ) \
    ( 10 > ( major ) || ( 10 == ( major ) && 0 >= ( minor ) ) )

// Visual Studio 2012
#elif ( _MSC_VER == 1700 )
#define KVS_COMPILER_VERSION "11.0"
#define KVS_COMPILER_VERSION_IS( major, minor ) ( 11 == ( major ) && 0 == ( minor ) )
#define KVS_COMPILER_VERSION_LESS_THAN( major, minor ) \
    ( 11 < ( major ) || ( 11 == ( major ) && 0 < ( minor ) ) )
#define KVS_COMPILER_VERSION_LESS_OR_EQUAL( major, minor ) \
    ( 11 < ( major ) || ( 11 == ( major ) && 0 <= ( minor ) ) )
#define KVS_COMPILER_VERSION_GREATER_THAN( major, minor ) \
    ( 11 > ( major ) || ( 11 == ( major ) && 0 > ( minor ) ) )
#define KVS_COMPILER_VERSION_GREATER_OR_EQUAL( major, minor ) \
    ( 11 > ( major ) || ( 11 == ( major ) && 0 >= ( minor ) ) )

// Visual Studio 2013
#elif ( _MSC_VER == 1800 )
#define KVS_COMPILER_VERSION "12.0"
#define KVS_COMPILER_VERSION_IS( major, minor ) ( 12 == ( major ) && 0 == ( minor ) )
#define KVS_COMPILER_VERSION_LESS_THAN( major, minor ) \
    ( 12 < ( major ) || ( 12 == ( major ) && 0 < ( minor ) ) )
#define KVS_COMPILER_VERSION_LESS_OR_EQUAL( major, minor ) \
    ( 12 < ( major ) || ( 12 == ( major ) && 0 <= ( minor ) ) )
#define KVS_COMPILER_VERSION_GREATER_THAN( major, minor ) \
    ( 12 > ( major ) || ( 12 == ( major ) && 0 > ( minor ) ) )
#define KVS_COMPILER_VERSION_GREATER_OR_EQUAL( major, minor ) \
    ( 12 > ( major ) || ( 12 == ( major ) && 0 >= ( minor ) ) )

// Visual Studio 2015
#elif ( _MSC_VER == 1900 )
#define KVS_COMPILER_VERSION "14.0"
#define KVS_COMPILER_VERSION_IS( major, minor ) ( 14 == ( major ) && 0 == ( minor ) )
#define KVS_COMPILER_VERSION_LESS_THAN( major, minor ) \
    ( 14 < ( major ) || ( 14 == ( major ) && 0 < ( minor ) ) )
#define KVS_COMPILER_VERSION_LESS_OR_EQUAL( major, minor ) \
    ( 14 < ( major ) || ( 14 == ( major ) && 0 <= ( minor ) ) )
#define KVS_COMPILER_VERSION_GREATER_THAN( major, minor ) \
    ( 14 > ( major ) || ( 14 == ( major ) && 0 > ( minor ) ) )
#define KVS_COMPILER_VERSION_GREATER_OR_EQUAL( major, minor ) \
    ( 14 > ( major ) || ( 14 == ( major ) && 0 >= ( minor ) ) )

// Visual Studio 2017
#elif ( _MSC_VER == 1910 )
#define KVS_COMPILER_VERSION "15.0"
#define KVS_COMPILER_VERSION_IS( major, minor ) ( 15 == ( major ) && 0 == ( minor ) )
#define KVS_COMPILER_VERSION_LESS_THAN( major, minor ) \
    ( 15 < ( major ) || ( 15 == ( major ) && 0 < ( minor ) ) )
#define KVS_COMPILER_VERSION_LESS_OR_EQUAL( major, minor ) \
    ( 15 < ( major ) || ( 15 == ( major ) && 0 <= ( minor ) ) )
#define KVS_COMPILER_VERSION_GREATER_THAN( major, minor ) \
    ( 15 > ( major ) || ( 15 == ( major ) && 0 > ( minor ) ) )
#define KVS_COMPILER_VERSION_GREATER_OR_EQUAL( major, minor ) \
    ( 15 > ( major ) || ( 15 == ( major ) && 0 >= ( minor ) ) )

// Visual Studio 2017 Update 3
#elif ( _MSC_VER == 1911 )
#define KVS_COMPILER_VERSION "15.3"
#define KVS_COMPILER_VERSION_IS( major, minor ) ( 15 == ( major ) && 3 == ( minor ) )
#define KVS_COMPILER_VERSION_LESS_THAN( major, minor ) \
    ( 15 < ( major ) || ( 15 == ( major ) && 3 < ( minor ) ) )
#define KVS_COMPILER_VERSION_LESS_OR_EQUAL( major, minor ) \
    ( 15 < ( major ) || ( 15 == ( major ) && 3 <= ( minor ) ) )
#define KVS_COMPILER_VERSION_GREATER_THAN( major, minor ) \
    ( 15 > ( major ) || ( 15 == ( major ) && 3 > ( minor ) ) )
#define KVS_COMPILER_VERSION_GREATER_OR_EQUAL( major, minor ) \
    ( 15 > ( major ) || ( 15 == ( major ) && 3 >= ( minor ) ) )

// Visual Studio 2017 Update 5
#elif ( _MSC_VER == 1912 )
#define KVS_COMPILER_VERSION "15.5"
#define KVS_COMPILER_VERSION_IS( major, minor ) ( 15 == ( major ) && 5 == ( minor ) )
#define KVS_COMPILER_VERSION_LESS_THAN( major, minor ) \
    ( 15 < ( major ) || ( 15 == ( major ) && 5 < ( minor ) ) )
#define KVS_COMPILER_VERSION_LESS_OR_EQUAL( major, minor ) \
    ( 15 < ( major ) || ( 15 == ( major ) && 5 <= ( minor ) ) )
#define KVS_COMPILER_VERSION_GREATER_THAN( major, minor ) \
    ( 15 > ( major ) || ( 15 == ( major ) && 5 > ( minor ) ) )
#define KVS_COMPILER_VERSION_GREATER_OR_EQUAL( major, minor ) \
    ( 15 > ( major ) || ( 15 == ( major ) && 5 >= ( minor ) ) )

// Visual Studio 2017 Update 6
#elif ( _MSC_VER == 1913 )
#define KVS_COMPILER_VERSION "15.6"
#define KVS_COMPILER_VERSION_IS( major, minor ) ( 15 == ( major ) && 6 == ( minor ) )
#define KVS_COMPILER_VERSION_LESS_THAN( major, minor ) \
    ( 15 < ( major ) || ( 15 == ( major ) && 6 < ( minor ) ) )
#define KVS_COMPILER_VERSION_LESS_OR_EQUAL( major, minor ) \
    ( 15 < ( major ) || ( 15 == ( major ) && 6 <= ( minor ) ) )
#define KVS_COMPILER_VERSION_GREATER_THAN( major, minor ) \
    ( 15 > ( major ) || ( 15 == ( major ) && 6 > ( minor ) ) )
#define KVS_COMPILER_VERSION_GREATER_OR_EQUAL( major, minor ) \
    ( 15 > ( major ) || ( 15 == ( major ) && 5 >= ( minor ) ) )

// Visual Studio 2017 Update 7
#elif ( _MSC_VER == 1914 )
#define KVS_COMPILER_VERSION "15.7"
#define KVS_COMPILER_VERSION_IS( major, minor ) ( 15 == ( major ) && 7 == ( minor ) )
#define KVS_COMPILER_VERSION_LESS_THAN( major, minor ) \
    ( 15 < ( major ) || ( 15 == ( major ) && 7 < ( minor ) ) )
#define KVS_COMPILER_VERSION_LESS_OR_EQUAL( major, minor ) \
    ( 15 < ( major ) || ( 15 == ( major ) && 7 <= ( minor ) ) )
#define KVS_COMPILER_VERSION_GREATER_THAN( major, minor ) \
    ( 15 > ( major ) || ( 15 == ( major ) && 7 > ( minor ) ) )
#define KVS_COMPILER_VERSION_GREATER_OR_EQUAL( major, minor ) \
    ( 15 > ( major ) || ( 15 == ( major ) && 7 >= ( minor ) ) )

// Visual Studio 2017 Update 8
#elif ( _MSC_VER == 1915 )
#define KVS_COMPILER_VERSION "15.8"
#define KVS_COMPILER_VERSION_IS( major, minor ) ( 15 == ( major ) && 8 == ( minor ) )
#define KVS_COMPILER_VERSION_LESS_THAN( major, minor ) \
    ( 15 < ( major ) || ( 15 == ( major ) && 8 < ( minor ) ) )
#define KVS_COMPILER_VERSION_LESS_OR_EQUAL( major, minor ) \
    ( 15 < ( major ) || ( 15 == ( major ) && 8 <= ( minor ) ) )
#define KVS_COMPILER_VERSION_GREATER_THAN( major, minor ) \
    ( 15 > ( major ) || ( 15 == ( major ) && 8 > ( minor ) ) )
#define KVS_COMPILER_VERSION_GREATER_OR_EQUAL( major, minor ) \
    ( 15 > ( major ) || ( 15 == ( major ) && 8 >= ( minor ) ) )

// Visual Studio 2017 Update 9
#elif ( _MSC_VER == 1916 )
#define KVS_COMPILER_VERSION "15.9"
#define KVS_COMPILER_VERSION_IS( major, minor ) ( 15 == ( major ) && 9 == ( minor ) )
#define KVS_COMPILER_VERSION_LESS_THAN( major, minor ) \
    ( 15 < ( major ) || ( 15 == ( major ) && 9 < ( minor ) ) )
#define KVS_COMPILER_VERSION_LESS_OR_EQUAL( major, minor ) \
    ( 15 < ( major ) || ( 15 == ( major ) && 9 <= ( minor ) ) )
#define KVS_COMPILER_VERSION_GREATER_THAN( major, minor ) \
    ( 15 > ( major ) || ( 15 == ( major ) && 9 > ( minor ) ) )
#define KVS_COMPILER_VERSION_GREATER_OR_EQUAL( major, minor ) \
    ( 15 > ( major ) || ( 15 == ( major ) && 9 >= ( minor ) ) )

// Visual Studio 2019
#elif ( _MSC_VER == 1920 )
#define KVS_COMPILER_VERSION "16.0"
#define KVS_COMPILER_VERSION_IS( major, minor ) ( 16 == ( major ) && 0 == ( minor ) )
#define KVS_COMPILER_VERSION_LESS_THAN( major, minor ) \
    ( 16 < ( major ) || ( 16 == ( major ) && 0 < ( minor ) ) )
#define KVS_COMPILER_VERSION_LESS_OR_EQUAL( major, minor ) \
    ( 16 < ( major ) || ( 16 == ( major ) && 0 <= ( minor ) ) )
#define KVS_COMPILER_VERSION_GREATER_THAN( major, minor ) \
    ( 16 > ( major ) || ( 16 == ( major ) && 0 > ( minor ) ) )
#define KVS_COMPILER_VERSION_GREATER_OR_EQUAL( major, minor ) \
    ( 16 > ( major ) || ( 16 == ( major ) && 0 >= ( minor ) ) )

// Visual Studio 2019 Update 1
#elif ( _MSC_VER == 1921 )
#define KVS_COMPILER_VERSION "16.1"
#define KVS_COMPILER_VERSION_IS( major, minor ) ( 16 == ( major ) && 1 == ( minor ) )
#define KVS_COMPILER_VERSION_LESS_THAN( major, minor ) \
    ( 16 < ( major ) || ( 16 == ( major ) && 1 < ( minor ) ) )
#define KVS_COMPILER_VERSION_LESS_OR_EQUAL( major, minor ) \
    ( 16 < ( major ) || ( 16 == ( major ) && 1 <= ( minor ) ) )
#define KVS_COMPILER_VERSION_GREATER_THAN( major, minor ) \
    ( 16 > ( major ) || ( 16 == ( major ) && 1 > ( minor ) ) )
#define KVS_COMPILER_VERSION_GREATER_OR_EQUAL( major, minor ) \
    ( 16 > ( major ) || ( 16 == ( major ) && 1 >= ( minor ) ) )

// Visual Studio 2019 Update 2
#elif ( _MSC_VER == 1922 )
#define KVS_COMPILER_VERSION "16.2"
#define KVS_COMPILER_VERSION_IS( major, minor ) ( 16 == ( major ) && 2 == ( minor ) )
#define KVS_COMPILER_VERSION_LESS_THAN( major, minor ) \
    ( 16 < ( major ) || ( 16 == ( major ) && 2 < ( minor ) ) )
#define KVS_COMPILER_VERSION_LESS_OR_EQUAL( major, minor ) \
    ( 16 < ( major ) || ( 16 == ( major ) && 2 <= ( minor ) ) )
#define KVS_COMPILER_VERSION_GREATER_THAN( major, minor ) \
    ( 16 > ( major ) || ( 16 == ( major ) && 2 > ( minor ) ) )
#define KVS_COMPILER_VERSION_GREATER_OR_EQUAL( major, minor ) \
    ( 16 > ( major ) || ( 16 == ( major ) && 2 >= ( minor ) ) )

// Visual Studio 2019 Update 3
#elif ( _MSC_VER == 1923 )
#define KVS_COMPILER_VERSION "16.3"
#define KVS_COMPILER_VERSION_IS( major, minor ) ( 16 == ( major ) && 3 == ( minor ) )
#define KVS_COMPILER_VERSION_LESS_THAN( major, minor ) \
    ( 16 < ( major ) || ( 16 == ( major ) && 3 < ( minor ) ) )
#define KVS_COMPILER_VERSION_LESS_OR_EQUAL( major, minor ) \
    ( 16 < ( major ) || ( 16 == ( major ) && 3 <= ( minor ) ) )
#define KVS_COMPILER_VERSION_GREATER_THAN( major, minor ) \
    ( 16 > ( major ) || ( 16 == ( major ) && 3 > ( minor ) ) )
#define KVS_COMPILER_VERSION_GREATER_OR_EQUAL( major, minor ) \
    ( 16 > ( major ) || ( 16 == ( major ) && 3 >= ( minor ) ) )

// Visual Studio 2019 Update 4
#elif ( _MSC_VER == 1924 )
#define KVS_COMPILER_VERSION "16.4"
#define KVS_COMPILER_VERSION_IS( major, minor ) ( 16 == ( major ) && 4 == ( minor ) )
#define KVS_COMPILER_VERSION_LESS_THAN( major, minor ) \
    ( 16 < ( major ) || ( 16 == ( major ) && 4 < ( minor ) ) )
#define KVS_COMPILER_VERSION_LESS_OR_EQUAL( major, minor ) \
    ( 16 < ( major ) || ( 16 == ( major ) && 4 <= ( minor ) ) )
#define KVS_COMPILER_VERSION_GREATER_THAN( major, minor ) \
    ( 16 > ( major ) || ( 16 == ( major ) && 4 > ( minor ) ) )
#define KVS_COMPILER_VERSION_GREATER_OR_EQUAL( major, minor ) \
    ( 16 > ( major ) || ( 16 == ( major ) && 4 >= ( minor ) ) )

// Visual Studio 2019 Update 5
#elif ( _MSC_VER == 1925 )
#define KVS_COMPILER_VERSION "16.5"
#define KVS_COMPILER_VERSION_IS( major, minor ) ( 16 == ( major ) && 5 == ( minor ) )
#define KVS_COMPILER_VERSION_LESS_THAN( major, minor ) \
    ( 16 < ( major ) || ( 16 == ( major ) && 5 < ( minor ) ) )
#define KVS_COMPILER_VERSION_LESS_OR_EQUAL( major, minor ) \
    ( 16 < ( major ) || ( 16 == ( major ) && 5 <= ( minor ) ) )
#define KVS_COMPILER_VERSION_GREATER_THAN( major, minor ) \
    ( 16 > ( major ) || ( 16 == ( major ) && 5 > ( minor ) ) )
#define KVS_COMPILER_VERSION_GREATER_OR_EQUAL( major, minor ) \
    ( 16 > ( major ) || ( 16 == ( major ) && 5 >= ( minor ) ) )

// Visual Studio 2019 Update 6
#elif ( _MSC_VER == 1926 )
#define KVS_COMPILER_VERSION "16.6"
#define KVS_COMPILER_VERSION_IS( major, minor ) ( 16 == ( major ) && 6 == ( minor ) )
#define KVS_COMPILER_VERSION_LESS_THAN( major, minor ) \
    ( 16 < ( major ) || ( 16 == ( major ) && 6 < ( minor ) ) )
#define KVS_COMPILER_VERSION_LESS_OR_EQUAL( major, minor ) \
    ( 16 < ( major ) || ( 16 == ( major ) && 6 <= ( minor ) ) )
#define KVS_COMPILER_VERSION_GREATER_THAN( major, minor ) \
    ( 16 > ( major ) || ( 16 == ( major ) && 6 > ( minor ) ) )
#define KVS_COMPILER_VERSION_GREATER_OR_EQUAL( major, minor ) \
    ( 16 > ( major ) || ( 16 == ( major ) && 6 >= ( minor ) ) )

#else
#define KVS_COMPILER_VERSION "unknown"
#define KVS_COMPILER_VERSION_IS( major, minor )
#define KVS_COMPILER_VERSION_LESS_THAN( major, minor )
#define KVS_COMPILER_VERSION_LESS_OR_EQUAL( major, minor )
#define KVS_COMPILER_VERSION_GREATER_THAN( major, minor )
#define KVS_COMPILER_VERSION_GREATER_OR_EQUAL( major, minor )
#endif

/*----------------------------------------------------------------------------
 * Fujitsu C/C++ compiler
 *----------------------------------------------------------------------------*/
#elif defined ( __FCC_VERSION )
#define KVS_COMPILER_FCC

#define KVS_COMPILER_NAME "Fujitsu C/C++"
#define KVS_COMPILER_VERSION KVS_COMPILER_NUMBER_TO_STRING( __FCC_VERSION )
#define KVS_COMPILER_VERSION_IS( major, minor )
#define KVS_COMPILER_VERSION_LESS_THAN( major, minor )
#define KVS_COMPILER_VERSION_LESS_OR_EQUAL( major, minor )
#define KVS_COMPILER_VERSION_GREATER_THAN( major, minor )
#define KVS_COMPILER_VERSION_GREATER_OR_EQUAL( major, minor )

/*----------------------------------------------------------------------------
 * Unknown compiler
 *----------------------------------------------------------------------------*/
#else
#define KVS_COMPILER_UNKNOWN
#define KVS_COMPILER_NAME "Unknown"
#endif


/*----------------------------------------------------------------------------
 * C++ version
 *----------------------------------------------------------------------------*/
#define KVS_COMPILER_SUPPORT_CXX98              \
    ( __cplusplus >= 199711L )
#define KVS_COMPILER_SUPPORT_CXX11              \
    ( __cplusplus >= 201103L )
#define KVS_COMPILER_SUPPORT_CXX14              \
    ( __cplusplus >= 201402L )
#define KVS_COMPILER_SUPPORT_CXX17              \
    ( __cplusplus >= 201703L )
#define KVS_COMPILER_SUPPORT_CXX20              \
    ( __cplusplus > 201703L )


namespace kvs
{

class Compiler
{
public:
/*==========================================================================*/
/**
 *  Get compiler name.
 *  @return compiler name
 */
/*==========================================================================*/
    static const char* Name()
    {
        return KVS_COMPILER_NAME;
    }

/*==========================================================================*/
/**
 *  Get compiler version.
 *  @return compiler version
 */
/*==========================================================================*/
    static const char* Version()
    {
        return KVS_COMPILER_VERSION;
    }

private:
    Compiler();
};

} // end of namespace kvs
