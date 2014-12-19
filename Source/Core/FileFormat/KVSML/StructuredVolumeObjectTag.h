/*****************************************************************************/
/**
 *  @file   StructuredVolumeObjectTag.h
 */
/*----------------------------------------------------------------------------
 *
 *  Copyright (c) Visualization Laboratory, Kyoto University.
 *  All rights reserved.
 *  See http://www.viz.media.kyoto-u.ac.jp/kvs/copyright/ for details.
 *
 *  $Id: StructuredVolumeObjectTag.h 1344 2012-11-07 14:59:01Z s.yamada0808@gmail.com $
 */
/*****************************************************************************/
#ifndef KVS__KVSML__STRUCTURED_VOLUME_OBJECT_H_INCLUDE
#define KVS__KVSML__STRUCTURED_VOLUME_OBJECT_H_INCLUDE

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
 *  @brief  Tag class for <StructuredVolumeObject>
 */
/*===========================================================================*/
class StructuredVolumeObjectTag : public kvs::kvsml::TagBase
{
public:

    typedef kvs::kvsml::TagBase BaseClass;

private:

    bool m_has_grid_type; ///< flag to check whether 'grid_type' is specified or not
    std::string m_grid_type; ///< grid type
    bool m_has_resolution; ///< flag to check whether 'resolution' is specified or not
    kvs::Vector3ui m_resolution; ///< grid resolution

public:

    StructuredVolumeObjectTag();

public:

    bool hasGridType() const;
    const std::string& gridType() const;
    bool hasResolution() const;
    const kvs::Vector3ui& resolution() const;

public:

    void setGridType( const std::string& grid_type );
    void setResolution( const kvs::Vector3ui& resolution );

public:

    bool read( const kvs::XMLNode::SuperClass* parent );
    bool write( kvs::XMLNode::SuperClass* parent );
};

} // end of namespace kvsml

} // end of namespace kvs

#endif // KVS__KVSML__STRUCTURED_VOLUME_OBJECT_H_INCLUDE
