/*****************************************************************************/
/**
 *  @file   ColorMapTag.h
 *  @author Naohisa Sakamoto
 */
/*****************************************************************************/
#pragma once
#include <kvs/XMLNode>
#include <kvs/XMLElement>
#include <kvs/RGBColor>
#include "TagBase.h"


namespace kvs
{

namespace kvsml
{

/*===========================================================================*/
/**
 *  @brief  Tag class for <ColorMap>
 */
/*===========================================================================*/
class ColorMapTag : public kvs::kvsml::TagBase
{
public:
    using BaseClass = kvs::kvsml::TagBase;

public:
    ColorMapTag();
};

/*===========================================================================*/
/**
 *  @brief  <ColorMapValue> tag class.
 */
/*===========================================================================*/
class ColorMapValueTag : public kvs::kvsml::TagBase
{
public:
    using BaseClass = kvs::kvsml::TagBase;

private:
    float m_scalar; ///< scalr value
    kvs::RGBColor m_color; ///< color value

public:
    ColorMapValueTag();

    float scalar() const { return m_scalar; }
    const kvs::RGBColor& color() const { return m_color; }
    void setScalar( const float scalar ) { m_scalar = scalar; }
    void setColor( const kvs::RGBColor& color ) { m_color = color; }

    bool read( const kvs::XMLNode::SuperClass* parent );
    bool read( const kvs::XMLElement::SuperClass* element );
    bool write( kvs::XMLNode::SuperClass* parent );
};

} // end of namespace kvsml

} // end of namespace kvs
