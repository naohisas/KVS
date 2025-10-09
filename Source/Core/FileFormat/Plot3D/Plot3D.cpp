/*****************************************************************************/
/**
 *  @file   Plot3D.cpp
 *  @author Naohisa Sakamoto
 */
/*****************************************************************************/
#include "Plot3D.h"
#include <kvs/IgnoreUnusedVariable>
#include <kvs/Tokenizer>


namespace kvs
{

/*===========================================================================*/
/**
 *  @brief  Construct a new Plot3D class.
 *  @param  filenames [in] filenames (ex. "aaa.xyz;bbb.q" )
 */
/*===========================================================================*/
Plot3D::Plot3D( const std::string& filenames )
{
    this->read( filenames );
}

/*===========================================================================*/
/**
 *  @brief  Construct a new Plot3D class.
 *  @param  grid_file [in] filename of grid data
 *  @param  solution_file [in] filename of solution data
 */
/*===========================================================================*/
Plot3D::Plot3D( const std::string& grid_file, const std::string& solution_file )
{
    this->read( grid_file, solution_file );
}

/*===========================================================================*/
/**
 *  @brief  Print file information.
 *  @param  os [in] output stream
 *  @param  indent [in] indent
 */
/*===========================================================================*/
void Plot3D::print( std::ostream& os, const kvs::Indent& indent ) const
{
    os << indent << "Filename : " << BaseClass::filename() << std::endl;
    os << indent << "Grid data : " << std::endl;
    m_grid_data.print( os, indent.nextIndent() );
    os << indent << "Solution data : " << std::endl;
    m_solution_data.print( os, indent.nextIndent() );
}

/*===========================================================================*/
/**
 *  @brief  Read Plot3D data files; grid data file and solution data file.
 *  @param  filenames [in] filenames (ex. "aaa.xyz;bbb.q" )
 *  @return true, if the reading process is done successfully
 */
/*===========================================================================*/
bool Plot3D::read( const std::string& filenames )
{
    kvs::Tokenizer t( filenames, ";" );
    const std::string grid_file = t.isLast() ? "" : t.token();
    const std::string solution_file = t.isLast() ? "" : t.token();
    return this->read( grid_file, solution_file );
}

/*===========================================================================*/
/**
 *  @brief  Read Plot3D data files; grid data file and solution data file.
 *  @param  grid_file [in] filename of grid data
 *  @param  solution_file [in] filename of solution data
 *  @return true, if the reading process is done successfully
 */
/*===========================================================================*/
bool Plot3D::read( const std::string& grid_file, const std::string& solution_file )
{
    auto filename = grid_file + ";" + solution_file;
    BaseClass::setFilename( filename );
    BaseClass::setSuccess( true );

    if ( !m_grid_data.read( grid_file ) )
    {
        kvsMessageError() << "Cannot read grid file " << grid_file.c_str() << "." << std::endl;
        BaseClass::setSuccess( false );
    }

    if ( !m_solution_data.read( solution_file ) )
    {
        kvsMessageError() << "Cannot read solution file " << solution_file.c_str() << "." << std::endl;
        BaseClass::setSuccess( false );
    }

    return BaseClass::isSuccess();
}

} // end of namespace kvs
