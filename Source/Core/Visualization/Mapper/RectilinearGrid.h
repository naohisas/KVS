/*****************************************************************************/
/**
 *  @file   RectilinearGrid.h
 *  @author Naohisa Sakamoto
 */
/*****************************************************************************/
#include "GridBase.h"


namespace kvs
{

class RectilinearGrid : public kvs::GridBase
{
public:

    typedef kvs::GridBase BaseClass;

public:

    RectilinearGrid( const kvs::StructuredVolumeObject* volume );

    int findGrid( const kvs::Vec3& global ) const;
    kvs::Vec3 globalToLocal( const kvs::Vec3& global ) const;
};

} // end of namespace kvs
