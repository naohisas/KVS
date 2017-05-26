/*****************************************************************************/
/**
 *  @file   Tag.h
 *  @author Naohisa Sakamoto
 */
/*----------------------------------------------------------------------------
 *
 *  Copyright (c) Visualization Laboratory, Kyoto University.
 *  All rights reserved.
 *  See http://www.viz.media.kyoto-u.ac.jp/kvs/copyright/ for details.
 *
 *  $Id$
 */
/*****************************************************************************/
#pragma once

#include "XML.h"
#include <string>


namespace kvs
{

namespace bdml
{

/*===========================================================================*/
/**
 *  @brief  Tag class.
 */
/*===========================================================================*/
class Tag
{
private:
    std::string m_name; ///< tag name
    Node* m_node; ///< pointer to the node

public:
    explicit Tag( const std::string& name );
    Tag( const Node* parent, const std::string& name );

    const std::string& name() const { return m_name; }
    Node* node() { return m_node; }
    const Node* node() const { return m_node; }
    bool read( const Node* parent );
    bool exists( const Node* parent ) const;
    bool exists() const;

private:
    Tag();
};

} // end of namespace bdml

} // end of namespace kvs
