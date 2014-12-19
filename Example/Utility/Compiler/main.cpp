/*****************************************************************************/
/**
 *  @file   main.cpp
 *  @brief  Example program for kvs::Compiler class.
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
#include <kvs/Compiler>


/*===========================================================================*/
/**
 *  @brief  Main function.
 */
/*===========================================================================*/
int main( void )
{
    // Output the information of compiler name and version.
    const char* compiler_name = kvs::Compiler::Name();
    const char* compiler_version = kvs::Compiler::Version();
    std::cout << "Compiler name : " << compiler_name << std::endl;
    std::cout << "Compiler version : " << compiler_version << std::endl;

    return 0;
}
