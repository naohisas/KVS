/*****************************************************************************/
/**
 *  @file   main.cpp
 *  @brief  Example program for kvs::Compiler class.
 *  @author Naohisa Sakamoto
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
