/*****************************************************************************/
/**
 *  @file   Options.h
 *  @author Naohisa Sakamoto
 */
/*****************************************************************************/
#pragma once
#include <list>
#include <string>
#include <fstream>


namespace kvs
{

namespace grads
{

/*===========================================================================*/
/**
 *  @brief  OPTIONS entry.
 */
/*===========================================================================*/
struct Options
{
    enum Keyword
    {
        Pascals,
        Yrev,
        Zrev,
        Template,
        Sequential,
        DayCalendar365,
        ByteSwapped,
        BigEndian,
        LittleEndian,
        Cray32bitIEEE
    };

    std::list<Keyword> values{}; ///< keyword list

    bool read( std::string line, std::ifstream& ifs );
    bool find( const Keyword keyword ) const;
};

} // end of namespace grads

} // end of namespace kvs
