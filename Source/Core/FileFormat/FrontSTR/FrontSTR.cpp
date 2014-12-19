/*****************************************************************************/
/**
 *  @file   FrontSTR.cpp
 *  @author Guo Jiazhen
 */
/*----------------------------------------------------------------------------
 *
 *  Copyright (c) Visualization Laboratory, Kyoto University.
 *  All rights reserved.
 *  See http://www.viz.media.kyoto-u.ac.jp/kvs/copyright/ for details.
 *
 *  $Id: FrontSTR.cpp 1313 2012-09-19 08:19:01Z naohisa.sakamoto@gmail.com $
 */
/*****************************************************************************/
#include "FrontSTR.h"
#include <vector>
#include <fstream>
#include <sstream>
#include <algorithm>
#include <kvs/Tokenizer>
#include <kvs/IgnoreUnusedVariable>
#include <kvs/Message>
#include <kvs/Assert>


namespace kvs
{

bool FrontSTR::CheckExtension( const std::string& filename )
{
    kvs::IgnoreUnusedVariable( filename );
    return false;
}

/*===========================================================================*/
/**
 *  @brief  Constructs a new FrontSTR class.
 */
/*===========================================================================*/
FrontSTR::FrontSTR():
    m_nmeshes( 0 ),
    m_nresults( 0 )
{
    m_mesh_data = NULL;
    m_result_data = NULL;
}

/*===========================================================================*/
/**
 *  @brief  Constructs a new FrontSTR class.
 *  @param  filenames [in] filename list separated by ';'
 */
/*===========================================================================*/
FrontSTR::FrontSTR( const std::string& filenames )
{
    this->read( filenames );
}

/*===========================================================================*/
/**
 *  @brief  Constructs a new FrontSTR class.
 *  @param  msh_filename [in] filename of mesh data
 *  @param  res_filename [in] filename of result data
 */
/*===========================================================================*/
FrontSTR::FrontSTR( const std::string& msh_filename, const std::string& res_filename )
{
    this->read( msh_filename, res_filename );
}

/*===========================================================================*/
/**
 *  @brief  Constructs a new FrontSTR class.
 *  @param  msh_filenames [in] filename list of mesh data
 *  @param  res_filenames [in] filename list of result data
 */
/*===========================================================================*/
FrontSTR::FrontSTR( const std::vector<std::string>& msh_filenames, const std::vector<std::string>& res_filenames )
{
    this->read( msh_filenames, res_filenames );
}

/*===========================================================================*/
/**
 *  @brief  Destroys the FrontSTR class.
 */
/*===========================================================================*/
FrontSTR::~FrontSTR()
{
    this->delete_data();
}

/*===========================================================================*/
/**
 *  @brief  Returns number of mesh data.
 *  @return number of mesh data
 */
/*===========================================================================*/
size_t FrontSTR::numberOfMeshData() const
{
    return m_nmeshes;
}

/*===========================================================================*/
/**
 *  @brief  Returns number of result data.
 *  @return number of result data
 */
/*===========================================================================*/
size_t FrontSTR::numberOfResultData() const
{
    return m_nresults;
}

/*===========================================================================*/
/**
 *  @brief  Returns mesh data specified by the index.
 *  @param  index [in] index of mesh data
 *  @return mesh data
 */
/*===========================================================================*/
const kvs::fstr::MeshData& FrontSTR::meshData( const size_t index ) const
{
    KVS_ASSERT( index < m_nmeshes );
    return m_mesh_data[ index ];
}

/*===========================================================================*/
/**
 *  @brief  Returns result data specified by the index.
 *  @param  index [in] index of result data
 *  @return mesh data
 */
/*===========================================================================*/
const kvs::fstr::ResultData& FrontSTR::resultData( const size_t index ) const
{
    KVS_ASSERT( index < m_nresults );
    return m_result_data[ index ];
}

/*===========================================================================*/
/**
 *  @brief  Read FrontSTR data composed of some mesh and result data files.
 *  @param  filenames [in] filename list separated by ';'
 *  @return true if the reading process is done successfully
 */
/*===========================================================================*/
bool FrontSTR::read( const std::string& filenames )
{
    std::vector<std::string> msh_filenames; // mesh files
    std::vector<std::string> res_filenames; // result files

    kvs::Tokenizer t( filenames, ";" );
    while ( !t.isLast() )
    {
        const std::string filename = t.token();
        if ( filename.find( ".msh" ) != std::string::npos )
        {
            msh_filenames.push_back( filename );
        }
        else if ( filename.find( ".res" ) != std::string::npos )
        {
            res_filenames.push_back( filename );
        }
    }

    if ( msh_filenames.size() == 1 && res_filenames.size() == 1 )
    {
        return this->read( msh_filenames[0], res_filenames[0] );
    }
    else
    {
        return this->read( msh_filenames, res_filenames );
    }
}

/*===========================================================================*/
/**
 *  @brief  Reads FrontSTR data (non divided data).
 *  @param  msh_filename [in] filename of mesh data
 *  @param  res_filename [in] filename of result data
 *  @return true if the reading process is done successfully
 */
/*===========================================================================*/
bool FrontSTR::read( const std::string& msh_filename, const std::string& res_filename )
{
    const std::string filename = msh_filename + ";" + res_filename;
    BaseClass::setFilename( filename );
    BaseClass::setSuccess( true );

    m_nmeshes = 1;
    m_nresults = 1;

    if ( !this->allocate_data() )
    {
        kvsMessageError("Cannot allocate memory for member parameters.");
        this->delete_data();
        BaseClass::setSuccess( false );
        return false;
    }

    // Reading non divided mesh data.
    if ( !m_mesh_data[0].readData( msh_filename ) )
    {
        kvsMessageError("Cannot read %s.", msh_filename.c_str() );
        BaseClass::setSuccess( false );
        return false;
    }

    // Reading non divided result data.
    if ( !m_result_data[0].readData( res_filename ) )
    {
        kvsMessageError("Cannot read %s.", res_filename.c_str() );
        BaseClass::setSuccess( false );
        return false;
    }

    return true;
}

void FrontSTR::print( std::ostream& os, const kvs::Indent& indent ) const
{
    os << indent << "Filename : " << BaseClass::filename() << std::endl;
    os << indent << "Number of mesh data : " << m_nmeshes << std::endl;
    os << indent << "Number of result data : " << m_nresults << std::endl;
    for ( size_t i = 0; i < m_nmeshes; i++ )
    {
        os << indent << "Mesh data [" << i << "] : " << std::endl;
        m_mesh_data[i].print( os, indent.nextIndent() );
    }
    for ( size_t i = 0; i < m_nresults; i++ )
    {
        os << indent << "Result data [" << i << "] : " << std::endl;
        m_result_data[i].print( os, indent.nextIndent() );
    }
}

/*===========================================================================*/
/**
 *  @brief  Reads FrontSTR data (divided data).
 *  @param  msh_filenames [in] filename list of mesh data
 *  @param  res_filenames [in] filename list of result data
 *  @return true if the reading process is done successfully
 */
/*===========================================================================*/
bool FrontSTR::read( const std::vector<std::string>& msh_filenames, const std::vector<std::string>& res_filenames )
{
    std::string filenames = msh_filenames[0];
    for ( size_t i = 1; i < msh_filenames.size(); i++ ) filenames += ";" + msh_filenames[i];
    for ( size_t i = 0; i < res_filenames.size(); i++ ) filenames += ";" + res_filenames[i];

    BaseClass::setFilename( filenames );
    BaseClass::setSuccess( true );

    m_nmeshes = msh_filenames.size();
    m_nresults = res_filenames.size();

    if ( !this->allocate_data() )
    {
        kvsMessageError("Cannot allocate memory for member parameters.");
        this->delete_data();
        BaseClass::setSuccess( false );
        return false;
    }

    if ( m_nmeshes == 1 )
    {
        // Reading non divided mesh data.
        if ( !m_mesh_data[0].readData( msh_filenames[0] ) )
        {
            kvsMessageError("Cannot read %s.", msh_filenames[0].c_str() );
            BaseClass::setSuccess( false );
            return false;
        }
    }
    else
    {
        // Reading divided mesh data.
        for ( size_t i = 0; i < m_nmeshes; i++ )
        {
            if ( !m_mesh_data[i].readDividedData( msh_filenames[i] ) )
            {
                kvsMessageError("Cannot read %s.", msh_filenames[i].c_str() );
                BaseClass::setSuccess( false );
                return false;
            }
        }
    }

    if ( m_nresults == 1 )
    {
        // Reading non divided result data.
        if ( !m_result_data[0].readData( res_filenames[0] ) )
        {
            kvsMessageError("Cannot read %s.", res_filenames[0].c_str() );
            BaseClass::setSuccess( false );
            return false;
        }
    }
    else
    {
        // Reading divided result data.
        for ( size_t i = 0; i < m_nresults; i++ )
        {
            if ( !m_result_data[i].readDividedData( res_filenames[i] ) )
            {
                kvsMessageError("Cannot read %s.", res_filenames[i].c_str() );
                BaseClass::setSuccess( false );
                return false;
            }
        }
    }

    return true;
}

/*===========================================================================*/
/**
 *  @brief  Allocates the mesh and result data.
 *  @return true if the membery is allocated successfully
 */
/*===========================================================================*/
bool FrontSTR::allocate_data()
{
    m_mesh_data = new kvs::fstr::MeshData [ m_nmeshes ];
    if ( !m_mesh_data ) { return false; }

    m_result_data = new kvs::fstr::ResultData [ m_nresults ];
    if ( !m_result_data ) { if ( m_mesh_data ) delete [] m_mesh_data; return false; }

    return true;
}

/*===========================================================================*/
/**
 *  @brief  Deletes the mesh and result data.
 */
/*===========================================================================*/
void FrontSTR::delete_data()
{
    if ( m_mesh_data ) delete [] m_mesh_data;
    if ( m_result_data ) delete [] m_result_data;
}

/*===========================================================================*/
/**
 *  @brief  Writes FrontSTR data.
 *  @param  filename [in] filename
 *  @return true if the writing process is done successfully
 */
/*===========================================================================*/
bool FrontSTR::write( const std::string& filename )
{
    // This method has not been implemented yet.
    kvs::IgnoreUnusedVariable( filename );
    return false;
}

} // end of namespace kvs
