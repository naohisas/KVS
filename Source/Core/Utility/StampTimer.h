/*****************************************************************************/
/**
 *  @file   StampTimer.h
 *  @author Naohisa Sakamoto
 */
/*****************************************************************************/
#pragma once
#include <kvs/Timer>
#include <kvs/Indent>
#include <vector>
#include <string>
#include <iostream>
#include <fstream>
#include <numeric>
#include <array>


namespace kvs
{

/*===========================================================================*/
/**
 *  @brief  Stamp timer class.
 */
/*===========================================================================*/
class StampTimer
{
public:
    using Time = float;
    using Times = std::vector<Time>;
    using UnitStrings = std::array<std::string, 4>;
    enum Unit { Sec = 0, MSec, USec, Fps };

private:
    std::string m_title = ""; ///< title of the stamped times
    Unit m_unit = Sec; ///< unit of time
    kvs::Timer m_timer{}; ///< timer
    Times m_times{}; ///< stamped times
    UnitStrings m_unit_strings{ "sec", "msec", "usec", "fps" }; ///< unit string

public:
    StampTimer() = default;
    StampTimer( const StampTimer& ) = default;
    StampTimer( const std::string& title ): m_title( title ) {}

    void setTitle( const std::string& title ) { m_title = title; }
    void setUnit( const Unit unit ) { m_unit = unit; }
    void setUnitToSec() { m_unit = Unit::Sec; }
    void setUnitToMSec() { m_unit = Unit::MSec; }
    void setUnitToUSec() { m_unit = Unit::USec; }
    void setUnitToFps() { m_unit = Unit::Fps; }

    const std::string& title() const { return m_title; }
    Unit unit() const { return m_unit; }
    const std::string& unitString() const { return m_unit_strings.at( this->unit() ); }
    const Times& times() const { return m_times; }
    Time time() const { return this->time( m_timer ); }
    size_t numberOfStamps() const { return m_times.size(); }
    Time last() const { return m_times.back(); }

    Time totalTime() const { return std::accumulate( std::begin( m_times ), std::end( m_times ), 0.0 ); }
    Time averageTime() const { return this->totalTime() / m_times.size(); }
    Time minTime() const { return *std::min_element( std::begin( m_times ), std::end( m_times ) ); }
    Time maxTime() const { return *std::max_element( std::begin( m_times ), std::end( m_times ) ); }

    void start() { m_timer.start(); }
    void stop() { m_timer.stop(); }
    void stamp( const Time time ) { m_times.push_back( time ); }

    void stamp( const kvs::Timer& timer )
    {
        this->stamp( this->time( timer ) );
    }

    void stamp( const bool restart = true )
    {
        this->stop();
        this->stamp( this->time() );
        if ( restart ) { this->start(); }
    }

    Time time( const kvs::Timer& timer ) const
    {
        switch ( m_unit )
        {
        case Unit::Sec: return static_cast<Time>( timer.sec() );
        case Unit::MSec: return static_cast<Time>( timer.msec() );
        case Unit::USec: return static_cast<Time>( timer.usec() );
        case Unit::Fps: return static_cast<Time>( timer.fps() );
        default: break;
        }
        return Time(0);
    }

    void print( std::ostream& os, const kvs::Indent& indent = kvs::Indent(0) ) const
    {
        if ( !m_title.empty() ) { os << indent << m_title << std::endl; }
        for ( const auto t : m_times ) { os << indent << t << std::endl; }
    }

    bool write( const std::string& filename, std::ios_base::openmode mode = std::ios_base::app ) const
    {
        std::ofstream ofs( filename.c_str(), std::ios::out | mode );
        if ( !ofs ) { return false; }
        this->print( ofs );
        return true;
    }

protected:
    Times& times() { return m_times; }
};

} // end of namespace kvs
