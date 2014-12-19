/****************************************************************************/
/**
 *  @file Time.cpp
 */
/*----------------------------------------------------------------------------
 *
 *  Copyright (c) Visualization Laboratory, Kyoto University.
 *  All rights reserved.
 *  See http://www.viz.media.kyoto-u.ac.jp/kvs/copyright/ for details.
 *
 *  $Id: Time.cpp 1129 2012-05-02 10:51:39Z s.yamada0808@gmail.com $
 */
/****************************************************************************/
#include "Time.h"
#include <iostream>
#include <string>
#include <ctime>
#include <cstdlib>
#include <cstdio>
#include <kvs/Tokenizer>


namespace kvs
{

Time::Time()
{
    this->now();
}

Time::Time( const int hour, const int minute, const int second ):
    m_hour( hour ),
    m_minute( minute ),
    m_second( second )
{
    m_total_seconds = this->convert_to_seconds( m_hour, m_minute, m_second );
}

Time::~Time()
{
}

Time& Time::operator = ( const Time& time )
{
    m_hour          = time.m_hour;
    m_minute        = time.m_minute;
    m_second        = time.m_second;
    m_total_seconds = time.m_total_seconds;

    return *this;
}

Time& Time::operator += ( const int seconds )
{
    this->addSeconds( seconds );

    return *this;
}

Time& Time::operator -= ( const int seconds )
{
    this->subtractSeconds( seconds );

    return *this;
}

Time operator + ( const Time& time, const int seconds )
{
    Time ret( time ); ret += seconds;

    return ret;
}

Time operator - ( const Time& time, const int seconds )
{
    Time ret( time ); ret -= seconds;

    return ret;
}

int operator - ( const Time& time1, const Time& time2 )
{
    return time1.m_total_seconds - time2.m_total_seconds;
}

bool operator > ( const Time& time1, const Time& time2 )
{
    return time1.m_total_seconds > time2.m_total_seconds;
}

bool operator >= ( const Time& time1, const Time& time2 )
{
    return time1.m_total_seconds >= time2.m_total_seconds;
}

bool operator < ( const Time& time1, const Time& time2 )
{
    return time1.m_total_seconds < time2.m_total_seconds;
}

bool operator <= ( const Time& time1, const Time& time2 )
{
    return time1.m_total_seconds <= time2.m_total_seconds;
}

bool operator == ( const Time& time1, const Time& time2 )
{
    return time1.m_total_seconds == time2.m_total_seconds;
}

bool operator != ( const Time& time1, const Time& time2 )
{
    return time1.m_total_seconds != time2.m_total_seconds;
}

std::ostream& operator << ( std::ostream& os, const Time& time )
{
    os << time.hour() << ":"
       << time.minute() << ":"
       << time.second();

    return os;
}

int Time::hour() const
{
    return m_hour;
}

int Time::minute() const
{
    return m_minute;
}

int Time::second() const
{
    return m_second;
}

long Time::totalSeconds() const
{
    return m_total_seconds;
}

const Time& Time::now()
{
    time_t t = time( NULL );
    tm* time = localtime( &t );

    m_hour   = time->tm_hour;
    m_minute = time->tm_min;
    m_second = time->tm_sec;
    m_total_seconds = this->convert_to_seconds( m_hour, m_minute, m_second );

    return *this;
}

std::string Time::toString( const std::string sep ) const
{
    char h[3]; sprintf( h, "%02d", m_hour );
    char m[3]; sprintf( m, "%02d", m_minute );
    char s[3]; sprintf( s, "%02d", m_second );

    return std::string( h ) + sep + std::string( m ) + sep + std::string( s );
}

void Time::fromString( const std::string time, const std::string sep )
{
    if ( sep.size() == 0 )
    {
        if ( time.size() == 6 )
        {
            // time: HHMMSS
            const std::string h = time.substr( 0,2 );
            const std::string m = time.substr( 2,2 );
            const std::string s = time.substr( 4,2 );

            m_hour   = atoi( h.c_str() );
            m_minute = atoi( m.c_str() );
            m_second = atoi( s.c_str() );
        }
    }
    else
    {
        // time: HH?MM?SS ( '?': separator )
        kvs::Tokenizer tokenizer( time, sep );
        const std::string h = tokenizer.token();
        const std::string m = tokenizer.token();
        const std::string s = tokenizer.token();

        m_hour   = atoi( h.c_str() );
        m_minute = atoi( m.c_str() );
        m_second = atoi( s.c_str() );
    }

    m_total_seconds = this->convert_to_seconds( m_hour, m_minute, m_second );
}

void Time::addHours( const int hour )
{
    m_hour += hour;
    m_total_seconds = this->convert_to_seconds( m_hour, m_minute, m_second );
}

void Time::subtractHours( const int hour )
{
    m_hour -= hour;
    m_total_seconds = this->convert_to_seconds( m_hour, m_minute, m_second );
}

void Time::addMinutes( const int minute )
{
    m_minute += minute;
    if ( m_minute > 60 )
    {
        while ( m_minute > 60 )
        {
            m_minute -= 60;
            m_hour   += 1;
        }
    }

    m_total_seconds = this->convert_to_seconds( m_hour, m_minute, m_second );
}

void Time::subtractMinutes( const int minute )
{
    m_minute -= minute;

    if ( minute > 60 )
    {
        int t = minute / 60;
        m_hour   -= t;
        m_minute += t * 60;
    }

    m_total_seconds = this->convert_to_seconds( m_hour, m_minute, m_second );
}

void Time::addSeconds( const int second )
{
    m_total_seconds += second;

    *this = this->convert_from_seconds( m_total_seconds );
}

void Time::subtractSeconds( const int second )
{
    m_total_seconds -= second;

    *this = this->convert_from_seconds( m_total_seconds );
}

long Time::convert_to_seconds( const int hour, const int minute, const int second )
{
    return hour * 3600 + minute * 60 + second;
}

Time Time::convert_from_seconds( const int seconds )
{
    const int hour   = seconds / 3600;
    const int minute = ( seconds - hour * 3600 ) / 60;
    const int second = seconds - hour * 3600 - minute * 60;

    return Time( hour, minute, second );
}

} // end of namespace kvs
