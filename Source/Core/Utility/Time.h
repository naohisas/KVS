/****************************************************************************/
/**
 *  @file Time.h
 */
/*----------------------------------------------------------------------------
 *
 *  Copyright (c) Visualization Laboratory, Kyoto University.
 *  All rights reserved.
 *  See http://www.viz.media.kyoto-u.ac.jp/kvs/copyright/ for details.
 *
 *  $Id: Time.h 1707 2014-01-27 07:37:04Z naohisa.sakamoto@gmail.com $
 */
/****************************************************************************/
#ifndef KVS__TIME_H_INCLUDE
#define KVS__TIME_H_INCLUDE

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
protected:

    int  m_hour;          ///< hour
    int  m_minute;        ///< minute
    int  m_second;        ///< second
    long m_total_seconds; ///< total seconds

public:

    Time();

    Time( const int hour, const int minute, const int second );

    virtual ~Time();

public:

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

public:

    int hour() const;

    int minute() const;

    int second() const;

    long totalSeconds() const;

public:

    const Time& now();

public:

    std::string toString( const std::string sep = ":" ) const;

    void fromString( const std::string time, const std::string sep = ":" );

public:

    void addHours( const int hour );

    void subtractHours( const int hour );

    void addMinutes( const int minute );

    void subtractMinutes( const int minute );

    void addSeconds( const int second );

    void subtractSeconds( const int second );

protected:

    long convert_to_seconds( const int hour, const int minute, const int second );

    Time convert_from_seconds( const int seconds );
};

} // end of namespace kvs

#endif // KVS__TIME_H_INCLUDE
