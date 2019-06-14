#pragma once
#include <kvs/VolumeObjectBase>
#include <kvs/StructuredVolumeObject>
#include <kvs/FilterBase>
#include <kvs/Module>


namespace kvs
{

/*===========================================================================*/
/**
 *  @brief  StructuredExtractScalar class.
 */
/*===========================================================================*/
class StructuredExtractScalar : public kvs::FilterBase, public kvs::StructuredVolumeObject
{
    kvsModule( kvs::StructuredExtractScalar, Filter );
    kvsModuleBaseClass( kvs::FilterBase );
    kvsModuleSuperClass( kvs::StructuredVolumeObject );

private:
    size_t m_offset;

public:
    StructuredExtractScalar(): m_offset( 0 ) {}
    StructuredExtractScalar( const kvs::StructuredVolumeObject* volume, const size_t offset );
    virtual ~StructuredExtractScalar() {}

    void setOffset( const size_t offset ) { m_offset = offset; }

    SuperClass* exec( const kvs::ObjectBase* object );

private:
    template <typename T>
    void slice_scalar( const kvs::StructuredVolumeObject* volume );
};

} // end of namespace kvs
