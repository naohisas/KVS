/*****************************************************************************/
/**
 *  @file   GriddedBinaryDataFile.h
 *  @author Naohisa Sakamoto
 */
/*----------------------------------------------------------------------------
 *
 *  Copyright (c) Visualization Laboratory, Kyoto University.
 *  All rights reserved.
 *  See http://www.viz.media.kyoto-u.ac.jp/kvs/copyright/ for details.
 *
 *  $Id: GriddedBinaryDataFile.h 1793 2014-08-01 07:43:50Z naohisa.sakamoto@gmail.com $
 */
/*****************************************************************************/
#ifndef KVS__GRADS__GRIDDED_BINARY_DATA_FILE_H_INCLUDE
#define KVS__GRADS__GRIDDED_BINARY_DATA_FILE_H_INCLUDE

#include <string>
#include <kvs/ValueArray>
#include <kvs/Type>
#include <kvs/Vector3>


namespace kvs
{

namespace grads
{

/*===========================================================================*/
/**
 *  @brief  GriddedBinaryDataFile class.
 */
/*===========================================================================*/
class GriddedBinaryDataFile
{
public:

    struct Date
    {
        int hour; ///< hour
        int minute; ///< minute
        int day; ///< day
        int month; ///< month
        int year; ///< year
    };

private:

    Date m_date; ///< date
    bool m_sequential; ///< sequential data or not
    bool m_big_endian; ///< big endian data or not
    std::string m_filename; ///< data filename
    mutable kvs::ValueArray<kvs::Real32> m_values; ///< data values

public:

    GriddedBinaryDataFile();

    void setDate( const Date& date );
    void setSequential( const bool sequential );
    void setBigEndian( const bool big_endian );
    void setFilename( const std::string& filename );
    const Date& date() const;
    bool sequential() const;
    bool bigEndian() const;
    const std::string& filename() const;
    const kvs::ValueArray<kvs::Real32>& values() const;
    const kvs::ValueArray<kvs::Real32> values( const size_t vindex, const kvs::Vec3ui& dim ) const;
    bool load() const;
    void free() const;
};

} // end of namespace grads

} // end of namespace kvs

#endif // KVS__GRADS__GRIDDED_BINARY_DATA_FILE_H_INCLUDE
