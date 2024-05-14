/****************************************************************************/
/**
 *  @file   Compiler.h
 *  @author Naohisa Sakamoto
 */
/****************************************************************************/
#pragma once
#define KVS_COMPILER_NUMBER( x )           # x
#define KVS_COMPILER_NUMBER_TO_STRING( x ) KVS_COMPILER_NUMBER( x )


#if defined ( __GNUC__ )
#define KVS_COMPILER_GCC

/*----------------------------------------------------------------------------
 * Fujitsu C/C++ compiler
 *----------------------------------------------------------------------------*/
#if defined ( __FCC_version__ )
#define KVS_COMPILER_FCC
#define KVS_COMPILER_NAME "Fujitsu C/C++"
#define KVS_COMPILER_VERSION __FCC_version__
#define KVS_COMPILER_VERSION_MAJOR __FCC_major__
#define KVS_COMPILER_VERSION_MINOR __FCC_minor__
#define KVS_COMPILER_VERSION_PATCH __FCC_patchlevel__

#elif defined ( __FCC_VERSION )
#define KVS_COMPILER_FCC
#define KVS_COMPILER_NAME "Fujitsu C/C++"
#define KVS_COMPILER_VERSION KVS_COMPILER_NUMBER_TO_STRING( __FCC_VERSION )
#define KVS_COMPILER_VERSION_UNKNOWN
#define KVS_COMPILER_VERSION_MAJOR 0
#define KVS_COMPILER_VERSION_MINOR 0
#define KVS_COMPILER_VERSION_PATCH 0

/*----------------------------------------------------------------------------
 * GNU C/C++ compiler
 *----------------------------------------------------------------------------*/
#else
#define KVS_COMPILER_NAME "GNU C/C++"
#define KVS_COMPILER_VERSION_MAJOR __GNUC__
#define KVS_COMPILER_VERSION_MINOR __GNUC_MINOR__
#define KVS_COMPILER_VERSION_PATCH __GNUC_PATCHLEVEL__
#endif

/*----------------------------------------------------------------------------
 * Microsoft Visual C/C++ compiler
 *----------------------------------------------------------------------------*/
#elif defined ( _MSC_VER )
#define KVS_COMPILER_VC
#define KVS_COMPILER_NAME "Microsoft Visual C/C++"

// Visual Studio 6.0
#if   ( _MSC_VER == 1200 )
#define KVS_COMPILER_VERSION_MAJOR 6
#define KVS_COMPILER_VERSION_MINOR 0
#define KVS_COMPILER_VERSION_PATCH 0

// Visual Studio 2002
#elif ( _MSC_VER == 1300 )
#define KVS_COMPILER_VERSION_MAJOR 7
#define KVS_COMPILER_VERSION_MINOR 0
#define KVS_COMPILER_VERSION_PATCH 0

// Visual Studio 2003
#elif ( _MSC_VER == 1310 )
#define KVS_COMPILER_VERSION_MAJOR 7
#define KVS_COMPILER_VERSION_MINOR 1
#define KVS_COMPILER_VERSION_PATCH 0

// Visual Studio 2005
#elif ( _MSC_VER == 1400 )
#define KVS_COMPILER_VERSION_MAJOR 8
#define KVS_COMPILER_VERSION_MINOR 0
#define KVS_COMPILER_VERSION_PATCH 0

// Visual Studio 2008
#elif ( _MSC_VER == 1500 )
#define KVS_COMPILER_VERSION_MAJOR 9
#define KVS_COMPILER_VERSION_MINOR 0
#define KVS_COMPILER_VERSION_PATCH 0

// Visual Studio 2010
#elif ( _MSC_VER == 1600 )
#define KVS_COMPILER_VERSION_MAJOR 10
#define KVS_COMPILER_VERSION_MINOR 0
#define KVS_COMPILER_VERSION_PATCH 0

// Visual Studio 2012
#elif ( _MSC_VER == 1700 )
#define KVS_COMPILER_VERSION_MAJOR 11
#define KVS_COMPILER_VERSION_MINOR 0
#define KVS_COMPILER_VERSION_PATCH 0

