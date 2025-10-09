/*****************************************************************************/
/**
 *  @file   SolutionData.h
 *  @author Naohisa Sakamoto
 */
/*****************************************************************************/
#pragma once
#include <kvs/FileFormatBase>
#include <kvs/Indent>
#include <kvs/Vector3>
#include <kvs/Vector4>
#include <kvs/ValueArray>
#include <string>
#include <fstream>


namespace kvs
{

namespace plot3d
{

/*===========================================================================*/
/**
 *  @brief  SolutionData class
 */
/*===========================================================================*/
class SolutionData : public kvs::FileFormatBase
{
public:
    using BaseClass = kvs::FileFormatBase;
    using Dims = kvs::Vec3i;
    using Params = kvs::Vec4;
    using Values = kvs::ValueArray<float>;
    struct Block
    {
        static const int nvars = 5; ///< density, momentum (3 components), energy
        Dims dims; ///< dimensions for x, y, and z coordinates
        Params params; ///< Mach number, angle-of-attack, Reynolds number, time
        Values values[nvars]; ///< values for 'density, momentum, energy'
    };
    using BlockList = std::vector<Block>;

private:
    int m_dimension = 3; ///< dimension of the data (2 or 3)
    BlockList m_block_list{}; ///< block list

public:
    SolutionData() = default;
    SolutionData( const std::string& filename ) { this->read( filename ); }
    virtual ~SolutionData() = default;

    int dimension() const { return m_dimension; }
    const BlockList& blockList() const { return m_block_list; }
    const Block& block( const size_t index =0 ) const { return m_block_list[index]; }

    void print( std::ostream& os, const kvs::Indent& indent = kvs::Indent(0) ) const;
    bool read( const std::string& filename );

private:
    bool write( const std::string& ) { return false; }
};

} // end of namespace plot3d

} // end of namespace kvs
