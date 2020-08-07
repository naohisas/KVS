/*****************************************************************************/
/**
 *  @file   SystemInformation.h
 *  @author Naohisa Sakamoto
 */
/*****************************************************************************/
#pragma once
#include <cstdio>


namespace kvs
{

/*===========================================================================*/
/**
 *  @brief  System information class.
 */
/*===========================================================================*/
class SystemInformation
{
public:
    static size_t NumberOfProcessors();
    static size_t TotalMemorySize();
    static size_t FreeMemorySize();

private:
    SystemInformation();
};

} // end of namespace kvs
