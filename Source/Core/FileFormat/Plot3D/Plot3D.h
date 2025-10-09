/*****************************************************************************/
/**
 *  @file   Plot3dData.h
 *  @author Naohisa Sakamoto
 */
/*****************************************************************************/
#pragma once
#include <iostream>
#include <kvs/FileFormatBase>
#include <kvs/Indent>
#include "GridData.h"
#include "SolutionData.h"


namespace kvs
{

/*===========================================================================*/
/**
 *  @brief  Plot3D data class.
 */
/*===========================================================================*/
class Plot3D : public kvs::FileFormatBase
{
public:
    using BaseClass = kvs::FileFormatBase;

private:
    plot3d::GridData m_grid_data{}; ///< grid data
    plot3d::SolutionData m_solution_data{}; ///< solution data

public:
    Plot3D() = default;
    virtual ~Plot3D() = default;
    Plot3D( const std::string& filenames );
    Plot3D( const std::string& grid_file, const std::string& solution_file );

    void setGridData( const plot3d::GridData& grid_data ) { m_grid_data = grid_data; }
    void setSolutionData( const plot3d::SolutionData& solution_data ) { m_solution_data = solution_data; }

    const plot3d::GridData& gridData() const { return m_grid_data; }
    const plot3d::SolutionData& solutionData() const { return m_solution_data; }

    void print( std::ostream& os, const kvs::Indent& indent = kvs::Indent(0) ) const;
    bool read( const std::string& filenames );
    bool read( const std::string& grid_file, const std::string& solution_file );

private:
    bool write( const std::string& ) { return false; }
};

} // end of namespace kvs
