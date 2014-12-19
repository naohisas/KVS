/*****************************************************************************/
/**
 *  @file   GrADS.h
 *  @author Naohisa Sakamoto
 */
/*----------------------------------------------------------------------------
 *
 *  Copyright (c) Visualization Laboratory, Kyoto University.
 *  All rights reserved.
 *  See http://www.viz.media.kyoto-u.ac.jp/kvs/copyright/ for details.
 *
 *  $Id: GrADS.h 1313 2012-09-19 08:19:01Z naohisa.sakamoto@gmail.com $
 */
/*****************************************************************************/
#ifndef KVS__GRADS_H_INCLUDE
#define KVS__GRADS_H_INCLUDE

#include <iostream>
#include <kvs/FileFormatBase>
#include <kvs/Indent>
#include "DataDescriptorFile.h"
#include "GriddedBinaryDataFile.h"


namespace kvs
{

/*===========================================================================*/
/**
 *  @brief  GrADS class.
 */
/*===========================================================================*/
class GrADS : public kvs::FileFormatBase
{
public:

    typedef kvs::FileFormatBase BaseClass;
    typedef kvs::grads::DataDescriptorFile DataDescriptorFile;
    typedef kvs::grads::GriddedBinaryDataFile GriddedBinaryDataFile;
    typedef std::vector<GriddedBinaryDataFile> GriddedBinaryDataFileList;

private:

    DataDescriptorFile m_data_descriptor; ///< data descriptor file
    GriddedBinaryDataFileList m_data_list; ///< gridded binary data file list

public:

    static bool CheckExtension( const std::string& filename );

public:

    GrADS();
    GrADS( const std::string& filename );
    virtual ~GrADS();

    const DataDescriptorFile& dataDescriptor() const;
    const GriddedBinaryDataFileList& dataList() const;
    const GriddedBinaryDataFile& data( const size_t index ) const;

    void print( std::ostream& os, const kvs::Indent& indent = kvs::Indent(0) ) const;
    bool read( const std::string& filename );

private:

    bool write( const std::string& filename );
};

} // end of namespace kvs

#endif // KVS__GRADS_H_INCLUDE
