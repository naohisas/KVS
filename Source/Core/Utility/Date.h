/****************************************************************************/
/**
 *  @file Date.h
 */
/*----------------------------------------------------------------------------
 *
 *  Copyright (c) Visualization Laboratory, Kyoto University.
 *  All rights reserved.
 *  See http://www.viz.media.kyoto-u.ac.jp/kvs/copyright/ for details.
 *
 *  $Id: Date.h 1707 2014-01-27 07:37:04Z naohisa.sakamoto@gmail.com $
 */
/****************************************************************************/
#ifndef KVS__DATE_H_INCLUDE
#define KVS__DATE_H_INCLUDE

#include <iostream>
#include <string>


namespace kvs
{

/*===========================================================================*/
/**
 *  @brief  Date class.
 */
/*===========================================================================*/
class Date
{
public:

    enum DayOfWeek
    {
        UnknownDay   = -1,
        Sun          =  0,
        Mon          =  1,
        Tue          =  2,
        Wed          =  3,
        Thu          =  4,
        Fri          =  5,
        Sat          =  6,
        NumberOfDays =  7
    };

    enum MonthOfYear
    {
        UnknownMonth   = -1,
        Jan            =  1,
        Feb            =  2,
        Mar            =  3,
        Apr            =  4,
        May            =  5,
        Jun            =  6,
        Jul            =  7,
        Aug            =  8,
        Sep            =  9,
        Oct            = 10,
        Nov            = 11,
        Dec            = 12,
        NumberOfMonths = 12
    };

protected:

    int  m_year;       ///< year
    int  m_month;      ///< month
    int  m_day;        ///< day
    long m_julian_day; ///< julian day

public:

    Date();

    Date( const long julian_day );

    Date( const int year, const int month, const int day );

    Date( const Date& date );

    virtual ~Date();

public:

    Date& operator = ( const Date& date );

    Date& operator += ( const int days );

    Date& operator -= ( const int days );

    friend Date operator + ( const Date& date, const int days );

    friend Date operator - ( const Date& date, const int days );

    friend int operator - ( const Date& date1, const Date& date2 );

    friend bool operator > ( const Date& date1, const Date& date2 );

    friend bool operator >= ( const Date& date1, const Date& date2 );

    friend bool operator < ( const Date& date1, const Date& date2 );

    friend bool operator <= ( const Date& date1, const Date& date2 );

    friend bool operator == ( const Date& date1, const Date& date2 );

    friend bool operator != ( const Date& date1, const Date& date2 );

    friend std::ostream& operator << ( std::ostream& os, const Date& date );

public:

    int year() const;

    int month() const;

    int day() const;

    long julianDay() const;

    std::string monthString( const bool abbrevition = true ) const;

public:

    const Date& today();

    int yearsOld() const;

public:

    DayOfWeek dayOfWeek() const;

    std::string dayOfWeekString( const bool abbr = true ) const;

    int daysInMonth() const;

    std::string toString( const std::string sep = "" ) const;

    void fromString( const std::string date, const std::string sep = "" );

public:

    bool isLeepYear() const;

    bool isValid() const;

public:

    void addYears( const int year );

    void subtractYears( const int year );

    void addMonths( const int month );

    void subtractMonths( const int month );

    void addDays( const int days );

    void subtractDays( const int days );

protected:

    void adjust_days();

    long convert_to_julian_date( const int year, const int month, const int day ) const;

    Date convert_from_julian_date( const long julian_day ) const;
};

} // end of namespace kvs

#endif // KVS__DATE_H_INCLUDE
