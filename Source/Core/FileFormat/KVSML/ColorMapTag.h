/*****************************************************************************/
/**
 *  @file   ColorMapTag.h
 */
/*----------------------------------------------------------------------------
 *
 *  Copyright (c) Visualization Laboratory, Kyoto University.
 *  All rights reserved.
 *  See http://www.viz.media.kyoto-u.ac.jp/kvs/copyright/ for details.
 *
 *  $Id: ColorMapTag.h 1812 2014-09-11 07:34:35Z naohisa.sakamoto@gmail.com $
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
    typedef kvs::kvsml::TagBase BaseClass;

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
    typedef kvs::kvsml::TagBase BaseClass;

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
