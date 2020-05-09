/****************************************************************************/
/**
 *  @file Version.h
 */
/*----------------------------------------------------------------------------
 *
 *  Copyright (c) Visualization Laboratory, Kyoto University.
 *  All rights reserved.
 *  See http://www.viz.media.kyoto-u.ac.jp/kvs/copyright/ for details.
 *
 *  $Id: Version.h 1819 2014-10-10 10:30:21Z naohisa.sakamoto@gmail.com $
 */
/****************************************************************************/
#ifndef KVS__VERSION_H_INCLUDE
#define KVS__VERSION_H_INCLUDE

// KVS version: <major_version>.<minor_version>.<patch_version>
#define KVS_VERSION_MAJOR  2
#define KVS_VERSION_MINOR  9
#define KVS_VERSION_PATCH  0

#define KVS_VERSION_NUMBER( x )           # x
#define KVS_VERSION_NUMBER_TO_STRING( x ) KVS_VERSION_NUMBER( x )
#define KVS_VERSION                                       \
    KVS_VERSION_NUMBER_TO_STRING( KVS_VERSION_MAJOR ) "." \
    KVS_VERSION_NUMBER_TO_STRING( KVS_VERSION_MINOR ) "." \
    KVS_VERSION_NUMBER_TO_STRING( KVS_VERSION_PATCH )

#if KVS_ENABLE_DEPRECATED
#include <cstdio>
#endif

namespace kvs
{

class Version
{
public:
/*==========================================================================*/
/**
 *  Get version number.
 *  @return version number
 */
/*==========================================================================*/
    static int Number()
    {
        const int version_number =
            100 * KVS_VERSION_MAJOR +
            10  * KVS_VERSION_MINOR +
            1   * KVS_VERSION_PATCH;

        return version_number;
    }

/*==========================================================================*/
/**
 *  Get major version number.
 *  @return major version number
 */
/*==========================================================================*/
    static int MajorNumber()
    {
        return KVS_VERSION_MAJOR;
    }

/*==========================================================================*/
/**
 *  Get minor version number.
 *  @return minor version number
 */
/*==========================================================================*/
    static int MinorNumber()
    {
        return KVS_VERSION_MINOR;
    }

/*==========================================================================*/
/**
 *  Get patch version number.
 *  @return patch version number
 */
/*==========================================================================*/
    static int PatchNumber()
    {
        return KVS_VERSION_PATCH;
    }

/*==========================================================================*/
/**
 *  Get version name.
 *  @return version name
 */
/*==========================================================================*/
    static const char* Name()
    {
        return KVS_VERSION;
    }

private:
    Version();
};

} // end of namespace kvs

#endif // KVS__VERSION_H_INCLUDE
