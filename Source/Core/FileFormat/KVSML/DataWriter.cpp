/*****************************************************************************/
/**
 *  @file   DataWriter.cpp
 */
/*----------------------------------------------------------------------------
 *
 *  Copyright (c) Visualization Laboratory, Kyoto University.
 *  All rights reserved.
 *  See http://www.viz.media.kyoto-u.ac.jp/kvs/copyright/ for details.
 *
 *  $Id: DataWriter.cpp 1303 2012-09-14 11:26:37Z naohisa.sakamoto@gmail.com $
 */
/*****************************************************************************/
#include "DataWriter.h"
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
 *  @brief  Writes coordinate data to <Coord>.
 *  @param  parent [out] pointer to the parent node
 *  @param  writing_type [in] writing data type
 *  @param  filename [in] filename
 *  @param  coords [in] coordinate value array
 *  @return true, if the writing process is done successfully
 */
/*===========================================================================*/
bool WriteCoordData(
    kvs::XMLNode::SuperClass* parent,
    const kvs::kvsml::WritingDataType writing_type,
    const std::string& filename,
    const kvs::ValueArray<kvs::Real32>& coords )
{
    // <Coord>
    if ( coords.size() > 0 )
    {
        kvs::kvsml::CoordTag coord_tag;
        if ( !coord_tag.write( parent ) )
        {
            kvsMessageError( "Cannot write <%s>.", coord_tag.name().c_str() );
            return false;
        }

        // <DataArray>
        kvs::kvsml::DataArrayTag data_tag;
        if ( writing_type == kvs::kvsml::ExternalAscii )
        {
            data_tag.setFile( kvs::kvsml::DataArray::GetDataFilename( filename, "coord" ) );
            data_tag.setFormat( "ascii" );
        }
        else if ( writing_type == kvs::kvsml::ExternalBinary )
        {
            data_tag.setFile( kvs::kvsml::DataArray::GetDataFilename( filename, "coord" ) );
            data_tag.setFormat( "binary" );
        }

        const std::string pathname = kvs::File( filename ).pathName();
        if ( !data_tag.write( coord_tag.node(), coords, pathname ) )
        {
            kvsMessageError( "Cannot write <%s> for <%s>.",
                             data_tag.name().c_str(),
                             coord_tag.name().c_str() );
            return false;
        }
    }

    return true;
}

/*===========================================================================*/
/**
 *  @brief  Writes color data to <Color>.
 *  @param  parent [out] pointer to the parent node
 *  @param  writing_type [in] writing data type
 *  @param  filename [in] filename
 *  @param  colors [in] color value array
 *  @return true, if the writing process is done successfully
 */
