/****************************************************************************/
/**
 *  @file   StructuredVolumeImporter.cpp
 *  @author Naohisa Sakamoto
 */
/*----------------------------------------------------------------------------
 *
 *  Copyright (c) Visualization Laboratory, Kyoto University.
 *  All rights reserved.
 *  See http://www.viz.media.kyoto-u.ac.jp/kvs/copyright/ for details.
 *
 *  $Id: StructuredVolumeImporter.cpp 1811 2014-09-03 02:53:36Z naohisa.sakamoto@gmail.com $
 */
/****************************************************************************/
#include "StructuredVolumeImporter.h"
#include <kvs/DebugNew>
#include <kvs/AVSField>
#include <kvs/DicomList>
#include <kvs/Message>
#include <kvs/Vector3>
#include <kvs/Directory>
#include <kvs/Value>


namespace kvs
{

/*==========================================================================*/
/**
 *  @brief  Constructs a new StructuredVolumeImporter class.
 */
/*==========================================================================*/
StructuredVolumeImporter::StructuredVolumeImporter()
{
}

/*===========================================================================*/
/**
 *  @brief  Constructs a new StructuredVolumeImporter class.
 *  @param  filename [in] input filename
 */
/*===========================================================================*/
StructuredVolumeImporter::StructuredVolumeImporter( const std::string& filename )
{
    if ( kvs::KVSMLStructuredVolumeObject::CheckExtension( filename ) )
    {
        BaseClass::setSuccess( SuperClass::read( filename ) );
    }
    else if ( kvs::AVSField::CheckExtension( filename ) )
    {
        kvs::AVSField* file_format = new kvs::AVSField( filename );
        if( !file_format )
        {
            BaseClass::setSuccess( false );
            kvsMessageError("Cannot read '%s'.",filename.c_str());
            return;
        }

        if( file_format->isFailure() )
        {
            BaseClass::setSuccess( false );
            kvsMessageError("Cannot read '%s'.",filename.c_str());
            delete file_format;
            return;
        }

        this->import( file_format );
        delete file_format;
    }
    else if ( kvs::DicomList::CheckDirectory( filename ) )
    {
        kvs::DicomList* file_format = new kvs::DicomList( filename );
        if( !file_format )
        {
            BaseClass::setSuccess( false );
            kvsMessageError("Cannot read '%s'.",filename.c_str());
            return;
        }

        if( file_format->isFailure() )
        {
            BaseClass::setSuccess( false );
            kvsMessageError("Cannot read '%s'.",filename.c_str());
            delete file_format;
            return;
        }

        this->import( file_format );
        delete file_format;
    }
    else
    {
        BaseClass::setSuccess( false );
        kvsMessageError("Cannot import '%s'.",filename.c_str());
        return;
    }
}

/*==========================================================================*/
/**
 *  @brief  Constructs a new StructuredVolumeImporter class.
 *  @param  file_format [in] pointer to the file format data
 */
/*==========================================================================*/
StructuredVolumeImporter::StructuredVolumeImporter( const kvs::FileFormatBase* file_format )
{
    if ( !this->exec( file_format ) ) BaseClass::setSuccess( true );
}

/*===========================================================================*/
/**
 *  @brief  Destructs the StructuredVolumeImporter class.
 */
/*===========================================================================*/
StructuredVolumeImporter::~StructuredVolumeImporter()
{
}

/*===========================================================================*/
/**
 *  @brief  Executes the import process.
 *  @param  file_format [in] pointer to the file format data
 *  @return pointer to the imported structured volume object
 */
/*===========================================================================*/
StructuredVolumeImporter::SuperClass* StructuredVolumeImporter::exec( const kvs::FileFormatBase* file_format )
{
    if ( !file_format )
    {
        BaseClass::setSuccess( false );
        kvsMessageError("Input file format is NULL.");
        return NULL;
    }

    if ( dynamic_cast<const kvs::KVSMLStructuredVolumeObject*>( file_format ) )
    {
        BaseClass::setSuccess( SuperClass::read( file_format->filename() ) );
    }
    else if ( const kvs::AVSField* volume = dynamic_cast<const kvs::AVSField*>( file_format ) )
    {
        this->import( volume );
    }
    else if ( const kvs::DicomList* volume = dynamic_cast<const kvs::DicomList*>( file_format ) )
    {
        this->import( volume );
    }
    else
    {
        BaseClass::setSuccess( false );
        kvsMessageError("Input file format is not supported.");
        return NULL;
    }

    return this;
}

/*==========================================================================*/
/**
 *  @brief  Imports the AVS field format data.
 *  @param  field [in] pointer to the AVS field format data
 */
/*==========================================================================*/
void StructuredVolumeImporter::import( const kvs::AVSField* field )
{
    if ( field->fieldType() != kvs::AVSField::Uniform )
    {
        kvsMessageError( "'Uniform' type in kvs::AVSField format is only supported." );
        BaseClass::setSuccess( false );
        return;
    }

    const float dim_x = static_cast<float>( field->dim().x() );
    const float dim_y = static_cast<float>( field->dim().y() );
    const float dim_z = static_cast<float>( field->dim().z() );
    const kvs::Vector3f  min_obj_coord( 0.0f, 0.0f, 0.0f );
    const kvs::Vector3f  max_obj_coord( dim_x - 1.0f, dim_y - 1.0f, dim_z - 1.0f );
    SuperClass::setMinMaxObjectCoords( min_obj_coord, max_obj_coord );

    if ( field->hasMinMaxExt() )
    {
        const kvs::Vector3f min_ext_coord( field->minExt() );
        const kvs::Vector3f max_ext_coord( field->maxExt() );
        SuperClass::setMinMaxExternalCoords( min_ext_coord, max_ext_coord );
    }
    else
    {
        const kvs::Vector3f min_ext_coord( min_obj_coord );
        const kvs::Vector3f max_ext_coord( max_obj_coord );
        SuperClass::setMinMaxExternalCoords( min_ext_coord, max_ext_coord );
    }

    SuperClass::setGridType( kvs::StructuredVolumeObject::Uniform );
    SuperClass::setVeclen( field->veclen() );
    SuperClass::setResolution( field->dim() );
    SuperClass::setValues( field->values() );
    SuperClass::updateMinMaxValues();
}

void StructuredVolumeImporter::import( const kvs::DicomList* dicom_list )
{
    if ( dicom_list->size() == 0 )
    {
        BaseClass::setSuccess( false );
        kvsMessageError("Dicom file is not included.");
        return;
    }

    const float spacing = static_cast<float>( dicom_list->sliceSpacing() );
    const float thickness = static_cast<float>( dicom_list->sliceThickness() );

    const size_t x_size = dicom_list->width();
    const size_t y_size = dicom_list->height();
    const size_t z_size = dicom_list->nslices();
    const float x_ratio = dicom_list->pixelSpacing()[0];
    const float y_ratio = dicom_list->pixelSpacing()[1];
    const float z_ratio = kvs::Math::IsZero( spacing ) ? kvs::Math::IsZero( thickness ) ? x_ratio : thickness : spacing;

    const kvs::Vector3f min_obj_coord( 0.0f, 0.0f, 0.0f );
    const kvs::Vector3f max_obj_coord( x_size - 1.0f, y_size - 1.0f, z_size - 1.0f );
    SuperClass::setMinMaxObjectCoords( min_obj_coord, max_obj_coord );

    const kvs::Vector3f min_ext_coord( min_obj_coord );
    const kvs::Vector3f max_ext_coord( max_obj_coord.x() * x_ratio,
                                       max_obj_coord.y() * y_ratio,
                                       max_obj_coord.z() * z_ratio );
    SuperClass::setMinMaxExternalCoords( min_ext_coord, max_ext_coord );

    bool shift = true;
    const kvs::Dicom* dicom = (*dicom_list)[0];
    const kvs::UInt32 bits_allocated = dicom->bitsAllocated();
    const bool pixel_representation = dicom->pixelRepresentation();
    switch ( bits_allocated )
    {
    case 8:
    {
        const kvs::AnyValueArray values = this->get_dicom_data<kvs::UInt8>( dicom_list, false );
        SuperClass::setValues( values );
        break;
    }
    case 16:
    {
        if ( pixel_representation )
        {
            const kvs::AnyValueArray values = this->get_dicom_data<kvs::UInt16>( dicom_list, false );
            SuperClass::setValues( values );
        }
        else
        {
            if ( shift )
            {
                const kvs::AnyValueArray values = this->get_dicom_data<kvs::UInt16>( dicom_list, true );
                SuperClass::setValues( values );
            }
            else
            {
                const kvs::AnyValueArray values = this->get_dicom_data<kvs::Int16>( dicom_list, false );
                SuperClass::setValues( values );
            }
        }
        break;
    }
    default: break;
    }

    const kvs::Vector3ui resolution( x_size, y_size, z_size );
    SuperClass::setGridType( kvs::StructuredVolumeObject::Uniform );
    SuperClass::setResolution( resolution );
    SuperClass::setVeclen( 1 );
    SuperClass::updateMinMaxValues();
}

/*===========================================================================*/
/**
 *  @brief  Returns the data values of the DICOM list.
 *  @param  dicom_list [in] pointer to the DICOM list
 *  @param  shift [in] check flag for value shift
 *  @return data values
 */
/*===========================================================================*/
template <typename T>
const kvs::AnyValueArray StructuredVolumeImporter::get_dicom_data(
    const kvs::DicomList* dicom_list,
    const bool shift )
{
    const size_t width = dicom_list->width();
    const size_t height = dicom_list->height();
    const size_t nslices = dicom_list->nslices();
    const size_t nnodes = width * height * nslices;

    const double min_range = static_cast<double>( kvs::Value<T>::Min() );
    const double max_range = static_cast<double>( kvs::Value<T>::Max() );

    kvs::AnyValueArray values;
    values.template allocate<T>( nnodes );

    T* pvalues = static_cast<T*>( values.data() );
    for ( size_t k = 0; k < nslices; k++ )
    {
        const kvs::Dicom* dicom = (*dicom_list)[k];
        const T* const raw_data = reinterpret_cast<const T*>( dicom->rawData().data() );
        const int shift_value = shift ? dicom->minRawValue() : 0;

        for ( size_t j = 0; j < height; j++ )
        {
            for ( size_t i = 0; i < width; i++ )
            {
                const size_t pixel_index = ( height - j - 1 ) * width + i;
                double value = static_cast<double>( raw_data[ pixel_index ] );
                value = value - shift_value;
                value = kvs::Math::Clamp( value, min_range, max_range );

                *(pvalues++) = static_cast<T>( value );
            }
        }
    }

    return values;
}

// Instatiation.
template const kvs::AnyValueArray StructuredVolumeImporter::get_dicom_data<kvs::UInt8>(
    const kvs::DicomList* dicom_list, const bool shift );
template const kvs::AnyValueArray StructuredVolumeImporter::get_dicom_data<kvs::UInt16>(
    const kvs::DicomList* dicom_list, const bool shift );
template const kvs::AnyValueArray StructuredVolumeImporter::get_dicom_data<kvs::Int16>(
    const kvs::DicomList* dicom_list, const bool shift );

} // end of namespace kvs
