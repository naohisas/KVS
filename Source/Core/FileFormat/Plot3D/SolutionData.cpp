/*****************************************************************************/
/**
 *  @file   SolutionData.cpp
 *  @author Naohisa Sakamoto
 */
/*****************************************************************************/
#include "SolutionData.h"
#include "Reader.h"
#include <kvs/Message>


namespace kvs
{

namespace plot3d
{

/*===========================================================================*/
/**
 *  @brief  Print solution data information.
 *  @param  os [in] output stream
 *  @param  indent [in] indent
 */
/*===========================================================================*/
void SolutionData::print( std::ostream& os, const kvs::Indent& indent ) const
{
    auto print_block = [&] ( const size_t index )
    {
        const auto& block = m_block_list[ index ];
        os << indent.nextIndent()
           << "Block #" << index << ": "
           << "dims = " << block.dims.format() << ", "
           << "params = " << block.params.format() << std::endl;
    };

    os << indent << "Filename: " << BaseClass::filename() << std::endl;
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
 *  @brief  Read solution data file.
 *  @param  filename [in] filename
 *  @return true, if the reading process is done successfully
 */
/*===========================================================================*/
bool SolutionData::read( const std::string& filename )
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
        plot3d::Reader::Seek( ifs, 4 );
        block.params[0] = plot3d::Reader::Value<float>( ifs, 0 ); // Mach number
        block.params[1] = plot3d::Reader::Value<float>( ifs, 0 ); // angle-of-attack
        block.params[2] = plot3d::Reader::Value<float>( ifs, 0 ); // Reynolds
        block.params[3] = plot3d::Reader::Value<float>( ifs, 0 ); // time
        plot3d::Reader::Seek( ifs, 4 );

        const auto dims = block.dims;
        const auto ngrids = dims.x() * dims.y() * dims.z();
        plot3d::Reader::Seek( ifs, 4 );
        block.values[0] = plot3d::Reader::Array<float>( ifs, ngrids, 0 ); // density
        block.values[1] = plot3d::Reader::Array<float>( ifs, ngrids, 0 ); // momentum 1
        block.values[2] = plot3d::Reader::Array<float>( ifs, ngrids, 0 ); // momentum 2
        block.values[3] = plot3d::Reader::Array<float>( ifs, ngrids, 0 ); // momentum 3
        block.values[4] = plot3d::Reader::Array<float>( ifs, ngrids, 0 ); // energy
        plot3d::Reader::Seek( ifs, 4 );
    }

    return BaseClass::isSuccess();
}

} // end of namespace plot3d

} // end of namespace kvs