/*===========================================================================*/
bool WriteColorData(
    kvs::XMLNode::SuperClass* parent,
    const kvs::kvsml::WritingDataType writing_type,
    const std::string& filename,
    const kvs::ValueArray<kvs::UInt8>& colors )
{
    // <Color>
    if ( colors.size() > 0 )
    {
        kvs::kvsml::ColorTag color_tag;
        if ( !color_tag.write( parent ) )
        {
            kvsMessageError( "Cannot write <%s>.", color_tag.name().c_str() );
            return false;
        }

        // <DataValue>
        if ( colors.size() == 3 )
        {
            kvs::kvsml::DataValueTag data_tag;
            if ( !data_tag.write( color_tag.node(), colors ) )
            {
                kvsMessageError( "Cannot write <%s> for <%s>.",
                                 data_tag.name().c_str(),
                                 color_tag.name().c_str() );
                return false;
            }
        }
        // <DataArray>
        else
        {
            kvs::kvsml::DataArrayTag data_tag;
            if ( writing_type == kvs::kvsml::ExternalAscii )
            {
                data_tag.setFile( kvs::kvsml::DataArray::GetDataFilename( filename, "color" ) );
                data_tag.setFormat( "ascii" );
            }
            else if ( writing_type == kvs::kvsml::ExternalBinary )
            {
                data_tag.setFile( kvs::kvsml::DataArray::GetDataFilename( filename, "color" ) );
                data_tag.setFormat( "binary" );
            }

            const std::string pathname = kvs::File( filename ).pathName();
            if ( !data_tag.write( color_tag.node(), colors, pathname ) )
            {
                kvsMessageError( "Cannot write <%s> for <%s>.",
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
 *  @brief  Writes normal data to <Coord>.
 *  @param  parent [out] pointer to the parent node
 *  @param  writing_type [in] writing data type
 *  @param  filename [in] filename
 *  @param  normals [in] normal value array
 *  @return true, if the writing process is done successfully
 */
/*===========================================================================*/
bool WriteNormalData(
    kvs::XMLNode::SuperClass* parent,
    const kvs::kvsml::WritingDataType writing_type,
    const std::string& filename,
    const kvs::ValueArray<kvs::Real32>& normals )
{
    // <Normal>
    if ( normals.size() > 0 )
    {
        kvs::kvsml::NormalTag normal_tag;
        if ( !normal_tag.write( parent ) )
        {
            kvsMessageError( "Cannot write <%s>.", normal_tag.name().c_str() );
            return false;
        }

        // <DataValue>
        if ( normals.size() == 3 )
        {
            kvs::kvsml::DataValueTag data_tag;
            if ( !data_tag.write( normal_tag.node(), normals ) )
            {
                kvsMessageError( "Cannot write <%s> for <%s>.",
                                 data_tag.name().c_str(),
                                 normal_tag.name().c_str() );
                return false;
            }
        }

        // <DataArray>
        else
        {
            kvs::kvsml::DataArrayTag data_tag;
            if ( writing_type == kvs::kvsml::ExternalAscii )
            {
                data_tag.setFile( kvs::kvsml::DataArray::GetDataFilename( filename, "normal" ) );
                data_tag.setFormat( "ascii" );
            }
            else if ( writing_type == kvs::kvsml::ExternalBinary )
            {
                data_tag.setFile( kvs::kvsml::DataArray::GetDataFilename( filename, "normal" ) );
                data_tag.setFormat( "binary" );
            }

            const std::string pathname = kvs::File( filename ).pathName();
            if ( !data_tag.write( normal_tag.node(), normals, pathname ) )
            {
                kvsMessageError( "Cannot write <%s> for <%s>.",
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
 *  @brief  Writes size data to <Coord>.
 *  @param  parent [out] pointer to the parent node
 *  @param  writing_type [in] writing data type
 *  @param  filename [in] filename
 *  @param  sizes [in] size value array
 *  @return true, if the writing process is done successfully
 */
/*===========================================================================*/
bool WriteSizeData(
    kvs::XMLNode::SuperClass* parent,
    const kvs::kvsml::WritingDataType writing_type,
    const std::string& filename,
    const kvs::ValueArray<kvs::Real32>& sizes )
{
    // <Size>
    if ( sizes.size() > 0 )
    {
        kvs::kvsml::SizeTag size_tag;
        if ( !size_tag.write( parent ) )
        {
            kvsMessageError( "Cannot write <%s>.", size_tag.name().c_str() );
            return false;
        }

        // <DataValue>
        if ( sizes.size() == 1 )
        {
            kvs::kvsml::DataValueTag data_tag;
            if ( !data_tag.write( size_tag.node(), sizes ) )
            {
                kvsMessageError( "Cannot write <%s> for <%s>.",
                                 data_tag.name().c_str(),
                                 size_tag.name().c_str() );
                return false;
            }
        }
        // <DataArray>
        else
        {
            kvs::kvsml::DataArrayTag data_tag;
            if ( writing_type == kvs::kvsml::ExternalAscii )
            {
                data_tag.setFile( kvs::kvsml::DataArray::GetDataFilename( filename, "size" ) );
                data_tag.setFormat( "ascii" );
            }
            else if ( writing_type == kvs::kvsml::ExternalBinary )
            {
                data_tag.setFile( kvs::kvsml::DataArray::GetDataFilename( filename, "size" ) );
                data_tag.setFormat( "binary" );
            }

            const std::string pathname = kvs::File( filename ).pathName();
            if ( !data_tag.write( size_tag.node(), sizes, pathname ) )
            {
                kvsMessageError( "Cannot write <%s> for <%s>.",
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
 *  @brief  Writes connection data to <Coord>.
 *  @param  parent [out] pointer to the parent node
 *  @param  writing_type [in] writing data type
 *  @param  filename [in] filename
 *  @param  connection [in] connection value array
 *  @return true, if the writing process is done successfully
 */
/*===========================================================================*/
bool WriteConnectionData(
    kvs::XMLNode::SuperClass* parent,
    const kvs::kvsml::WritingDataType writing_type,
    const std::string& filename,
    const kvs::ValueArray<kvs::UInt32>& connections )
{
    // <Connection>
    if ( connections.size() > 0 )
    {
        kvs::kvsml::ConnectionTag connection_tag;
        if ( !connection_tag.write( parent ) )
        {
            kvsMessageError( "Cannot write <%s>.", connection_tag.name().c_str() );
            return false;
        }

        // <DataArray>
        kvs::kvsml::DataArrayTag data_tag;
        if ( writing_type == kvs::kvsml::ExternalAscii )
        {
            data_tag.setFile( kvs::kvsml::DataArray::GetDataFilename( filename, "connect" ) );
            data_tag.setFormat( "ascii" );
        }
        else if ( writing_type == kvs::kvsml::ExternalBinary )
        {
            data_tag.setFile( kvs::kvsml::DataArray::GetDataFilename( filename, "connect" ) );
            data_tag.setFormat( "binary" );
        }

        const std::string pathname = kvs::File( filename ).pathName();
        if ( !data_tag.write( connection_tag.node(), connections, pathname ) )
        {
            kvsMessageError( "Cannot write <%s> for <%s>.",
                             data_tag.name().c_str(),
                             connection_tag.name().c_str() );
            return false;
        }
    }

    return true;
}

/*===========================================================================*/
/**
 *  @brief  Writes opacity data to <Coord>.
 *  @param  parent [out] pointer to the parent node
 *  @param  writing_type [in] writing data type
 *  @param  filename [in] filename
 *  @param  opacity [in] opacity value array
 *  @return true, if the writing process is done successfully
 */
/*===========================================================================*/
bool WriteOpacityData(
    kvs::XMLNode::SuperClass* parent,
    const kvs::kvsml::WritingDataType writing_type,
    const std::string& filename,
    const kvs::ValueArray<kvs::UInt8>& opacities )
{
    // <Opacity>
    if ( opacities.size() > 0 )
    {
        kvs::kvsml::OpacityTag opacity_tag;
        if ( !opacity_tag.write( parent ) )
        {
            kvsMessageError( "Cannot write <%s>.", opacity_tag.name().c_str() );
            return false;
        }

        // <DataValue>
        if ( opacities.size() == 1 )
        {
            kvs::kvsml::DataValueTag data_tag;
            if ( !data_tag.write( opacity_tag.node(), opacities ) )
            {
                kvsMessageError( "Cannot write <%s> for <%s>.",
                                 data_tag.name().c_str(),
                                 opacity_tag.name().c_str() );
                return false;
            }
        }
        // <DataArray>
        else
        {
            kvs::kvsml::DataArrayTag data_tag;
            if ( writing_type == kvs::kvsml::ExternalAscii )
            {
                data_tag.setFile( kvs::kvsml::DataArray::GetDataFilename( filename, "opacity" ) );
                data_tag.setFormat( "ascii" );
            }
            else if ( writing_type == kvs::kvsml::ExternalBinary )
            {
                data_tag.setFile( kvs::kvsml::DataArray::GetDataFilename( filename, "opacity" ) );
                data_tag.setFormat( "binary" );
            }

            const std::string pathname = kvs::File( filename ).pathName();
            if ( !data_tag.write( opacity_tag.node(), opacities, pathname ) )
            {
                kvsMessageError( "Cannot write <%s> for <%s>.",
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
