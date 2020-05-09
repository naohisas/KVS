/*****************************************************************************/
/**
 *  @file   OpacityMapTag.h
 */
/*----------------------------------------------------------------------------
 *
 *  Copyright (c) Visualization Laboratory, Kyoto University.
 *  All rights reserved.
 *  See http://www.viz.media.kyoto-u.ac.jp/kvs/copyright/ for details.
 *
 *  $Id: OpacityMapTag.h 1342 2012-11-07 13:47:24Z s.yamada0808@gmail.com $
 */
/*****************************************************************************/
#pragma once
#include <kvs/XMLNode>
#include <kvs/XMLElement>
#include "TagBase.h"


namespace kvs
{

namespace kvsml
{

/*===========================================================================*/
/**
 *  @brief  Tag class for <OpacityMap>
 */
/*===========================================================================*/
class OpacityMapTag : public kvs::kvsml::TagBase
{
public:
    typedef kvs::kvsml::TagBase BaseClass;

public:
    OpacityMapTag();

};


/*===========================================================================*/
/**
 *  @brief  <OpacityMapValue> tag class.
 */
/*===========================================================================*/
class OpacityMapValueTag : public kvs::kvsml::TagBase
{
public:
    typedef kvs::kvsml::TagBase BaseClass;

private:
    float m_scalar; ///< scalr value
    float m_opacity; ///< opacity value

public:
    OpacityMapValueTag();

    float scalar() const;
    float opacity() const;

    void setScalar( const float scalar );
    void setOpacity( const float opacity );

    bool read( const kvs::XMLNode::SuperClass* parent );
    bool read( const kvs::XMLElement::SuperClass* element );
    bool write( kvs::XMLNode::SuperClass* parent );
};

} // end of namespace kvsml

} // end of namespace kvs
