/*****************************************************************************/
/**
 *  @file   File.h
 *  @author Naohisa Sakamoto
 */
/*----------------------------------------------------------------------------
 *
 *  Copyright (c) Visualization Laboratory, Kyoto University.
 *  All rights reserved.
 *  See http://www.viz.media.kyoto-u.ac.jp/kvs/copyright/ for details.
 *
 *  $Id: File.h 1309 2012-09-18 04:29:34Z naohisa.sakamoto@gmail.com $
 */
/*****************************************************************************/
#ifndef KVS__GF__FILE_H_INCLUDE
#define KVS__GF__FILE_H_INCLUDE

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

    std::string m_file_type_header; ///< file type header
    std::vector<std::string> m_comment_list; ///< comment list
    std::vector<kvs::gf::DataSet> m_data_set_list; ///< data set list

public:

    File();
    File( const std::string filename );

    friend std::ostream& operator << ( std::ostream& os, const File& f );

    const std::string& fileTypeHeader() const;
    const std::vector<std::string>& commentList() const;
    const std::string& comment( const size_t index ) const;
    const std::vector<kvs::gf::DataSet>& dataSetList() const;
    const kvs::gf::DataSet& dataSet( const size_t index ) const;
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

#endif // KVS__GF__FILE_H_INCLUDE
