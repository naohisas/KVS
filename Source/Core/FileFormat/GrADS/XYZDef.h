/*****************************************************************************/
/**
 *  @file   GridPoint.h
 *  @author Naohisa Sakamoto
 */
/*****************************************************************************/
#pragma once
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

    size_t num = 0; ///< number of grid points
    MappingMethod mapping = MappingMethod::Linear; ///< mapping method
    kvs::ValueArray<kvs::Real32> values{}; ///< values

    bool read( std::string line, std::ifstream& ifs );
};

} // end of namespace grads

} // end of namespace kvs
