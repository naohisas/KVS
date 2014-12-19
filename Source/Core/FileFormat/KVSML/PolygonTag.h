/*****************************************************************************/
/**
 *  @file   PolygonTag.h
 */
/*----------------------------------------------------------------------------
 *
 *  Copyright (c) Visualization Laboratory, Kyoto University.
 *  All rights reserved.
 *  See http://www.viz.media.kyoto-u.ac.jp/kvs/copyright/ for details.
 *
 *  $Id: PolygonTag.h 1344 2012-11-07 14:59:01Z s.yamada0808@gmail.com $
 */
/*****************************************************************************/
#ifndef KVS__KVSML__POLYGON_TAG_H_INCLUDE
#define KVS__KVSML__POLYGON_TAG_H_INCLUDE

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

public:

    bool hasNPolygons() const;
    size_t npolygons() const;

public:

    void setNPolygons( const size_t npolygons );

public:

    bool read( const kvs::XMLNode::SuperClass* parent );
    bool write( kvs::XMLNode::SuperClass* parent );
};

} // end of namespace kvsml

} // end of namespace kvs

#endif // KVS__KVSML__POLYGONS_TAG_H_INCLUDE
