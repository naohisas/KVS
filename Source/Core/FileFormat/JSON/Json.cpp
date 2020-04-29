/*****************************************************************************/
/**
 *  @file   Json.cpp
 *  @author Naohisa Sakamoto
 */
/*****************************************************************************/
#include "Json.h"
#include <iostream>
#include <fstream>
#include <sstream>
#include <kvs/Message>


namespace kvs
{

bool Json::read( const std::string& filename )
{
    BaseClass::setFilename( filename );
    BaseClass::setSuccess( true );

    std::ifstream ifs( filename, std::ios::in );
    if ( !ifs.is_open() )
    {
        kvsMessageError() << "Cannot open " << filename << "." << std::endl;
        BaseClass::setSuccess( false );
        return false;
    }

    std::stringstream json;
    json << ifs.rdbuf();
    ifs.close();

    picojson::value value;
    json >> m_value;
    const std::string error_message = picojson::get_last_error();
    if ( !error_message.empty() )
    {
        kvsMessageError() << error_message << std::endl;
        BaseClass::setSuccess( false );
        return false;
    }

    return true;
}

bool Json::write( const std::string& filename )
{
    BaseClass::setFilename( filename );
    BaseClass::setSuccess( true );

    std::ofstream ofs( filename.c_str(), std::ios::out );
    if ( !ofs.is_open() )
    {
        kvsMessageError() << "Cannot open " << filename << "." << std::endl;
        BaseClass::setSuccess( false );
        return false;
    }

    ofs << m_value.serialize( true );
    ofs.close();

    return true;
}

} // end of namespace kvs
