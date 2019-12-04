/*****************************************************************************/
/**
 *  @file   ImageObjectTag.h
 */
/*----------------------------------------------------------------------------
 *
 *  Copyright (c) Visualization Laboratory, Kyoto University.
 *  All rights reserved.
 *  See http://www.viz.media.kyoto-u.ac.jp/kvs/copyright/ for details.
 *
 *  $Id: ImageObjectTag.h 1812 2014-09-11 07:34:35Z naohisa.sakamoto@gmail.com $
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
 *  @brief  Tag class for <ImageObject>
 */
/*===========================================================================*/
class ImageObjectTag : public kvs::kvsml::TagBase
{
public:
    typedef kvs::kvsml::TagBase BaseClass;

private:
    bool m_has_width; ///< flag to check whether 'width' is specified or not
    bool m_has_height; ///< flag to check whether 'height' is specified or not
    size_t m_width; ///< image width
    size_t m_height; ///< image height

public:
    ImageObjectTag();

    bool hasWidth() const { return m_has_height; }
    bool hasHeight() const { return m_has_height; }
    size_t width() const { return m_width; }
    size_t height() const { return m_height; }

    void setWidth( const size_t width ) { m_has_width = true; m_width = width; }
    void setHeight( const size_t height ) { m_has_height = true; m_height = height; }

    bool read( const kvs::XMLNode::SuperClass* parent );
    bool write( kvs::XMLNode::SuperClass* parent );
};

} // end of namespace kvsml

} // end of namespace kvs
