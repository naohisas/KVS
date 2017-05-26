#pragma once
#include <string>
#include <vector>
#include <iostream>
#include <fstream>
#include <kvs/Type>
#include <kvs/ValueArray>
#include <kvs/FileFormatBase>
#include <kvs/Indent>


namespace kvs
{

/*===========================================================================*/
/**
 *  @brief  IPLab image class.
 */
/*===========================================================================*/
class IPLab : public kvs::FileFormatBase
{
public:
    typedef kvs::FileFormatBase BaseClass;
    typedef kvs::ValueArray<kvs::UInt8> Data;
    typedef std::vector<Data> DataSet;

private:
    kvs::Int32 m_width; ///< data width
    kvs::Int32 m_height; ///< data height
    kvs::Int16 m_nframes; ///< number of frames
    DataSet m_dataset; ///< set of pixel data
    mutable size_t m_importing_frame_index; ///< frame index for importing

public:
    static bool CheckExtension( const std::string& filename );

public:
    IPLab();
    IPLab( const std::string& filename );

    kvs::Int32 width() const { return m_width; }
    kvs::Int32 height() const { return m_height; }
    kvs::Int16 numberOfFrames() const { return m_nframes; }
    const DataSet& dataSet() const { return m_dataset; }
    const Data& data( const size_t index ) const { return m_dataset[index]; }
    size_t importingFrameIndex() const { return m_importing_frame_index; }
    void setImportingFrameIndex( const size_t index ) const { m_importing_frame_index = index; }

    void print( std::ostream& os, const kvs::Indent& indent = kvs::Indent(0) ) const;
    bool read( const std::string& filename );

private:
    bool write( const std::string& filename );
};

} // end of namespace kvs
