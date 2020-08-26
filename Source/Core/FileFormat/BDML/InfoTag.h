/*****************************************************************************/
/**
 *  @file   InfoTag.h
 *  @author Naohisa Sakamoto
 */
/*****************************************************************************/
#pragma once

#include "Tag.h"
#include <string>
#include <iostream>
#include <kvs/Indent>


namespace kvs
{

namespace bdml
{

/*===========================================================================*/
/**
 *  @brief  Info. tag class. Information about the BDML file.
 */
/*===========================================================================*/
class InfoTag : public Tag
{
private:
    std::string m_bdmlID; ///< unique ID
    std::string m_title; ///< title
    std::string m_license; ///< license information
    std::string m_release; ///< release data
    float m_version; ///< version of the file

public:
    InfoTag();

    void print( std::ostream& os, const kvs::Indent& indent = kvs::Indent(0) ) const;
    bool read( const Node* parent );
};

} // end of namespace bdml

} // end of namespace kvs
