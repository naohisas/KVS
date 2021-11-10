/*****************************************************************************/
/**
 *  @file   DataSet.h
 *  @author Naohisa Sakamoto
 */
/*****************************************************************************/
#pragma once
#include <vector>
#include <string>
#include <kvs/Assert>
#include "Data.h"


namespace kvs
{

namespace gf
{

/*===========================================================================*/
/**
 *  @brief  GF DataSet class.
 */
/*===========================================================================*/
class DataSet
{
private:
    std::vector<std::string> m_comment_list{}; ///< comment list
    std::vector<kvs::gf::Data> m_data_list{}; ///< data list

public:
    DataSet() = default;

    friend std::ostream& operator << ( std::ostream& os, const DataSet& d );

    const std::vector<std::string>& commentList() const { return m_comment_list; }
    const std::string& comment( const size_t index ) const { return m_comment_list[index]; }
    const std::vector<kvs::gf::Data>& dataList() const { return m_data_list; }
    const kvs::gf::Data& data( const size_t index ) const { return m_data_list[index]; }
    void deallocate();

    bool readAscii( FILE* fp );
    bool readBinary( FILE* fp, const bool swap = false );
};

} // end of namespace gf

} // end of namespace kvs
