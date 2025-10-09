/*****************************************************************************/
/**
 *  @file   GridData.h
 *  @author Naohisa Sakamoto
 */
/*****************************************************************************/
#pragma once
#include <kvs/FileFormatBase>
#include <kvs/Vector3>
#include <kvs/Indent>
#include <kvs/ValueArray>
#include <string>
#include <iostream>
#include <vector>


namespace kvs
{

namespace plot3d
{

/*===========================================================================*/
/**
 *  @brief  GridData class
 */
/*===========================================================================*/
class GridData : public kvs::FileFormatBase
{
public:
    using BaseClass = kvs::FileFormatBase;
    using Dims = kvs::Vec3i;
    using Coords = kvs::ValueArray<float>;
    struct Block
    {
        Dims dims; ///< {dimx,dimy,dimz}
        Coords coords[3]; ///< {x0,x1,...}, {y0,y1,...}, {z0,z1,...}
    };
    using BlockList = std::vector<Block>;

private:
    int m_dimension = 3; ///< dimension of the data (2 or 3)
    BlockList m_block_list{}; ///< block list

public:
    GridData() = default;
    GridData( const std::string& filename ) { this->read( filename ); }
    virtual ~GridData() = default;

    int dimension() const { return m_dimension; }
    const BlockList& blockList() const { return m_block_list; }
    const Block& block( const size_t index = 0 ) const { return m_block_list[index]; }
    void print( std::ostream& os, const kvs::Indent& indent = kvs::Indent(0) ) const;
    bool read( const std::string& filename );

private:
    bool write( const std::string& ) { return false; }
};

} // end of namespace plot3d

} // end of namespace kvs
