/****************************************************************************/
/**
 *  @file   Date.h
 *  @author Naohisa Sakamoto
 */
/****************************************************************************/
#pragma once
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
    int m_year = 0; ///< year
    int m_month = 0; ///< month
    int m_day = 0; ///< day
    long m_julian_day = 0; ///< julian day

public:
    Date();
    Date( const long julian_day );
    Date( const int year, const int month, const int day );
    Date( const Date& date );
    virtual ~Date() = default;

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
};

} // end of namespace kvs
