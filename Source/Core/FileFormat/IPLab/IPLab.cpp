#include "IPLab.h"
#include <iostream>
#include <fstream>
#include <kvs/Endian>
#include <kvs/File>
#include <kvs/IgnoreUnusedVariable>


namespace kvs
{

bool IPLab::CheckExtension( const std::string& filename )
{
    const kvs::File file( filename );
    if ( file.extension() == "ipl" || file.extension() == "ipm" )
    {
        return true;
    }

    return false;
}

IPLab::IPLab():
    m_width(0),
    m_height(0),
    m_nframes(0),
    m_importing_frame_index(0)
{
}

void IPLab::print( std::ostream& os, const kvs::Indent& indent ) const
{
    os << indent << "Filename : " << BaseClass::filename() << std::endl;
    os << indent << "Width : " << m_width << std::endl;
    os << indent << "Height : " << m_height << std::endl;
    os << indent << "Number of frames : " << m_nframes;
}

IPLab::IPLab( const std::string& filename ):
    m_width(0),
    m_height(0),
    m_nframes(0),
    m_importing_frame_index(0)
{
    this->read( filename );
}

bool IPLab::read( const std::string& filename )
{
    BaseClass::setFilename( filename );
    BaseClass::setSuccess( true );

    std::ifstream ifs( filename.c_str(), std::ios::binary | std::ios::in );
    if ( !ifs.is_open() )
    {
        kvsMessageError( "Cannot open %s.", filename.c_str() );
        BaseClass::setSuccess( false );
        return false;
    }

    kvs::Int8 version[4];
    ifs.read( (char*)version, 4 );

    kvs::UInt8 format;
    ifs.read( (char*)&format, 1 );

    kvs::UInt8 data_type; // 6 = ushort
    ifs.read( (char*)&data_type, 1 );

    ifs.read( (char*)&m_width, 4 );
    kvs::Endian::Swap( &m_width );

    ifs.read( (char*)&m_height, 4 );
    kvs::Endian::Swap( &m_height );

    kvs::Int8 researved1[6];
    ifs.read( (char*)researved1, 6 );

    ifs.read( (char*)&m_nframes, 2 );
    kvs::Endian::Swap( &m_nframes );

    kvs::Int8 researved2[50];
    ifs.read( (char*)researved2, 50 );

    kvs::Int8 clut[2048];
    ifs.read( (char*)clut, 2048 );

    for ( kvs::UInt16 i = 0; i < m_nframes; i++ )
    {
        kvs::ValueArray<kvs::UInt8> data( m_width * m_height );
        for ( size_t j = 0; j < data.size(); j++ )
        {
            kvs::UInt16 temp;
            ifs.read( (char*)&temp, 2 );
            kvs::Endian::Swap( &temp );
            data[j] = (int)( temp / 4095.0f * 255.0f );
        }
        m_dataset.push_back( data );
    }

    ifs.close();

    return true;
}

bool IPLab::write( const std::string& filename )
{
    kvs::IgnoreUnusedVariable( filename );
    return true;
}

} // end of namespace kvs
