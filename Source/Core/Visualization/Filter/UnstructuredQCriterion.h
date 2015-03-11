/*****************************************************************************/
/**
 *  @file   UnstructuredQCriterion.h
 *  @author Naohisa Sakamoto
 */
/*----------------------------------------------------------------------------
 *
 *  Copyright (c) Visualization Laboratory, Kyoto University.
 *  All rights reserved.
 *  See http://www.viz.media.kyoto-u.ac.jp/kvs/copyright/ for details.
 *
 *  $Id$
 */
/*****************************************************************************/
#pragma once

#include <kvs/Module>
#include <kvs/FilterBase>
#include <kvs/UnstructuredVolumeObject>


namespace kvs
{

/*===========================================================================*/
/**
 *  @brief  Q-criterion (2nd invariant of gradient tensor) calculation class for unstructured volume object
 */
/*===========================================================================*/
class UnstructuredQCriterion : public kvs::FilterBase, public kvs::UnstructuredVolumeObject
{
    kvsModule( kvs::UnstructuredQCriterion, Filter );
    kvsModuleBaseClass( kvs::FilterBase );
    kvsModuleSuperClass( kvs::UnstructuredVolumeObject );

public:

    UnstructuredQCriterion( const kvs::UnstructuredVolumeObject* volume );
    SuperClass* exec( const kvs::ObjectBase* object );

private:

    void qvalues_from_vectors( const kvs::UnstructuredVolumeObject* volume );
    void qvalues_from_tensors( const kvs::UnstructuredVolumeObject* volume );
};

} // end of namespace kvs
