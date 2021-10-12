/*****************************************************************************/
/**
 *  @file   GrADS.h
 *  @author Naohisa Sakamoto
 */
/*****************************************************************************/
#pragma once
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
    using BaseClass = kvs::FileFormatBase;
    using DataDescriptorFile = kvs::grads::DataDescriptorFile;
    using GriddedBinaryDataFile = kvs::grads::GriddedBinaryDataFile;
    using GriddedBinaryDataFileList = std::vector<GriddedBinaryDataFile>;

private:
    DataDescriptorFile m_data_descriptor{}; ///< data descriptor file
    GriddedBinaryDataFileList m_data_list{}; ///< gridded binary data file list

public:
    static bool CheckExtension( const std::string& filename );

public:
    GrADS() = default;
    GrADS( const std::string& filename ) { this->read( filename ); }
    virtual ~GrADS() = default;

    const DataDescriptorFile& dataDescriptor() const { return m_data_descriptor; }
    const GriddedBinaryDataFileList& dataList() const { return m_data_list; }
    const GriddedBinaryDataFile& data( const size_t index ) const { return m_data_list[index]; }

    void print( std::ostream& os, const kvs::Indent& indent = kvs::Indent(0) ) const;
    bool read( const std::string& filename );

private:

    bool write( const std::string& filename );
};

} // end of namespace kvs
