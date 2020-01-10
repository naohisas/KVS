/*****************************************************************************/
/**
 *  @file   PlatformChecker.h
 *  @author Naohisa Sakamoto
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
#pragma once
#include <string>
#include <iostream>
#include <kvs/Program>
#include <kvs/Platform>
#include <kvs/SystemInformation>
#include <kvs/Endian>


namespace kvscheck
{

class Platform : public kvs::Program
{
public:
    int exec( int argc, char** argv );
};

inline int Platform::exec( int argc, char** argv )
{
    const std::string name = kvs::Platform::Name();
    const std::string cpu = kvs::Platform::CPUName();
    const std::string byte_order = kvs::Endian::IsBig() ? "Big endian" : "Little endian";
    const size_t ncores = kvs::SystemInformation::NumberOfProcessors();

#if defined( KVS_PLATFORM_CPU_64 )
    const size_t nbits = 64;
#elif defined( KVS_PLATFORM_CPU_32 )
    const size_t nbits = 32;
#else
    const size_t nbits = 0;
#endif

    std::cout << "Platform   : " << name << std::endl;
    std::cout << "CPU        : " << cpu << " (" << nbits << " bits)" << std::endl;
    std::cout << "# of cores : " << ncores << std::endl;
    std::cout << "Byte-order : " << byte_order << std::endl;

    return 0;
}


/*==========================================================================*/
/**
 *  Platform checker class.
 */
/*==========================================================================*/
class PlatformChecker
{
private:
    std::string m_name; ///< platform name
    std::string m_cpu; ///< CPU name

public:
    PlatformChecker();
    const std::string& name() const { return m_name; }
    const std::string& cpu() const { return m_cpu; }
    friend std::ostream& operator << ( std::ostream& os, const PlatformChecker& checker );
};

} // end of namespace kvscheck
