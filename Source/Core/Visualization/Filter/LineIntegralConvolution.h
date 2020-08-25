/*****************************************************************************/
/**
 *  @file   LineIntegralConvolution.h
 *  @author Naohisa Sakamoto
 */
/*****************************************************************************/
#ifndef KVS__LINE_INTEGRAL_CONVOLUTION_H_INCLUDE
#define KVS__LINE_INTEGRAL_CONVOLUTION_H_INCLUDE

#include <kvs/StructuredVolumeObject>
#include <kvs/FilterBase>
#include <kvs/Module>


namespace kvs
{

/*===========================================================================*/
/**
 *  @brief  LIC class.
 */
/*===========================================================================*/
class LineIntegralConvolution : public kvs::FilterBase, public kvs::StructuredVolumeObject
{
    kvsModule( kvs::LineIntegralConvolution, Filter );
    kvsModuleBaseClass( kvs::FilterBase );
    kvsModuleSuperClass( kvs::StructuredVolumeObject );

protected:

    double m_length; ///< stream length
    kvs::StructuredVolumeObject* m_noise; ///< white noise volume

public:

    LineIntegralConvolution();
    LineIntegralConvolution( const kvs::StructuredVolumeObject* volume );
    LineIntegralConvolution( const kvs::StructuredVolumeObject* volume, const double length );
    virtual ~LineIntegralConvolution();

    void setLength( const double length );

    SuperClass* exec( const kvs::ObjectBase* object );

protected:

    void filtering( const kvs::StructuredVolumeObject* volume );
    void create_noise_volume( const kvs::StructuredVolumeObject* volume );
    template <typename T>
    void convolution( const kvs::StructuredVolumeObject* volume );
};

} // end of namespace kvs

#endif // KVS__LINE_INTEGRAL_CONVOLUTION_H_INCLUDE
