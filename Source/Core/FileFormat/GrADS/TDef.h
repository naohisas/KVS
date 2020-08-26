/*****************************************************************************/
/**
 *  @file   TDef.h
 *  @author Naohisa Sakamoto
 */
/*****************************************************************************/
#ifndef KVS__GRADS__TDEF_H_INCLUDE
#define KVS__GRADS__TDEF_H_INCLUDE

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
        int hour; ///< hour
        int minute; ///< minute
        int day; ///< day
        int month; ///< month
        int year; ///< year
    };

    struct Increment
    {
        int value; ///< value
        int unit; ///< unit (0:mn, 1:hr, 2:dy, 3:mo, 4:yr)
    };

    size_t num; ///< number of the time steps
    Start start; ///< start time/date
    Increment increment; ///< increment time

    bool read( std::string line, std::ifstream& ifs );

    TDef& operator ++ ();
};

} // end of namespace grads

} // end of namespace kvs

#endif // KVS__GRADS__TDEF_H_INCLUDE
