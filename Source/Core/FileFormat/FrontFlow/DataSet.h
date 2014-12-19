/*****************************************************************************/
/**
 *  @file   DataSet.h
 *  @author Naohisa Sakamoto
 */
/*----------------------------------------------------------------------------
 *
 *  Copyright (c) Visualization Laboratory, Kyoto University.
 *  All rights reserved.
 *  See http://www.viz.media.kyoto-u.ac.jp/kvs/copyright/ for details.
 *
 *  $Id: DataSet.h 1309 2012-09-18 04:29:34Z naohisa.sakamoto@gmail.com $
 */
/*****************************************************************************/
#ifndef KVS__GF__DATA_SET_H_INCLUDE
#define KVS__GF__DATA_SET_H_INCLUDE

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

    std::vector<std::string> m_comment_list; ///< comment list
    std::vector<kvs::gf::Data> m_data_list; ///< data list

public:

    DataSet();

    friend std::ostream& operator << ( std::ostream& os, const DataSet& d );

    const std::vector<std::string>& commentList() const;
    const std::string& comment( const size_t index ) const;
    const std::vector<kvs::gf::Data>& dataList() const;
    const kvs::gf::Data& data( const size_t index ) const;
    void deallocate();

    bool readAscii( FILE* fp );
    bool readBinary( FILE* fp, const bool swap = false );
};

} // end of namespace gf

} // end of namespace kvs

#endif // KVS__GF__DATA_SET_H_INCLUDE
