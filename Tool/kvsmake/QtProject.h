/****************************************************************************/
/**
 *  @file   QtProject.h
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
 *  @brief  Qt project file generator class.
 */
/*===========================================================================*/
class QtProject : public kvs::Program
{
private:
    std::string m_project_name; ///< project name
    int exec( int argc, char** argv );
public:
    QtProject( const std::string& project_name ): m_project_name( project_name ) {}
};

} // end of namespace kvsmake
