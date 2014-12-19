/*****************************************************************************/
/**
 *  @file   FrontSTR.h
 *  @author Guo Jiazhen
 */
/*----------------------------------------------------------------------------
 *
 *  Copyright (c) Visualization Laboratory, Kyoto University.
 *  All rights reserved.
 *  See http://www.viz.media.kyoto-u.ac.jp/kvs/copyright/ for details.
 *
 *  $Id: FrontSTR.h 1313 2012-09-19 08:19:01Z naohisa.sakamoto@gmail.com $
 */
/*****************************************************************************/
#ifndef KVS__FRONT_STR_H_INCLUDE
#define KVS__FRONT_STR_H_INCLUDE

#include <iostream>
#include <string>
#include <fstream>
#include <kvs/FileFormatBase>
#include <kvs/Type>
#include <kvs/ValueArray>
#include <kvs/Indent>
#include "MeshData.h"
#include "ResultData.h"


namespace kvs
{

/*===========================================================================*/
/**
 *  @brief  FrontSTR class.
 */
/*===========================================================================*/
class FrontSTR : public kvs::FileFormatBase
{
public:

    typedef kvs::FileFormatBase BaseClass;

private:

    size_t m_nmeshes; ///< number of mesh data
    size_t m_nresults; ///< number of result data
    kvs::fstr::MeshData* m_mesh_data; ///< mesh data
    kvs::fstr::ResultData* m_result_data; ///< result data

public:

    static bool CheckExtension( const std::string& filename );

public:

    FrontSTR();
    FrontSTR( const std::string& filenames );
    FrontSTR( const std::string& msh_filename, const std::string& res_filename );
    FrontSTR( const std::vector<std::string>& msh_filenames, const std::vector<std::string>& res_filenames );
    virtual ~FrontSTR();

    size_t numberOfMeshData() const;
    size_t numberOfResultData() const;
    const kvs::fstr::MeshData& meshData( const size_t index = 0 ) const;
    const kvs::fstr::ResultData& resultData( const size_t index = 0 ) const;

    void print( std::ostream& os, const kvs::Indent& indent = kvs::Indent(0) ) const;
    bool read( const std::string& filenames );
    bool read( const std::string& msh_filename, const std::string& res_filename );
    bool read( const std::vector<std::string>& msh_filenames, const std::vector<std::string>& res_filenames );

private:

    bool allocate_data();
    void delete_data();
    bool write( const std::string& filename );
};

} // end of namespace kvs

#endif // KVS__FRONT_STR_H_INCLUDE
