/*****************************************************************************/
/**
 *  @file   ObjectInformation.h
 *  @author Naohisa Sakamoto
 */
/*----------------------------------------------------------------------------
 *
 *  Copyright (c) Visualization Laboratory, Kyoto University.
 *  All rights reserved.
 *  See http://www.viz.media.kyoto-u.ac.jp/kvs/copyright/ for details.
 *
 *  $Id: ObjectInformation.h 602 2010-08-19 02:43:34Z naohisa.sakamoto $
 */
/*****************************************************************************/
#pragma once
#include <iostream>
#include <kvs/ObjectBase>


namespace kvsview
{

/*===========================================================================*/
/**
 *  Displaying the object information.
 */
/*===========================================================================*/
class ObjectInformation
{
    const kvs::ObjectBase* m_object; // pointer to the object

    ObjectInformation();

public:
    explicit ObjectInformation( const kvs::ObjectBase* object );
    friend std::ostream& operator << ( std::ostream& os, const ObjectInformation& info );
};

} // end of namespace kvsview
