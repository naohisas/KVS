/*****************************************************************************/
/**
 *  @file   main.cpp
 *  @brief  Example program for KVS_ASSERT macro.
 *  @author Naohisa Sakamoto
 */
/*****************************************************************************/
#include <iostream>
#include <kvs/Assert>


/*===========================================================================*/
/**
 *  @brief  Main function.
 */
/*===========================================================================*/
int main( void )
{
    /* KVS_ASSERT is enabled only when the program is compiled with debug mode
     * (kvsmake DEBUG=1). In this case, if the expression is true, execution
     * contibues without any interruption. However, if false, the program halt
     * here with the breakpoint for the debugger. On the other hand, when the
     * program is compiled normally (without 'DEBUG=1'), KVS_ASSERT is ignored.
     */
    int i;
    i = 2;
    KVS_ASSERT( i == 1 );

    std::cout << "Program terminated normally." << std::endl;

    return 0;
}
