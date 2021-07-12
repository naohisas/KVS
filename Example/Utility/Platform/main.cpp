/*****************************************************************************/
/**
 *  @file   main.cpp
 *  @brief  Example program for kvs::Platform class.
 *  @author Naohisa Sakamoto
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
