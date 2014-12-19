/****************************************************************************/
/**
 *  @file   PolygonImporter.h
 *  @author Naohisa Sakamoto
 */
/*----------------------------------------------------------------------------
 *
 *  Copyright (c) Visualization Laboratory, Kyoto University.
 *  All rights reserved.
 *  See http://www.viz.media.kyoto-u.ac.jp/kvs/copyright/ for details.
 *
 *  $Id: PolygonImporter.h 1721 2014-03-12 15:27:38Z naohisa.sakamoto@gmail.com $
 */
/****************************************************************************/
#ifndef KVS__POLYGON_IMPORTER_H_INCLUDE
#define KVS__POLYGON_IMPORTER_H_INCLUDE

#include "ImporterBase.h"
#include <kvs/Module>
#include <kvs/PolygonObject>
#include <kvs/KVSMLObjectPolygon>
#include <kvs/Stl>
#include <kvs/Ply>


namespace kvs
{

/*==========================================================================*/
/**
 *  @brief  Polygon importer class.
 */
/*==========================================================================*/
class PolygonImporter : public kvs::ImporterBase, public kvs::PolygonObject
{
    kvsModule( kvs::PolygonImporter, Importer );
    kvsModuleBaseClass( kvs::ImporterBase );
    kvsModuleSuperClass( kvs::PolygonObject );

public:

    PolygonImporter();
    PolygonImporter( const std::string& filename );
    PolygonImporter( const kvs::FileFormatBase* file_format );
    virtual ~PolygonImporter();

    SuperClass* exec( const kvs::FileFormatBase* file_format );

private:

    void import( const kvs::KVSMLObjectPolygon* kvsml );
    void import( const kvs::Stl* stl );
    void import( const kvs::Ply* ply );
    void set_min_max_coord();
};

} // end of namespace kvs

#endif // KVS__POLYGON_IMPORTER_H_INCLUDE
