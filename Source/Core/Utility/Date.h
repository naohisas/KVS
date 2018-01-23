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
#include <kvs/Deprecated>


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

private:

    int m_year; ///< year
    int m_month; ///< month
    int m_day; ///< day
    long m_julian_day; ///< julian day

public:

    Date();
    Date( const long julian_day );
    Date( const int year, const int month, const int day );
    Date( const Date& date );
    virtual ~Date();

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

    int year() const { return m_year; }
    int month() const { return m_month; }
    int day() const { return m_day; }
    long julianDay() const { return m_julian_day; }

    std::string monthString( const bool abbrevition = true ) const;
    const Date& today();
    int yearsOld() const;

    DayOfWeek dayOfWeek() const;
    std::string dayOfWeekString( const bool abbr = true ) const;
    int daysInMonth() const;
    int daysInYear() const;
    std::string toString( const std::string sep = "" ) const;
    void fromString( const std::string date, const std::string sep = "" );

    bool isLeepYear() const;
    bool isValid() const;

    void addYears( const int years );
    void addMonths( const int months );
    void addDays( const int days );
    void subYears( const int years ) { this->addYears( -1 * years ); }
    void subMonths( const int months ) { this->addMonths( -1 * months ); }
    void subDays( const int days ) { this->addDays( -1 * days ); }

private:

//    void adjust_days();
//    long convert_to_julian_date( const int year, const int month, const int day ) const;
//    Date convert_from_julian_date( const long julian_day ) const;

public:
    KVS_DEPRECATED( void subtractYears( const int years ) ) { this->subYears( years ); }
    KVS_DEPRECATED( void subtractMonths( const int months ) ) { this->subMonths( months ); }
    KVS_DEPRECATED( void subtractDays( const int days ) ) { this->subDays( days ); }
};

} // end of namespace kvs

#endif // KVS__DATE_H_INCLUDE
