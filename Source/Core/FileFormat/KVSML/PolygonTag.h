/*****************************************************************************/
/**
 *  @file   PolygonTag.h
 *  @author Naohisa Sakamoto
 */
/*****************************************************************************/
#pragma once
#include <kvs/XMLNode>
#include "TagBase.h"


namespace kvs
{

namespace kvsml
{

/*===========================================================================*/
/**
 *  @brief  Tag class for <Polygon>
 */
/*===========================================================================*/
class PolygonTag : public kvs::kvsml::TagBase
{
public:
    typedef kvs::kvsml::TagBase BaseClass;

private:
    bool m_has_npolygons; ///< flag to check whether 'npolygons' is specified or not
    size_t m_npolygons; ///< number of polygons

public:
    PolygonTag();

    bool hasNPolygons() const;
    size_t npolygons() const;
    void setNPolygons( const size_t npolygons );

    bool read( const kvs::XMLNode::SuperClass* parent );
    bool write( kvs::XMLNode::SuperClass* parent );
};

} // end of namespace kvsml

} // end of namespace kvs
