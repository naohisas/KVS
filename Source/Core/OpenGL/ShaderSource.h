/*****************************************************************************/
/**
 *  @file   ShaderSource.h
 *  @author Naohisa Sakamoto
 */
/*----------------------------------------------------------------------------
 *
 *  Copyright (c) Visualization Laboratory, Kyoto University.
 *  All rights reserved.
 *  See http://www.viz.media.kyoto-u.ac.jp/kvs/copyright/ for details.
 *
 *  $Id: ShaderSource.h 634 2010-10-13 07:04:05Z naohisa.sakamoto $
 */
/*****************************************************************************/
#ifndef KVS__SHADER_SOURCE_H_INCLUDE
#define KVS__SHADER_SOURCE_H_INCLUDE

#include <string>


namespace kvs
{

/*===========================================================================*/
/**
 *  Shader source class.
 */
/*===========================================================================*/
class ShaderSource
{
private:

    std::string m_code; ///< shader source code

public:

    static void AddSearchPath( const std::string& path );
    static void SetSearchPath( const std::string& path );
    static void ResetSearchPath();
    static void RemoveSearchPath();

public:

    ShaderSource( const std::string& source );

    const std::string& code() const;
    void setCode( const std::string& code );
    void clearCode();
    void define( const std::string& name );
    bool read( const std::string& filename );

private:

    bool read_code( const std::string& filename, const std::string& content, std::string& code );
};

} // end of namespace kvs

#endif // KVS__SHADER_SOURCE_H_INCLUDE
