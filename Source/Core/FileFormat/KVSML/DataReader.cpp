/*****************************************************************************/
/**
 *  @file   DataReader.cpp
 */
/*----------------------------------------------------------------------------
 *
 *  Copyright (c) Visualization Laboratory, Kyoto University.
 *  All rights reserved.
 *  See http://www.viz.media.kyoto-u.ac.jp/kvs/copyright/ for details.
 *
 *  $Id: DataReader.cpp 1303 2012-09-14 11:26:37Z naohisa.sakamoto@gmail.com $
 */
/*****************************************************************************/
#include "DataReader.h"
#include "TagBase.h"
#include "CoordTag.h"
#include "ColorTag.h"
#include "NormalTag.h"
#include "SizeTag.h"
#include "ConnectionTag.h"
#include "OpacityTag.h"
#include "DataArrayTag.h"
#include "DataValueTag.h"
#include <kvs/Message>
#include <kvs/ValueArray>
#include <kvs/XMLNode>


namespace kvs
{

namespace kvsml
{

/*===========================================================================*/
/**
 *  @brief  Reads coordinate data from <Coord>.
 *  @param  parent  [in] pointer to the parent node
 *  @param  ncoords [in] number of coordinates
 *  @param  coords [out] pointer to the coordinate value array
 *  @return true, if the reading process is done successfully
 */
/*===========================================================================*/
bool ReadCoordData(
    const kvs::XMLNode::SuperClass* parent,
    const size_t ncoords,
    kvs::ValueArray<kvs::Real32>* coords )
{
    // <Coord>
    kvs::kvsml::CoordTag coord_tag;
    if ( coord_tag.isExisted( parent ) )
    {
        if ( !coord_tag.read( parent ) )
        {
            kvsMessageError( "Cannot read <%s>.", coord_tag.name().c_str() );
            return false;
        }

        // <DataArray>
        const size_t dimension = 3;
        const size_t nelements = ncoords * dimension;
        kvs::kvsml::DataArrayTag data_tag;
        if ( !data_tag.read( coord_tag.node(), nelements, coords ) )
        {
            kvsMessageError( "Cannot read <%s> for <%s>.",
                             data_tag.name().c_str(),
                             coord_tag.name().c_str() );
            return false;
        }
    }

    return true;
}

/*===========================================================================*/
/**
 *  @brief  Reads color data from <Color>.
 *  @param  parent  [in] pointer to the parent node
 *  @param  ncoords [in] number of colors
 *  @param  colors [out] pointer to the color value array
 *  @return true, if the reading process is done successfully
 */
/*===========================================================================*/
bool ReadColorData(
    const kvs::XMLNode::SuperClass* parent,
    const size_t ncolors,
    kvs::ValueArray<kvs::UInt8>* colors )
{
    // <Color>
    kvs::kvsml::ColorTag color_tag;
    if ( color_tag.isExisted( parent ) )
    {
        if ( !color_tag.read( parent ) )
        {
            kvsMessageError( "Cannot read <%s>.", color_tag.name().c_str() );
            return false;
        }

        // <DataValue>
        if ( kvs::XMLNode::FindChildNode( color_tag.node(), "DataValue" ) )
        {
            // In this case, 'ncolors' is equal to 1.
            const size_t nchannels = 3; // RGB
            const size_t nelements = nchannels; // = 1 * nchannels
            kvs::kvsml::DataValueTag data_tag;
            if ( !data_tag.read( color_tag.node(), nelements, colors ) )
            {
                kvsMessageError( "Cannot read <%s> for <%s>.",
                                 data_tag.name().c_str(),
                                 color_tag.name().c_str() );
                return false;
            }
        }
        // <DataArray>
        else
        {
            const size_t nchannels = 3; // RGB
            const size_t nelements = ncolors * nchannels;
            kvs::kvsml::DataArrayTag data_tag;
            if ( !data_tag.read( color_tag.node(), nelements, colors ) )
            {
                kvsMessageError( "Cannot read <%s> for <%s>.",
                                 data_tag.name().c_str(),
                                 color_tag.name().c_str() );
                return false;
            }
        }
    }

    return true;
}

/*===========================================================================*/
/**
 *  @brief  Reads normal data from <Color>.
 *  @param  parent  [in] pointer to the parent node
 *  @param  ncoords [in] number of normals
 *  @param  normals [out] pointer to the normal value array
 *  @return true, if the reading process is done successfully
 */
/*===========================================================================*/
bool ReadNormalData(
    const kvs::XMLNode::SuperClass* parent,
    const size_t nnormals,
    kvs::ValueArray<kvs::Real32>* normals )
{
    // <Normal>
    kvs::kvsml::NormalTag normal_tag;
    if ( normal_tag.isExisted( parent ) )
    {
        if ( !normal_tag.read( parent ) )
        {
            kvsMessageError( "Cannot read <%s>.", normal_tag.name().c_str() );
            return false;
        }

        // <DataValue>
        if ( kvs::XMLNode::FindChildNode( normal_tag.node(), "DataValue" ) )
        {
            // In this case, 'nnormals' is equal to 1.
            const size_t dimension = 3;
            const size_t nelements = dimension; // = 1 * dimension
            kvs::kvsml::DataValueTag data_tag;
            if ( !data_tag.read( normal_tag.node(), nelements, normals ) )
            {
                kvsMessageError( "Cannot read <%s> for <%s>.",
                                 data_tag.name().c_str(),
                                 normal_tag.name().c_str() );
                return false;
            }
        }

        // <DataArray>
        else
        {
            const size_t dimension = 3;
            const size_t nelements = nnormals * dimension;
            kvs::kvsml::DataArrayTag data_tag;
            if ( !data_tag.read( normal_tag.node(), nelements, normals ) )
            {
                kvsMessageError( "Cannot read <%s> for <%s>.",
                                 data_tag.name().c_str(),
                                 normal_tag.name().c_str() );
                return false;
            }
        }
    }

    return true;
}

/*===========================================================================*/
/**
 *  @brief  Reads size data from <Size>.
 *  @param  parent  [in] pointer to the parent node
 *  @param  ncoords [in] number of sizes
 *  @param  sizes [out] pointer to the size value array
 *  @return true, if the reading process is done successfully
 */
/*===========================================================================*/
bool ReadSizeData(
    const kvs::XMLNode::SuperClass* parent,
    const size_t nsizes,
    kvs::ValueArray<kvs::Real32>* sizes )
{
    // <Size>
    kvs::kvsml::SizeTag size_tag;
    if ( size_tag.isExisted( parent ) )
    {
        if ( !size_tag.read( parent ) )
        {
            kvsMessageError( "Cannot read <%s>.", size_tag.name().c_str() );
            return false;
        }

        // <DataValue>
        if ( kvs::XMLNode::FindChildNode( size_tag.node(), "DataValue" ) )
        {
            const size_t nelements = 1;
            kvs::kvsml::DataValueTag data_tag;
            if ( !data_tag.read( size_tag.node(), nelements, sizes ) )
            {
                kvsMessageError( "Cannot read <%s> for <%s>.",
                                 data_tag.name().c_str(),
                                 size_tag.name().c_str() );
                return false;
            }
        }
        // <DataArray>
        else
        {
            const size_t nelements = nsizes;
            kvs::kvsml::DataArrayTag data_tag;
            if ( !data_tag.read( size_tag.node(), nelements, sizes ) )
            {
                kvsMessageError( "Cannot read <%s> for <%s>.",
                                 data_tag.name().c_str(),
                                 size_tag.name().c_str() );
                return false;
            }
        }
    }

    return true;
}

/*===========================================================================*/
/**
 *  @brief  Reads connection data from <Connection>.
 *  @param  parent  [in] pointer to the parent node
 *  @param  ncoords [in] number of connections
 *  @param  connections [out] pointer to the connection value array
 *  @return true, if the reading process is done successfully
 */
/*===========================================================================*/
bool ReadConnectionData(
    const kvs::XMLNode::SuperClass* parent,
    const size_t nconnections,
    kvs::ValueArray<kvs::UInt32>* connections )
{
    // <Connection>
    kvs::kvsml::ConnectionTag connection_tag;
    if ( connection_tag.isExisted( parent ) )
    {
        if ( !connection_tag.read( parent ) )
        {
            kvsMessageError( "Cannot read <%s>.", connection_tag.name().c_str() );
            return false;
        }

        // <DataArray>
        const size_t nelements = nconnections;
        kvs::kvsml::DataArrayTag data_tag;
        if ( !data_tag.read( connection_tag.node(), nelements, connections ) )
        {
            kvsMessageError( "Cannot read <%s> for <%s>.",
                             data_tag.name().c_str(),
                             connection_tag.name().c_str() );
            return false;
        }
    }

    return true;
}

/*===========================================================================*/
/**
 *  @brief  Reads opacity data from <Opacity>.
 *  @param  parent  [in] pointer to the parent node
 *  @param  ncoords [in] number of opacities
 *  @param  opacities [out] pointer to the opacity value array
 *  @return true, if the reading process is done successfully
 */
/*===========================================================================*/
bool ReadOpacityData(
    const kvs::XMLNode::SuperClass* parent,
    const size_t nopacities,
    kvs::ValueArray<kvs::UInt8>* opacities )
{
    // <Opacity>
    kvs::kvsml::OpacityTag opacity_tag;
    if ( opacity_tag.isExisted( parent ) )
    {
        if ( !opacity_tag.read( parent ) )
        {
            kvsMessageError( "Cannot read <%s>.", opacity_tag.name().c_str() );
            return false;
        }

        // <DataValue>
        if ( kvs::XMLNode::FindChildNode( opacity_tag.node(), "DataValue" ) )
        {
            const size_t nelements = 1;
            kvs::kvsml::DataValueTag data_tag;
            if ( !data_tag.read( opacity_tag.node(), nelements, opacities ) )
            {
                kvsMessageError( "Cannot read <%s> for <%s>.",
                                 data_tag.name().c_str(),
                                 opacity_tag.name().c_str() );
                return false;
            }
        }
        // <DataArray>
        else
        {
            const size_t nelements = nopacities;
            kvs::kvsml::DataArrayTag data_tag;
            if ( !data_tag.read( opacity_tag.node(), nelements, opacities ) )
            {
                kvsMessageError( "Cannot read <%s> for <%s>.",
                                 data_tag.name().c_str(),
                                 opacity_tag.name().c_str() );
                return false;
            }
        }
    }

    return true;
}

} // end of namespace kvsml

} // end of namespace kvs
