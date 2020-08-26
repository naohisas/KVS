/*****************************************************************************/
/**
 *  @file   GridPoint.h
 *  @author Naohisa Sakamoto
 */
/*****************************************************************************/
#ifndef KVS__GRADS__XYZDEF_H_INCLUDE
#define KVS__GRADS__XYZDEF_H_INCLUDE

#include <string>
#include <fstream>
#include <kvs/ValueArray>
#include <kvs/Type>


namespace kvs
{

namespace grads
{

/*===========================================================================*/
/**
 *  @brief  XDEF/YDEF/ZDEF entry.
 */
/*===========================================================================*/
struct XYZDef
{
    enum MappingMethod
    {
        Linear,
        Levels,
        GausT62,
        GausR15,
        GausR20,
        GausR30,
        GausR40
    };

    size_t num; ///< number of grid points
    MappingMethod mapping; ///< mapping method
    kvs::ValueArray<kvs::Real32> values; ///< values

    bool read( std::string line, std::ifstream& ifs );
};

} // end of namespace grads

} // end of namespace kvs

#endif // KVS__GRADS__XYZDEF_H_INCLUDE
