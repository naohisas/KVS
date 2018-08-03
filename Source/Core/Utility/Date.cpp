/****************************************************************************/
/**
 *  @file Date.cpp
 */
/*----------------------------------------------------------------------------
 *
 *  Copyright (c) Visualization Laboratory, Kyoto University.
 *  All rights reserved.
 *  See http://www.viz.media.kyoto-u.ac.jp/kvs/copyright/ for details.
 *
 *  $Id: Date.cpp 1129 2012-05-02 10:51:39Z s.yamada0808@gmail.com $
 */
/****************************************************************************/
#include "Date.h"
#include <ctime>
#include <cstdlib>
#include <cstdio>
#include <kvs/Tokenizer>
#include <kvs/Math>


namespace
{

const char* MonthName[] = {
    "January",
    "February",
    "March",
    "April",
    "May",
    "June",
    "July",
    "August",
    "September",
    "October",
    "November",
    "December"
};

const char* MonthShortName[] = {
    "Jan",
    "Feb",
    "Mar",
    "Apr",
    "May",
    "Jun",
    "Jul",
    "Aug",
    "Sep",
    "Oct",
    "Nov",
    "Dec"
};

const char* DayOfWeekName[] = {
    "Sunday",
    "Monday",
    "Tuesday",
    "Wednesday",
    "Thursday",
    "Friday",
    "Saturday"
};

const char* DayOfWeekShortName[] = {
    "Sun",
    "Mon",
    "Tue",
    "Wed",
    "Thu",
    "Fri",
    "Sat"
};

const size_t DaysInMonth[] = {
    31, 28, 31, 30, 31, 30, 31, 31, 30, 31, 30, 31
};

inline void DateToJulian( const int year, const int month, const int day, long* julian )
{
    int jy = year;
    if ( year < 0 ) { jy++; }

    int jm = month;
    if ( month > 2 ) { jm++; }
    else
    {
        jy--;
        jm += 13;
    }

    // Gregorian Calendar adopted Oct. 15, 1582.
    const int IGREG = 15 + 31 * ( 10 + 12 * 1582 );

    // Test whether to change to Gregorian Calendar.
    long jul = (long)( int( 365.25 * jy ) + int( 30.6001 * jm ) + day + 1720995.0 );
    if ( day + 31 * ( month + 12 * year ) >= IGREG )
    {
        const int ja = (int)( 0.01 * jy );
        jul += 2 - ja + (int)( 0.25 * ja );
    }

    if ( julian ) *julian = jul;
}

inline void JulianToDate( const long julian, int* year, int* month, int* day )
{
    const int JGREG = 2299161;

    int ja = julian;
    // Cross-over to Gregorian Calendar produces this correction.
    if ( julian >= JGREG )
    {
        const int jalpha = (int)( ( (float)( julian - 1867216 ) - 0.25 ) / 36524.25 );
        ja += 1 + jalpha - (int)( 0.25 * jalpha );
    }
    // Make day number positive by adding integer number of Julian centuries,
    // then subtract them off at the end.
    else if ( julian < 0 )
    {
        ja += 36525 * ( 1 - julian / 36525 );
    }

    int jb = ja + 1524;
    int jc = (int)( 6680.0 + ( (float)( jb - 2439870 ) - 122.1 ) / 365.25 );
    int jd = (int)( 365 * jc + ( 0.25 * jc ) );
    int je = (int)( ( jb - jd ) / 30.6001 );

    int d = jb - jd - (int)( 30.6001 * je );

    int m = je - 1;
    if ( m > 12 ) m -= 12;

    int y = jc - 4715;
    if ( m > 2 ) --y;
    if ( y <= 0 ) --y;

    if ( year ) *year = y;
    if ( month ) *month = m;
    if ( day ) *day = d;
}

inline kvs::Date FixedDate( const int year, const int month, const int day )
{
    kvs::Date temp( year, month, 1 );
    return kvs::Date( year, month, kvs::Math::Min( day, temp.daysInMonth() ) );
}

} // end of namespace


