/*****************************************************************************/
/**
 *  @file   ShaderSource.h
 *  @author Naohisa Sakamoto
 */
/*****************************************************************************/
#pragma once
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
    std::string m_code{}; ///< shader source code

public:
    static void AddSearchPath( const std::string& path );
    static void SetSearchPath( const std::string& path );
    static void ResetSearchPath();
    static void RemoveSearchPath();

public:
    ShaderSource( const std::string& source );

    const std::string& code() const { return m_code; }
    void setCode( const std::string& code ) { m_code = code; }
    void clearCode() { m_code.erase(); }
    void define( const std::string& name );
    bool read( const std::string& filename );

private:
    bool read_code( const std::string& filename, const std::string& content, std::string& code );
};

} // end of namespace kvs
