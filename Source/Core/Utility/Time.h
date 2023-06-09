/****************************************************************************/
/**
 *  @file   Time.h
 *  @author Naohisa Sakamoto
 */
/****************************************************************************/
#pragma once
#include <iostream>
#include <string>


namespace kvs
{

/*===========================================================================*/
/**
 *  @brief  Time class.
 */
/*===========================================================================*/
class Time
{
private:
    int m_hour = 0; ///< hour
    int m_minute = 0; ///< minute
    int m_second = 0; ///< second
    long m_total_seconds = 0; ///< total seconds

public:
    Time();
    Time( const int hour, const int minute, const int second );
    virtual ~Time() = default;

    Time& operator = ( const Time& time );
    Time& operator += ( const int seconds );
    Time& operator -= ( const int seconds );
    friend Time operator + ( const Time& time, const int seconds );
    friend Time operator - ( const Time& time, const int seconds );
    friend int operator - ( const Time& time1, const Time& time2 );
    friend bool operator > ( const Time& time1, const Time& time2 );
    friend bool operator >= ( const Time& time1, const Time& time2 );
    friend bool operator < ( const Time& time1, const Time& time2 );
    friend bool operator <= ( const Time& time1, const Time& time2 );
    friend bool operator == ( const Time& time1, const Time& time2 );
    friend bool operator != ( const Time& time1, const Time& time2 );
    friend std::ostream& operator << ( std::ostream& os, const Time& time );

    int hour() const;
    int minute() const;
    int second() const;
    long totalSeconds() const;

    const Time& now();
    std::string toString( const std::string sep = ":" ) const;
    void fromString( const std::string time, const std::string sep = ":" );

    void addHours( const int hour );
    void subtractHours( const int hour );
    void addMinutes( const int minute );
    void subtractMinutes( const int minute );
    void addSeconds( const int second );
    void subtractSeconds( const int second );

private:
    long convert_to_seconds( const int hour, const int minute, const int second );
    Time convert_from_seconds( const int seconds );
};

} // end of namespace kvs
