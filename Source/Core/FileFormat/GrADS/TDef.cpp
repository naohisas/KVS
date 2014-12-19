/*****************************************************************************/
/**
 *  @file   TDef.cpp
 *  @author Naohisa Sakamoto
 */
/*----------------------------------------------------------------------------
 *
 *  Copyright (c) Visualization Laboratory, Kyoto University.
 *  All rights reserved.
 *  See http://www.viz.media.kyoto-u.ac.jp/kvs/copyright/ for details.
 *
 *  $Id: TDef.cpp 1346 2012-11-08 11:57:55Z s.yamada0808@gmail.com $
 */
/*****************************************************************************/
#include "TDef.h"
#include <kvs/Tokenizer>
#include <string>
#include <iostream>
#include <cstdlib>


namespace
{

int Month( const std::string month )
{
    if ( month == "jan" || month == "Jan" || month == "JAN" ) return 1;
    if ( month == "feb" || month == "Feb" || month == "FEB" ) return 2;
    if ( month == "mar" || month == "Mar" || month == "MAR" ) return 3;
    if ( month == "apr" || month == "Apr" || month == "APR" ) return 4;
    if ( month == "may" || month == "May" || month == "MAY" ) return 5;
    if ( month == "jun" || month == "Jun" || month == "JUN" ) return 6;
    if ( month == "jul" || month == "Jul" || month == "JUL" ) return 7;
    if ( month == "aug" || month == "Aug" || month == "AUG" ) return 8;
    if ( month == "sep" || month == "Sep" || month == "SEP" ) return 9;
    if ( month == "oct" || month == "Oct" || month == "OCT" ) return 10;
    if ( month == "nov" || month == "Nov" || month == "NOV" ) return 11;
    if ( month == "dec" || month == "Dec" || month == "DEC" ) return 12;
    return -1;
}

int Unit( const std::string unit )
{
    if ( unit == "mn" ) return 0;
    if ( unit == "hr" ) return 1;
    if ( unit == "dy" ) return 2;
    if ( unit == "mo" ) return 3;
    if ( unit == "yr" ) return 4;
    return -1;
}

const int DaysInMonth[] = {
    31, 28, 31, 30, 31, 30, 31, 31, 30, 31, 30, 31
};

}

namespace kvs
{

namespace grads
{

/*===========================================================================*/
/**
 *  @brief  Reads TDEF entry.
 *  @param  line [in] read line
 *  @param  ifs [in] input file stream
 *  @return true, if the reading process is done successfully
 */
/*===========================================================================*/
bool TDef::read( std::string line, std::ifstream& )
{
    kvs::Tokenizer t( line, " \t\n" );
    t.token(); // TDEF

    // xnum
    if ( t.isLast() ) return false;
    this->num = static_cast<size_t>( atoi( t.token().c_str() ) );

    // mapping
    if ( t.isLast() ) return false;
    t.token(); // LINEAR

    // start
    if ( t.isLast() ) return false;
    std::string start = t.token();
    {
        // hour
        this->start.hour = 0;
        if ( start.find( ':', 0 ) != std::string::npos )
        {
            std::string hour = start.substr( 0, start.find_first_of(':') );
            start.erase( 0, start.find_first_of(':') + 1 );
        }

        // minute
        this->start.minute = 0;
        if ( start.find( 'Z', 0 ) != std::string::npos )
        {
            std::string minute = start.substr( 0, start.find_first_of('Z') );
            this->start.minute = atoi( minute.c_str() );
            start.erase( 0, start.find_first_of('Z') + 1 );
        }

        // year
        std::string year = start.substr( start.length() - 4 );
        this->start.year = atoi( year.c_str() );
        start.erase( start.length() - 4 );

        // month
        std::string month = start.substr( start.length() - 3 );
        this->start.month = ::Month( month );
        start.erase( start.length() - 3 );

        // day
        this->start.day = 1;
        if ( start.length() > 0 ) this->start.day = atoi( start.c_str() );
    }

    // Increment
    if ( t.isLast() ) return false;
    std::string increment = t.token();
    {
        // unit
        std::string unit = increment.substr( increment.length() - 2 );

        this->increment.unit = ::Unit( unit );
        increment.erase( increment.length() - 2 );

        // day
        this->increment.value = atoi( increment.c_str() );
    }

    return true;
}

/*===========================================================================*/
/**
 *  @brief  '++' operator.
 */
/*===========================================================================*/
TDef& TDef::operator ++ ()
{
    const int value = this->increment.value;
    switch ( this->increment.unit )
    {
    case 0: { this->start.minute += value; break; }
    case 1: { this->start.hour += value; break; }
    case 2: { this->start.day += value; break; }
    case 3: { this->start.month += value; break; }
    case 4: { this->start.year += value; break; }
    default: break;
    }

    if ( this->start.minute >= 60 )
    {
        this->start.minute -= 60;
        this->start.hour++;
    }

    if ( this->start.hour >= 24 )
    {
        this->start.hour -= 24;
        this->start.day++;
    }

    // Check leap year.
    const int month = this->start.month;
    const int year = this->start.year;
    int ndays = ::DaysInMonth[ month - 1 ];
    if ( ( !( year % 4 ) && ( year % 100 || !( year % 400 ) ) ) && month == 2 ) ndays++;

    if ( this->start.day >= ndays + 1 )
    {
        this->start.day -= ndays;
        this->start.month++;
    }

    if ( this->start.month >= 13 )
    {
        this->start.month -= 12;
        this->start.year++;
    }

    return *this;
}

} // end of namespace grads

} // end of namespace kvs
