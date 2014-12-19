/*****************************************************************************/
/**
 *  @file   main.cpp
 *  @brief  Example program for kvs::Platform class.
 *  @author Naohisa Sakamoto
 */
/*----------------------------------------------------------------------------
 *
 *  Copyright (c) Visualization Laboratory, Kyoto University.
 *  All rights reserved.
 *  See http://www.viz.media.kyoto-u.ac.jp/kvs/copyright/ for details.
 *
 *  $Id: main.cpp 602 2010-08-19 02:43:34Z naohisa.sakamoto $
 */
/*****************************************************************************/
#include <iostream>
#include <kvs/Platform>


/*===========================================================================*/
/**
 *  @brief  Main function.
 */
/*===========================================================================*/
int main( void )
{
    // Output the information of the platform.
    const char* name = kvs::Platform::Name();
    const char* cpu_name = kvs::Platform::CPUName();
    std::cout << "Platform name: " << name << std::endl;
    std::cout << "CPU name: " << cpu_name << std::endl;

    return 0;
}