// Visual Studio 2013
#elif ( _MSC_VER == 1800 )
#define KVS_COMPILER_VERSION_MAJOR 12
#define KVS_COMPILER_VERSION_MINOR 0
#define KVS_COMPILER_VERSION_PATCH 0

// Visual Studio 2015
#elif ( _MSC_VER == 1900 )
#define KVS_COMPILER_VERSION_MAJOR 14
#define KVS_COMPILER_VERSION_MINOR 0
#define KVS_COMPILER_VERSION_PATCH 0

// Visual Studio 2017
#elif ( _MSC_VER == 1910 )
#define KVS_COMPILER_VERSION_MAJOR 15
#define KVS_COMPILER_VERSION_MINOR 0
#define KVS_COMPILER_VERSION_PATCH 0

// Visual Studio 2017 Update 3
#elif ( _MSC_VER == 1911 )
#define KVS_COMPILER_VERSION_MAJOR 15
#define KVS_COMPILER_VERSION_MINOR 3
#define KVS_COMPILER_VERSION_PATCH 3

// Visual Studio 2017 Update 5
#elif ( _MSC_VER == 1912 )
#define KVS_COMPILER_VERSION_MAJOR 15
#define KVS_COMPILER_VERSION_MINOR 5
#define KVS_COMPILER_VERSION_PATCH 2

// Visual Studio 2017 Update 6
#elif ( _MSC_VER == 1913 )
#define KVS_COMPILER_VERSION_MAJOR 15
#define KVS_COMPILER_VERSION_MINOR 6
#define KVS_COMPILER_VERSION_PATCH 0

// Visual Studio 2017 Update 7
#elif ( _MSC_VER == 1914 )
#define KVS_COMPILER_VERSION_MAJOR 15
#define KVS_COMPILER_VERSION_MINOR 7
#define KVS_COMPILER_VERSION_PATCH 1

// Visual Studio 2017 Update 8
#elif ( _MSC_VER == 1915 )
#define KVS_COMPILER_VERSION_MAJOR 15
#define KVS_COMPILER_VERSION_MINOR 8
#define KVS_COMPILER_VERSION_PATCH 0

// Visual Studio 2017 Update 9
#elif ( _MSC_VER == 1916 )
#define KVS_COMPILER_VERSION_MAJOR 15
#define KVS_COMPILER_VERSION_MINOR 9
#define KVS_COMPILER_VERSION_PATCH 0

// Visual Studio 2019
#elif ( _MSC_VER == 1920 )
#define KVS_COMPILER_VERSION_MAJOR 16
#define KVS_COMPILER_VERSION_MINOR 0
#define KVS_COMPILER_VERSION_PATCH 0

// Visual Studio 2019 Update 1
#elif ( _MSC_VER == 1921 )
#define KVS_COMPILER_VERSION_MAJOR 16
#define KVS_COMPILER_VERSION_MINOR 1
#define KVS_COMPILER_VERSION_PATCH 2

// Visual Studio 2019 Update 2
#elif ( _MSC_VER == 1922 )
#define KVS_COMPILER_VERSION_MAJOR 16
#define KVS_COMPILER_VERSION_MINOR 2
#define KVS_COMPILER_VERSION_PATCH 3

// Visual Studio 2019 Update 3
#elif ( _MSC_VER == 1923 )
#define KVS_COMPILER_VERSION_MAJOR 16
#define KVS_COMPILER_VERSION_MINOR 3
#define KVS_COMPILER_VERSION_PATCH 2

// Visual Studio 2019 Update 4
#elif ( _MSC_VER == 1924 )
#define KVS_COMPILER_VERSION_MAJOR 16
#define KVS_COMPILER_VERSION_MINOR 4
#define KVS_COMPILER_VERSION_PATCH 0

// Visual Studio 2019 Update 5
#elif ( _MSC_VER == 1925 )
#define KVS_COMPILER_VERSION_MAJOR 16
#define KVS_COMPILER_VERSION_MINOR 5
#define KVS_COMPILER_VERSION_PATCH 1

// Visual Studio 2019 Update 6
#elif ( _MSC_VER == 1926 )
#define KVS_COMPILER_VERSION_MAJOR 16
#define KVS_COMPILER_VERSION_MINOR 6
#define KVS_COMPILER_VERSION_PATCH 2

