/*****************************************************************************/
/**
 *  @file   GriddedBinaryDataFile.h
 *  @author Naohisa Sakamoto
 */
/*****************************************************************************/
#pragma once
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
        int hour = 0; ///< hour
        int minute = 0; ///< minute
        int day = 1; ///< day
        int month = 1; ///< month
        int year = 0; ///< year
    };

private:
    Date m_date{}; ///< date
    bool m_sequential = false; ///< sequential data or not
    bool m_big_endian = false; ///< big endian data or not
    std::string m_filename = ""; ///< data filename
    mutable kvs::ValueArray<kvs::Real32> m_values{}; ///< data values

public:
    GriddedBinaryDataFile() = default;

    void setDate( const Date& date ) { m_date = date; }
    void setSequential( const bool sequential ) { m_sequential = sequential; }
    void setBigEndian( const bool big_endian ) { m_big_endian = big_endian; }
    void setFilename( const std::string& filename ) { m_filename = filename; }
    const Date& date() const { return m_date; }
    bool sequential() const { return m_sequential; }
    bool bigEndian() const { return m_big_endian; }
    const std::string& filename() const { return m_filename; }
    const kvs::ValueArray<kvs::Real32>& values() const { return m_values; }
    const kvs::ValueArray<kvs::Real32> values( const size_t vindex, const kvs::Vec3ui& dim ) const;
    bool load() const;
    void free() const;
};

} // end of namespace grads

} // end of namespace kvs
