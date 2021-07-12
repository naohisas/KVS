/*****************************************************************************/
/**
 *  @file   main.cpp
 *  @brief  Example program for KVS_BREAKPOINT macro.
 *  @author Naohisa Sakamoto
 */
/*****************************************************************************/
#include <iostream>
#include <kvs/Breakpoint>


/*===========================================================================*/
/**
 *  @brief  Main function.
 */
/*===========================================================================*/
int main( void )
{
    /* KVS_BREAKPOINT is a signal that tells the debugger to temporary suspend
     * execusion of the program here.
     */
    KVS_BREAKPOINT;

    return 0;
}
