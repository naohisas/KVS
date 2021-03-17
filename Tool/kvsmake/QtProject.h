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
public:
    QtProject( const std::string& project_name ): m_project_name( project_name ) {}
    int exec();
};

} // end of namespace kvsmake
