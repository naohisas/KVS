/****************************************************************************/
/**
 *  @file   StructuredVolumeImporter.h
 *  @author Naohisa Sakamoto
 */
/****************************************************************************/
#pragma once
#include "ImporterBase.h"
#include <kvs/Module>
#include <kvs/StructuredVolumeObject>
#include <kvs/KVSMLStructuredVolumeObject>
#include <kvs/AVSField>
#include <kvs/DicomList>


namespace kvs
{

/*==========================================================================*/
/**
 *  @brief  Structured volume object importer class.
 */
/*==========================================================================*/
class StructuredVolumeImporter : public kvs::ImporterBase, public kvs::StructuredVolumeObject
{
    kvsModule( kvs::StructuredVolumeImporter, Importer );
    kvsModuleBaseClass( kvs::ImporterBase );
    kvsModuleSuperClass( kvs::StructuredVolumeObject );

public:
    StructuredVolumeImporter();
    StructuredVolumeImporter( const std::string& filename );
    StructuredVolumeImporter( const kvs::FileFormatBase* file_format );
    virtual ~StructuredVolumeImporter();

    SuperClass* exec( const kvs::FileFormatBase* file_format );

private:
    void import( const kvs::AVSField* field );
    void import( const kvs::DicomList* dicom_list );
    template <typename T>
    const kvs::AnyValueArray get_dicom_data( const kvs::DicomList* dicom_list, const bool shift );
};

} // end of namespace kvs
