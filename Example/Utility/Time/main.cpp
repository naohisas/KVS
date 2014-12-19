/*****************************************************************************/
/**
 *  @file   main.cpp
 *  @brief  Example program for kvs::Time class.
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
#include <iomanip>
#include <kvs/Time>


/*===========================================================================*/
/**
 *  @brief  Main function.
 */
/*===========================================================================*/
int main( void )
{
    const kvs::Time now;
    std::cout << "It is " << now << " now." << std::endl;
    std::cout << "  hour: " << now.hour() << std::endl;
    std::cout << "  minute: " << now.minute() << std::endl;
    std::cout << "  second: " << now.second() << std::endl;

    return 0;
}
