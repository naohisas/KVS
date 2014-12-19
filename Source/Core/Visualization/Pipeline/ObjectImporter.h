/****************************************************************************/
/**
 *  @file ObjectImporter.h
 */
/*----------------------------------------------------------------------------
 *
 *  Copyright (c) Visualization Laboratory, Kyoto University.
 *  All rights reserved.
 *  See http://www.viz.media.kyoto-u.ac.jp/kvs/copyright/ for details.
 *
 *  $Id: ObjectImporter.h 1399 2012-12-06 15:27:59Z naohisa.sakamoto@gmail.com $
 */
/****************************************************************************/
#ifndef KVS__OBJECT_IMPORTER_H_INCLUDE
#define KVS__OBJECT_IMPORTER_H_INCLUDE

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

    std::string m_filename; ///< input filename
    ImporterType m_importer_type; ///< importer type
    kvs::FileFormatBase* m_file_format; ///< pointer to the estimated file format class
    kvs::ImporterBase* m_importer; ///< pointer to the estimated importer class

public:

    explicit ObjectImporter( const std::string& filename );
    ~ObjectImporter();

    kvs::ObjectBase* import();

private:

    bool estimate_file_format();
    bool estimate_importer();
};

} // end of namespace kvs

#endif // KVS__OBJECT_IMPORTER_H_INCLUDE