namespace kvs
{

Date::Date()
{
    this->today();
}

Date::Date( const long julian_day ):
    m_julian_day( julian_day )
{
    ::JulianToDate( m_julian_day, &m_year, &m_month, &m_day );
}

Date::Date( const int year, const int month, const int day ):
    m_year( year ),
    m_month( month ),
    m_day( day )
{
    ::DateToJulian( m_year, m_month, m_day, &m_julian_day );
}

Date::Date( const Date& date ):
    m_year( date.m_year ),
    m_month( date.m_month ),
    m_day( date.m_day ),
    m_julian_day( date.m_julian_day )
{
}

Date::~Date()
{
}

Date& Date::operator = ( const Date& date )
{
    m_year = date.m_year;
    m_month = date.m_month;
    m_day = date.m_day;
    m_julian_day = date.m_julian_day;
    return *this;
}

Date& Date::operator += ( const int days )
{
    this->addDays( days );
    return *this;
}

Date& Date::operator -= ( const int days )
{
    this->subDays( days );
    return *this;
}

Date operator + ( const Date& date, const int days )
{
    Date ret( date ); ret += days;
    return ret;
}

Date operator - ( const Date& date, const int days )
{
    Date ret( date ); ret -= days;
    return ret;
}

int operator - ( const Date& date1, const Date& date2 )
{
    return date1.m_julian_day - date2.m_julian_day;
}

bool operator > ( const Date& date1, const Date& date2 )
{
    return date1.m_julian_day > date2.m_julian_day;
}

bool operator >= ( const Date& date1, const Date& date2 )
{
    return date1.m_julian_day >= date2.m_julian_day;
}

bool operator < ( const Date& date1, const Date& date2 )
{
    return date1.m_julian_day < date2.m_julian_day;
}

bool operator <= ( const Date& date1, const Date& date2 )
{
    return date1.m_julian_day <= date2.m_julian_day;
}

bool operator == ( const Date& date1, const Date& date2 )
{
    return date1.m_julian_day == date2.m_julian_day;
}

bool operator != ( const Date& date1, const Date& date2 )
{
    return date1.m_julian_day != date2.m_julian_day;
}

std::ostream& operator << ( std::ostream& os, const Date& date )
{
    os << date.dayOfWeekString() << " "
       << date.monthString()     << " "
       << date.day()             << " "
       << date.year();
    return os;
}

std::string Date::monthString( const bool abbreviation ) const
{
    return abbreviation ?
           std::string( ::MonthShortName[m_month-1] ) :
           std::string( ::MonthName[m_month-1] );
}

const Date& Date::today()
{
    time_t t = time( NULL );
    tm* time = localtime( &t );
    m_year = time->tm_year + 1900;
    m_month = time->tm_mon + 1;
    m_day = time->tm_mday;
    ::DateToJulian( m_year, m_month, m_day, &m_julian_day );
    return *this;
}

int Date::yearsOld() const
{
    Date today;
    if ( today <= *this ) { return 0; }

    const long t = today.julianDay();
    const long b = this->julianDay();
    return static_cast<int>( ( t - b ) / 365.2425 );
}

Date::DayOfWeek Date::dayOfWeek() const
{
    struct tm t;
    t.tm_year = m_year - 1900;
    t.tm_mon = m_month - 1;
    t.tm_mday = m_day;
    t.tm_hour = 0;
    t.tm_min = 0;
    t.tm_sec = 0;
    t.tm_isdst = -1;

    if ( mktime( &t ) == -1 ) { return Date::UnknownDay; }
    return ( Date::DayOfWeek )( t.tm_wday );
}

std::string Date::dayOfWeekString( const bool abbreviation ) const
{
    const Date::DayOfWeek day_of_week = this->dayOfWeek();
    if ( day_of_week == Date::UnknownDay ) { return "Unknown"; }

    return abbreviation ?
           std::string( ::DayOfWeekShortName[day_of_week] ) :
           std::string( ::DayOfWeekName[day_of_week] );
}

int Date::daysInMonth() const
{
    if ( ( m_month < Date::Jan ) || ( Date::Dec < m_month ) ) { return 0; }
    if ( ( Date::Feb == m_month ) && this->isLeepYear() ) { return 29; }
    return ::DaysInMonth[ m_month - 1 ];
}

int Date::daysInYear() const
{
    return this->isLeepYear() ? 366 : 365;
}

std::string Date::toString( const std::string sep ) const
{
    char y[5]; sprintf( y, "%04d", m_year );
    char m[3]; sprintf( m, "%02d", m_month );
    char d[3]; sprintf( d, "%02d", m_day );
    return std::string( y ) + sep + std::string( m ) + sep + std::string( d );
}

void Date::fromString( const std::string date, const std::string sep )
{
    if ( sep.size() == 0 )
    {
        if ( date.size() == 8 )
        {
            // date: YYYYMMDD
            const std::string y = date.substr( 0,4 );
            const std::string m = date.substr( 4,2 );
            const std::string d = date.substr( 6,2 );

            m_year = atoi( y.c_str() );
            m_month = atoi( m.c_str() );
            m_day = atoi( d.c_str() );
        }
    }
    else
    {
        // date: YYYY?MM?DD ( '?': separator )
        kvs::Tokenizer tokenizer( date, sep );
        const std::string y = tokenizer.token();
        const std::string m = tokenizer.token();
        const std::string d = tokenizer.token();

        m_year = atoi( y.c_str() );
        m_month = atoi( m.c_str() );
        m_day = atoi( d.c_str() );
    }

    ::DateToJulian( m_year, m_month, m_day, &m_julian_day );
//    m_julian_day = this->convert_to_julian_date( m_year, m_month, m_day );
}

bool Date::isLeepYear() const
{
    return !( m_year % 4 ) && ( m_year % 100 || !( m_year % 400 ) );
}

bool Date::isValid() const
{
    return m_year &&
           m_month >= Date::Jan && m_month <= Date::Dec &&
           m_day >= 1 && m_day <= this->daysInMonth();
}

void Date::addYears( const int years )
{
    if ( years == 0 ) return;

    int y = m_year;
    y += years;

    const int old_year = m_year;
    if ( ( old_year > 0 && y <= 0 ) || ( old_year < 0 && y >= 0 ) )
    {
        y += years > 0 ? +1 : -1;
    }

    *this = ::FixedDate( y, m_month, m_day );
}

void Date::addMonths( const int months )
{
    if ( months == 0 ) return;

    const int old_year = m_year;
    const bool increasing = ( months > 0 );

    int y = m_year;
    int m = m_month;
    int d = m_day;

    int nm = months;
    while ( nm != 0 )
    {
        if ( nm < 0 && nm + 12 <= 0 )
        {
            y--;
            nm += 12;
        }
        else if ( nm < 0 )
        {
            m += nm;
            nm = 0;
            if ( m <= 0 )
            {
                --y;
                m += 12;
            }
        }
        else if ( nm - 12 >= 0 )
        {
            y++;
            nm -= 12;
        }
        else if ( m == 12 )
        {
            y++;
            m = 0;
        }
        else
        {
            m += nm;
            nm = 0;
            if ( m > 12 )
            {
                ++y;
                m -= 12;
            }
        }
    }

    if ( ( old_year > 0 && y <= 0 ) || ( old_year < 0 && y >= 0 ) )
    {
        y += increasing ? +1 : -1;
    }

    if ( y == 1528 && m == 10 && d > 4 && d < 15 )
    {
        d = increasing ? 15 : 4;
    }

    *this = ::FixedDate( y, m, d );
}

void Date::addDays( const int days )
{
    if ( days == 0 ) return;

    const long jd = m_julian_day;
    if ( days >= 0 )
    {
        m_julian_day = ( jd + days >= jd ) ? jd + days : 0;
    }
    else
    {
        m_julian_day = ( jd - days < jd ) ? jd - days : 0;
    }
    ::JulianToDate( m_julian_day, &m_year, &m_month, &m_day );
}

/*
void Date::adjust_days()
{
    const Date this_month( m_year, m_month,     1 );
    const Date next_month( m_year, m_month + 1, 1 );

    const int ndays = next_month.daysInMonth() - this_month.daysInMonth();
    while ( m_day > ndays ) --m_day;
}

long Date::convert_to_julian_date( const int year, const int month, const int day ) const
{
    const int a = ( 14 - month ) / 12;
    const int m = month + 12 * a - 3;
    const int y = year + 4800 - a;

    return day + ( ( 153 * m + 2 ) / 5 ) + ( 365 * y ) + ( y / 4 ) - ( y / 100 ) + ( y / 400 ) - 32045;
}

Date Date::convert_from_julian_date( const long julian_day ) const
{
    const long a = julian_day + 32044;
    const long b = ( 4 * a + 3 ) / 146097;
    const long c = a - ( ( 146097 * b ) / 4 );
    const long d = ( 4 * c + 3 ) / 1461;
    const long e = c - ( 1461 * d / 4 );
    const long m = ( 5 * e + 2 ) / 153;

    const int day = e - ( ( 153 * m + 2 ) / 5 ) + 1;
    const int month = m + 3 - ( 12 * ( m / 10 ) );
    const int year = ( 100 * b ) + d - 4800 + ( m / 10 );

    return Date( year, month, day );
}
*/
} // end of namespace kvs
