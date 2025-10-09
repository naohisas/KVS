/*****************************************************************************/
/**
 *  @file   GridData.cpp
 *  @author Naohisa Sakamoto
 */
/*****************************************************************************/
#include "GridData.h"
#include "Reader.h"
#include <fstream>
#include <kvs/Vector2>


namespace kvs
{

namespace plot3d
{

/*===========================================================================*/
/**
 *  @brief  Print grid data information.
 *  @param  os [in] output stream
 *  @param  indent [in] indent
 */
/*===========================================================================*/
void GridData::print( std::ostream& os, const kvs::Indent& indent ) const
{
    auto print_block = [&] ( const size_t index )
    {
        const auto& block = m_block_list[ index ];
        os << indent.nextIndent()
           << "Block #" << index << ": "
           << "dims = " << block.dims.format() << std::endl;
    };

    os << indent << "Filename : " << BaseClass::filename() << std::endl;
    os << indent << "Number of blocks : " << m_block_list.size() << std::endl;
    const auto nblocks = m_block_list.size();
    if ( nblocks < 8 )
    {
        for ( size_t i = 0; i < nblocks; i++ ) { print_block( i ); }
    }
    else
    {
        for ( size_t i = 0; i < 8; i++ ) { print_block( i ); }
        os << indent.nextIndent() << "..." << std::endl;
        print_block( nblocks - 1 );
    }
}

/*===========================================================================*/
/**
 *  @brief  Read grid data file.
 *  @param  filename [in] filename
 *  @return true, if the reading process is done successfully
 */
/*===========================================================================*/
bool GridData::read( const std::string& filename )
{
    BaseClass::setFilename( filename );
    BaseClass::setSuccess( true );

    std::ifstream ifs( filename.c_str() );
    if ( !ifs.is_open() )
    {
        kvsMessageError() << "Cannot open " << filename.c_str() << "." << std::endl;
        BaseClass::setSuccess( false );
        return BaseClass::isSuccess();
    }

    const auto nblocks = plot3d::Reader::Value<int>( ifs );

    plot3d::Reader::Seek( ifs, 4 );
    for ( int i = 0; i < nblocks; i++ )
    {
        Block block;
        block.dims = plot3d::Reader::Vec3<int>( ifs, 0 );
        m_block_list.push_back( block );
    }
    plot3d::Reader::Seek( ifs, 4 );

    for ( auto& block : m_block_list )
    {
        const auto& dims = block.dims;
        const auto size = dims.x() * dims.y() * dims.z();
        plot3d::Reader::Seek( ifs, 4 );
        block.coords[0] = plot3d::Reader::Array<float>( ifs, size, 0 );
        block.coords[1] = plot3d::Reader::Array<float>( ifs, size, 0 );
        block.coords[2] = plot3d::Reader::Array<float>( ifs, size, 0 );
        plot3d::Reader::Seek( ifs, sizeof(int) * size ); // flags
        plot3d::Reader::Seek( ifs, 4 );
    }

    return BaseClass::isSuccess();
}

} // end of namespace plot3d

} // end of namespace kvs
