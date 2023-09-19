/****************************************************************************/
/**
 *  @file   ObjectImporter.h
 *  @author Naohisa Sakamoto
 */
/****************************************************************************/
#pragma once
#include <string>
#include <kvs/FileFormatBase>
#include <kvs/ImporterBase>
#include <kvs/ObjectBase>


namespace kvs
{

/*==========================================================================*/
/**
 *  Object importer class.
 */
/*==========================================================================*/
class ObjectImporter
{
public:
    enum ImporterType
    {
        Image = 0, ///< image object importer
        Point, ///< point object importer
        Line, ///< line object importer
        Polygon,///< polygon object importer
        StructuredVolume, ///< structured volume object importer
        UnstructuredVolume, ///< unstructured volume object importer
        Unknown ///< unknown importer
    };

private:
    std::string m_filename{}; ///< input filename
    ImporterType m_importer_type = ImporterType::Unknown; ///< importer type
    kvs::FileFormatBase* m_file_format = nullptr; ///< pointer to the estimated file format
    kvs::ImporterBase* m_importer = nullptr; ///< pointer to the estimated importer

public:
    explicit ObjectImporter( const std::string& filename );
    ~ObjectImporter();

    kvs::ObjectBase* import();

private:

    bool estimate_file_format();
    bool estimate_importer();
};

} // end of namespace kvs
