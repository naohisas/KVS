/****************************************************************************/
/**
 *  @file   VCProjectCUDA.h
 *  @author Naohisa Sakamoto
 */
/****************************************************************************/
#pragma once
#include <string>
#include <kvs/Program>


namespace kvsmake
{

/*===========================================================================*/
/**
 *  @brief  MSVC project file generator.
 */
/*===========================================================================*/
class VCProjectCUDA : public kvs::Program
{
private:
    std::string m_project_name; ///< project name
    int exec( int argc, char** argv );
public:
    VCProjectCUDA( const std::string& project_name ): m_project_name( project_name ) {}
};

} // end of namespace kvsmake
