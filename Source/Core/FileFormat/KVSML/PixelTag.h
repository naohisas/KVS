/*****************************************************************************/
/**
 *  @file   PixelTag.h
 *  @author Naohisa Sakamoto
 */
/*****************************************************************************/
#pragma once
#include <string>
#include <kvs/XMLNode>
#include <kvs/Vector3>
#include "TagBase.h"


namespace kvs
{

namespace kvsml
{

/*===========================================================================*/
/**
 *  @brief  Tag class for <Pixel>
 */
/*===========================================================================*/
class PixelTag : public kvs::kvsml::TagBase
{
public:
    typedef kvs::kvsml::TagBase BaseClass;

private:
    bool m_has_type; ///< flag to check whether 'type' is specified or not
    std::string m_type; ///< pixel type

public:
    PixelTag();

    bool hasType() const;
    const std::string& type() const;
    void setType( const std::string& type );

    bool read( const kvs::XMLNode::SuperClass* parent );
    bool write( kvs::XMLNode::SuperClass* parent );
};

} // end of namespace kvsml

} // end of namespace kvs
