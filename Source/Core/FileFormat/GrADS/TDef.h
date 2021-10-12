/*****************************************************************************/
/**
 *  @file   TDef.h
 *  @author Naohisa Sakamoto
 */
/*****************************************************************************/
#pragma once
#include <string>
#include <fstream>


namespace kvs
{

namespace grads
{

/*===========================================================================*/
/**
 *  @brief  TDEF entry.
 */
/*===========================================================================*/
struct TDef
{
    struct Start
    {
        int hour = 0; ///< hour
        int minute = 0; ///< minute
        int day = 1; ///< day
        int month = 1; ///< month
        int year = 0; ///< year
    };

    struct Increment
    {
        int value = 0; ///< value
        int unit = 0; ///< unit (0:mn, 1:hr, 2:dy, 3:mo, 4:yr)
    };

    size_t num = 0; ///< number of the time steps
    Start start{}; ///< start time/date
    Increment increment{}; ///< increment time

    bool read( std::string line, std::ifstream& ifs );

    TDef& operator ++ ();
};

} // end of namespace grads

} // end of namespace kvs