// Visual Studio 2019 Update 7
#elif ( _MSC_VER == 1927 )
#define KVS_COMPILER_VERSION_MAJOR 16
#define KVS_COMPILER_VERSION_MINOR 7
#define KVS_COMPILER_VERSION_PATCH 0

// Visual Studio 2019 Update 8
#elif ( _MSC_VER == 1928 )
#if ( _MSC_FULL_VER < 192829333 )
#define KVS_COMPILER_VERSION_MAJOR 16
#define KVS_COMPILER_VERSION_MINOR 8
#define KVS_COMPILER_VERSION_PATCH 0
#elif ( _MSC_FULL_VER < 192829334 )
#define KVS_COMPILER_VERSION_MAJOR 16
#define KVS_COMPILER_VERSION_MINOR 8
#define KVS_COMPILER_VERSION_PATCH 1
#elif ( _MSC_FULL_VER < 192829913 )
#define KVS_COMPILER_VERSION_MAJOR 16
#define KVS_COMPILER_VERSION_MINOR 8
#define KVS_COMPILER_VERSION_PATCH 2

// Visual Studio 2019 Update 9 (_MSC_FULL_VER == 192829913)
#else
#define KVS_COMPILER_VERSION_MAJOR 16
#define KVS_COMPILER_VERSION_MINOR 9
#define KVS_COMPILER_VERSION_PATCH 2
#endif

// Visual Studio 2019 Update 10
#elif ( _MSC_VER == 1929 )
#if ( _MSC_FULL_VER < 192930133 )
#define KVS_COMPILER_VERSION_MAJOR 16
#define KVS_COMPILER_VERSION_MINOR 10
#define KVS_COMPILER_VERSION_PATCH 0

// Visual Studio 2019 Update 11 (_MSC_FULL_VER == 192930133)
#else
#define KVS_COMPILER_VERSION_MAJOR 16
#define KVS_COMPILER_VERSION_MINOR 11
#define KVS_COMPILER_VERSION_PATCH 2
#endif

// Visual Studio 2022
#elif ( _MSC_VER == 1930 )
#if ( _MSC_FULL_VER < 193030706 )
#define KVS_COMPILER_VERSION_MAJOR 17
#define KVS_COMPILER_VERSION_MINOR 0
#define KVS_COMPILER_VERSION_PATCH 1

// Visual Studio 2022 (_MSC_FULL_VER == 193030706)
#else
#define KVS_COMPILER_VERSION_MAJOR 17
#define KVS_COMPILER_VERSION_MINOR 0
#define KVS_COMPILER_VERSION_PATCH 2
#endif

#elif ( _MSC_VER == 1931 )
#define KVS_COMPILER_VERSION_MAJOR 17
#define KVS_COMPILER_VERSION_MINOR 1
#define KVS_COMPILER_VERSION_PATCH 0

#elif ( _MSC_VER == 1932 )
#define KVS_COMPILER_VERSION_MAJOR 17
#define KVS_COMPILER_VERSION_MINOR 2
#define KVS_COMPILER_VERSION_PATCH 0

#elif ( _MSC_VER == 1933 )
#define KVS_COMPILER_VERSION_MAJOR 17
#define KVS_COMPILER_VERSION_MINOR 3
#define KVS_COMPILER_VERSION_PATCH 0

#elif ( _MSC_VER == 1934 )
#define KVS_COMPILER_VERSION_MAJOR 17
#define KVS_COMPILER_VERSION_MINOR 4
#define KVS_COMPILER_VERSION_PATCH 0

#elif ( _MSC_VER == 1935 )
#define KVS_COMPILER_VERSION_MAJOR 17
#define KVS_COMPILER_VERSION_MINOR 5
#define KVS_COMPILER_VERSION_PATCH 0

#elif ( _MSC_VER == 1936 )
#define KVS_COMPILER_VERSION_MAJOR 17
#define KVS_COMPILER_VERSION_MINOR 6
#define KVS_COMPILER_VERSION_PATCH 2

