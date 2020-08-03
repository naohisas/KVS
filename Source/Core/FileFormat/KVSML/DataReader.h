/*****************************************************************************/
/**
 *  @file   DataReader.h
 *  @author Naohisa Sakamoto
 */
/*****************************************************************************/
#pragma once
#include <kvs/ValueArray>
#include <kvs/XMLNode>
#include <kvs/Type>


namespace kvs
{

namespace kvsml
{

bool ReadCoordData(
    const kvs::XMLNode::SuperClass* parent,
    const size_t ncoords,
    kvs::ValueArray<kvs::Real32>* coords );

bool ReadColorData(
    const kvs::XMLNode::SuperClass* parent,
    const size_t ncolors,
    kvs::ValueArray<kvs::UInt8>* colors );

bool ReadNormalData(
    const kvs::XMLNode::SuperClass* parent,
    const size_t nnormals,
    kvs::ValueArray<kvs::Real32>* normals );

bool ReadSizeData(
    const kvs::XMLNode::SuperClass* parent,
    const size_t nsizes,
    kvs::ValueArray<kvs::Real32>* sizes );

bool ReadConnectionData(
    const kvs::XMLNode::SuperClass* parent,
    const size_t nconnections,
    kvs::ValueArray<kvs::UInt32>* connections );

bool ReadOpacityData(
    const kvs::XMLNode::SuperClass* parent,
    const size_t nopacities,
    kvs::ValueArray<kvs::UInt8>* opacities );

} // end of namespace kvsml

} // end of namespace kvs
