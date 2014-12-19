/*****************************************************************************/
/**
 *  @file   main.cpp
 *  @brief  Exmaple program for kvs::Date class.
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
#include <kvs/Date>


/*===========================================================================*/
/**
 *  @brief  Main function.
 */
/*===========================================================================*/
int main( void )
{
    const kvs::Date today;
    std::cout << "Today is " << today << "." << std::endl;
    std::cout << "  year: " << today.year() << std::endl;
    std::cout << "  month: ";
    std::cout << today.month() << " ";
    std::cout << today.monthString() << " ";
    std::cout << today.monthString(false) << std::endl;
    std::cout << "  day: " << today.day() << std::endl;
    std::cout << "  day of week : ";
    std::cout << today.dayOfWeekString() << " ";
    std::cout << today.dayOfWeekString(false) << std::endl;
    std::cout << "  leep year: ";
    std::cout << std::setiosflags(std::ios::boolalpha) << today.isLeepYear() << std::endl;

    const kvs::Date birthday( 1975, 8, 30 );
    std::cout << "Birthday is " << birthday << ". ";
    std::cout << "(" << birthday.yearsOld() << " years old)" << std::endl;
    std::cout << "  year: " << birthday.year() << std::endl;
    std::cout << "  month: ";
    std::cout << birthday.month() << " ";
    std::cout << birthday.monthString() << " ";
    std::cout << birthday.monthString(false) << std::endl;
    std::cout << "  day: " << birthday.day() << std::endl;
    std::cout << "  day of week: ";
    std::cout << birthday.dayOfWeekString() << " ";
    std::cout << birthday.dayOfWeekString(false) << std::endl;
    std::cout << "  leep year: ";
    std::cout << std::setiosflags(std::ios::boolalpha) << birthday.isLeepYear() << std::endl;

    const int days = today - birthday;
    std::cout << "Today - Birthday = " << days << " days" << std::endl;

    return 0;
}
