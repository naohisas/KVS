/*****************************************************************************/
/**
 *  @file   File.h
 *  @author Naohisa Sakamoto
 */
/*****************************************************************************/
#pragma once
#include "DataSet.h"
#include <vector>
#include <string>


namespace kvs
{

namespace gf
{

/*===========================================================================*/
/**
 *  @brief  GF File class.
 */
/*===========================================================================*/
class File
{
private:
    std::string m_file_type_header = ""; ///< file type header
    std::vector<std::string> m_comment_list{}; ///< comment list
    std::vector<kvs::gf::DataSet> m_data_set_list{}; ///< data set list

public:
    File() = default;
    File( const std::string filename ) { this->read( filename ); }

    friend std::ostream& operator << ( std::ostream& os, const File& f );

    const std::string& fileTypeHeader() const { return m_file_type_header; }
    const std::vector<std::string>& commentList() const { return m_comment_list; }
    const std::string& comment( const size_t index ) const { return m_comment_list[index]; }
    const std::vector<kvs::gf::DataSet>& dataSetList() const { return m_data_set_list; }
    const kvs::gf::DataSet& dataSet( const size_t index ) const { return m_data_set_list[index]; }
    void deallocate();

    bool read( const std::string filename );

private:
    bool is_ascii( const std::string filename );
    bool is_binary( const std::string filename );
    bool read_ascii( const std::string filename );
    bool read_binary( const std::string filename, const bool swap = false );
};

} // end of namespace gf

} // end of namespace kvs
