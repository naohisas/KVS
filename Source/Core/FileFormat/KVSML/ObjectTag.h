/*****************************************************************************/
/**
 *  @file   ObjectTag.h
 *  @author Naohisa Sakamoto
 */
/*----------------------------------------------------------------------------
 *
 *  Copyright (c) Visualization Laboratory, Kyoto University.
 *  All rights reserved.
 *  See http://www.viz.media.kyoto-u.ac.jp/kvs/copyright/ for details.
 *
 *  $Id: ObjectTag.h 1812 2014-09-11 07:34:35Z naohisa.sakamoto@gmail.com $
 */
/*****************************************************************************/
#pragma once
#include <string>
#include <kvs/Vector3>
#include <kvs/XMLDocument>
#include <kvs/XMLNode>
#include "TagBase.h"


namespace kvs
{

namespace kvsml
{

/*===========================================================================*/
/**
 *  @brief  Tag class for <Object>.
 */
/*===========================================================================*/
class ObjectTag : public kvs::kvsml::TagBase
{
public:
    typedef kvs::kvsml::TagBase BaseClass;

private:
    bool m_has_type; ///< flag to check whether 'type' is specified or not
    bool m_has_external_coord; ///< flag to check whether 'external_coord' is specified or not
    bool m_has_object_coord; ///< flag to check whether 'object_coord' is specified or not
    std::string m_type; ///< object type
    kvs::Vec3 m_min_external_coord; ///< min. external coordinate
    kvs::Vec3 m_max_external_coord; ///< max. external coordinate
    kvs::Vec3 m_min_object_coord; ///< min. object coordinate
    kvs::Vec3 m_max_object_coord; ///< max. object coordinate

public:
    ObjectTag();

    bool hasType() const;
    bool hasExternalCoord() const;
    bool hasObjectCoord() const;
    const std::string& type() const;
    const kvs::Vec3& minExternalCoord() const;
    const kvs::Vec3& maxExternalCoord() const;
    const kvs::Vec3& minObjectCoord() const;
    const kvs::Vec3& maxObjectCoord() const;

    void setType( const std::string& type );
    void setMinMaxExternalCoords( const kvs::Vec3& min_coord, const kvs::Vec3& max_coord );
    void setMinMaxObjectCoords( const kvs::Vec3& min_coord, const kvs::Vec3& max_coord );

    bool read( const kvs::XMLNode::SuperClass* parent );
    bool write( kvs::XMLNode::SuperClass* parent );
};

} // end of namespace kvsml

} // end of namespace kvs
