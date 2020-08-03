/*****************************************************************************/
/**
 *  @file   UnstructuredVolumeObjectTag.h
 *  @author Naohisa Sakamoto
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
 *  @brief  Tag class for <UnstructuredVolumeObject>
 */
/*===========================================================================*/
class UnstructuredVolumeObjectTag : public kvs::kvsml::TagBase
{
public:
    typedef kvs::kvsml::TagBase BaseClass;

private:
    kvs::kvsml::TagAttribute<std::string> m_cell_type; ///< cell type

public:
    UnstructuredVolumeObjectTag();

    bool hasCellType() const { return m_cell_type.hasValue(); }
    const std::string& cellType() const { return m_cell_type; }
    void setCellType( const std::string& cell_type ) { m_cell_type = cell_type; }

    bool read( const kvs::XMLNode::SuperClass* parent );
    bool write( kvs::XMLNode::SuperClass* parent );
};

} // end of namespace kvsml

} // end of namespace kvs
