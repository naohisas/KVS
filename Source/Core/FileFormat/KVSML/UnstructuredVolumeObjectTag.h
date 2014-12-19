/*****************************************************************************/
/**
 *  @file   UnstructuredVolumeObjectTag.h
 */
/*----------------------------------------------------------------------------
 *
 *  Copyright (c) Visualization Laboratory, Kyoto University.
 *  All rights reserved.
 *  See http://www.viz.media.kyoto-u.ac.jp/kvs/copyright/ for details.
 *
 *  $Id: UnstructuredVolumeObjectTag.h 1344 2012-11-07 14:59:01Z s.yamada0808@gmail.com $
 */
/*****************************************************************************/
#ifndef KVS__KVSML__UNSTRUCTURED_VOLUME_OBJECT_H_INCLUDE
#define KVS__KVSML__UNSTRUCTURED_VOLUME_OBJECT_H_INCLUDE

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
 *  @brief  Tag class for <UnstructuredVolumeObject>
 */
/*===========================================================================*/
class UnstructuredVolumeObjectTag : public kvs::kvsml::TagBase
{
public:

    typedef kvs::kvsml::TagBase BaseClass;

private:

    bool m_has_cell_type; ///< flag to check whether 'cell_type' is specified or not
    std::string m_cell_type; ///< cell type

public:

    UnstructuredVolumeObjectTag();

public:

    bool hasCellType() const;
    const std::string& cellType() const;

public:

    void setCellType( const std::string& cell_type );

public:

    bool read( const kvs::XMLNode::SuperClass* parent );
    bool write( kvs::XMLNode::SuperClass* parent );
};

} // end of namespace kvsml

} // end of namespace kvs

#endif // KVS__KVSML__UNSTRUCTURED_VOLUME_OBJECT_H_INCLUDE