#elif ( _MSC_VER == 1937 )
#define KVS_COMPILER_VERSION_MAJOR 17
#define KVS_COMPILER_VERSION_MINOR 7
#define KVS_COMPILER_VERSION_PATCH 0

#elif ( _MSC_VER == 1938 )
#define KVS_COMPILER_VERSION_MAJOR 17
#define KVS_COMPILER_VERSION_MINOR 8
#define KVS_COMPILER_VERSION_PATCH 3

#elif ( _MSC_VER == 1939 )
#define KVS_COMPILER_VERSION_MAJOR 17
#define KVS_COMPILER_VERSION_MINOR 9
#define KVS_COMPILER_VERSION_PATCH 2

#else
#define KVS_COMPILER_VERSION_UNKNOWN
#define KVS_COMPILER_VERSION_MAJOR 0
#define KVS_COMPILER_VERSION_MINOR 0
#define KVS_COMPILER_VERSION_PATCH 0
#endif


/*----------------------------------------------------------------------------
 * Unknown compiler
 *----------------------------------------------------------------------------*/
#else
#define KVS_COMPILER_UNKNOWN
#define KVS_COMPILER_NAME "Unknown"
#define KVS_COMPILER_VERSION_UNKNOWN
#define KVS_COMPILER_VERSION_MAJOR 0
#define KVS_COMPILER_VERSION_MINOR 0
#define KVS_COMPILER_VERSION_PATCH 0
#endif


/*----------------------------------------------------------------------------
 * Compiler version string
 *----------------------------------------------------------------------------*/
#if !defined( KVS_COMPILER_VERSION )
#define KVS_COMPILER_VERSION                                            \
    KVS_COMPILER_NUMBER_TO_STRING( KVS_COMPILER_VERSION_MAJOR ) "."     \
    KVS_COMPILER_NUMBER_TO_STRING( KVS_COMPILER_VERSION_MINOR ) "."     \
    KVS_COMPILER_NUMBER_TO_STRING( KVS_COMPILER_VERSION_PATCH )
#endif


/*----------------------------------------------------------------------------
 * Compiler version comparison functions
 *----------------------------------------------------------------------------*/
#if defined( KVS_COMPILER_VERSION_UNKNOWN )
#define KVS_COMPILER_VERSION_IS( major, minor )
#define KVS_COMPILER_VERSION_LESS_THAN( major, minor )
#define KVS_COMPILER_VERSION_LESS_OR_EQUAL( major, minor )
#define KVS_COMPILER_VERSION_GREATER_THAN( major, minor )
#define KVS_COMPILER_VERSION_GREATER_OR_EQUAL( major, minor )
#else
#define KVS_COMPILER_VERSION_IS( major, minor )                         \
    ( KVS_COMPILER_VERSION_MAJOR == ( major ) &&                        \
      KVS_COMPILER_VERSION_MINOR == ( minor ) )
#define KVS_COMPILER_VERSION_LESS_THAN( major, minor )                  \
    ( KVS_COMPILER_VERSION_MAJOR < ( major ) ||                         \
      ( KVS_COMPILER_VERSION_MAJOR == ( major ) &&                      \
        KVS_COMPILER_VERSION_MINOR < ( minor ) ) )
#define KVS_COMPILER_VERSION_LESS_OR_EQUAL( major, minor )              \
    ( KVS_COMPILER_VERSION_MAJOR < ( major ) ||                         \
      ( KVS_COMPILER_VERSION_MAJOR == ( major ) &&                      \
        KVS_COMPILER_VERSION_MINOR <= ( minor ) ) )
#define KVS_COMPILER_VERSION_GREATER_THAN( major, minor )               \
    ( KVS_COMPILER_VERSION_MAJOR > ( major ) ||                         \
      ( KVS_COMPILER_VERSION_MAJOR == ( major ) &&                      \
        KVS_COMPILER_VERSION_MINOR > ( minor ) ) )
#define KVS_COMPILER_VERSION_GREATER_OR_EQUAL( major, minor )           \
    ( KVS_COMPILER_VERSION_MAJOR > ( major ) ||                         \
      ( KVS_COMPILER_VERSION_MAJOR == ( major ) &&                      \
        KVS_COMPILER_VERSION_MINOR >= ( minor ) ) )
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
