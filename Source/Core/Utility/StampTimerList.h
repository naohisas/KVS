/*****************************************************************************/
/**
 *  @file   StampTimerList.h
 *  @author Naohisa Sakamoto
 */
/*****************************************************************************/
#pragma once
#include <vector>
#include <string>
#include <kvs/Math>
#include <kvs/StampTimer>


namespace kvs
{

class StampTimerList
{
private:
    size_t m_max_nstamps = 0; ///< max. number of stamps in the stamp timers
    std::vector<kvs::StampTimer> m_stamp_timers{}; ///< stamp timer list

public:
    StampTimerList() = default;

    void push( const kvs::StampTimer& timer )
    {
        m_max_nstamps = kvs::Math::Max( m_max_nstamps, timer.numberOfStamps() );
        m_stamp_timers.push_back( timer );
    }

    void clear()
    {
        m_max_nstamps = 0;
        m_stamp_timers.clear();
        m_stamp_timers.shrink_to_fit();
    }

    void print(
        std::ostream& os,
        const std::string delim = ", " ) const;

    bool write(
        const std::string& filename,
        const std::string delim = ", ",
        std::ios_base::openmode mode = std::ios_base::app ) const;

private:
    bool has_title() const
    {
        for ( const auto& t : m_stamp_timers )
        {
            if ( !t.title().empty() ) { return true; }
        }
        return false;
    }
};

/*===========================================================================*/
/**
 *  @brief  Prints stamped times to specified output stream.
 *  @param  os [in] output stream
 *  @param  delim [in] delimiter
 */
/*===========================================================================*/
inline void StampTimerList::print(
    std::ostream& os,
    const std::string delim ) const
{
    const size_t nrows = m_max_nstamps;
    const size_t ncols = m_stamp_timers.size();
    if ( ncols == 0 ) { return; }

    if ( this->has_title() )
    {
        for ( size_t j = 0; j < ncols - 1; ++j )
        {
            const auto& title = m_stamp_timers[j].title();
            os << title << delim;
        }
        const auto& title = m_stamp_timers[ ncols - 1 ].title();
        os << title << std::endl;
    }

    for ( size_t i = 0; i < nrows; ++i )
    {
        for ( size_t j = 0; j < ncols - 1; ++j )
        {
            const auto& col = m_stamp_timers[j];
            if ( i < col.numberOfStamps() ) { os << col.times()[i] << delim; }
            else { os << delim; }
        }
        const auto& col = m_stamp_timers[ ncols - 1 ];
        if ( i < col.numberOfStamps() ) { os << col.times()[i] << std::endl; }
        else { os << std::endl; }
    }
}

/*===========================================================================*/
/**
 *  @brief  Writes stamped times to a specified file.
 *  @param  filename [in] output filename
 *  @param  delim [in] delimiter
 *  @param  mode [in] output file mode
 *  @return true if the stamped times are output successfully
 */
/*===========================================================================*/
inline bool StampTimerList::write(
    const std::string& filename,
    const std::string delim,
    std::ios_base::openmode mode ) const
{
    std::ofstream ofs( filename.c_str(), std::ios::out | mode );
    if ( !ofs ) { return false; }
    this->print( ofs, delim );
    return true;
}

} // end of namespace kvs
