/****************************************************************************/
/**
 *  @file   VCProject.h
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
class VCProject : public kvs::Program
{
private:
    std::string m_project_name; ///< project name
public:
    VCProject( const std::string& project_name ): m_project_name( project_name ) {}
    int exec();
};

} // end of namespace kvsmake
