/*****************************************************************************/
/**
 *  @file   Version.h
 *  @author Naohisa Sakamoto
 */
/*****************************************************************************/
#pragma once
#include <string>
#include <iostream>
#include <kvs/Program>
#include <kvs/Version>


namespace kvscheck
{

/*===========================================================================*/
/**
 *  @brief  KVS version checker class.
 */
/*===========================================================================*/
class Version : public kvs::Program
{
    int exec()
    {
        const auto version = kvs::Version::Name();
        std::cout << "KVS version : " << version << std::endl;
        return 0;
    }
};

} // end of namespace kvscheck
