/*****************************************************************************/
/**
 *  @file   main.cpp
 *  @brief  Example program for kvs::Version class.
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
#include <string>
#include <kvs/Version>


/*===========================================================================*/
/**
 *  @brief  Main function.
 */
/*===========================================================================*/
int main( void )
{
    // Output the information of KVS version.
    const size_t number = kvs::Version::Number();
    const size_t major_number = kvs::Version::MajorNumber();
    const size_t minor_number = kvs::Version::MinorNumber();
    const size_t patch_number = kvs::Version::PatchNumber();
    const char* version_name = kvs::Version::Name();
    std::cout << "Number: " << number << std::endl;
    std::cout << "Major number: " << major_number << std::endl;
    std::cout << "Minor number: " << minor_number << std::endl;
    std::cout << "Patch number: " << patch_number << std::endl;
    std::cout << "Version name: " << version_name << std::endl;

    return 0;
}
