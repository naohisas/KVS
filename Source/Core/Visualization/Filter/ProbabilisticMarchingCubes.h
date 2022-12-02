/*****************************************************************************/
/**
 *  @file   ProbabilisticMarchingCubes.h
 *  @author Go Tamura, Naohisa Sakamoto
 */
/*----------------------------------------------------------------------------
 *
 *  Kai Pöthkow, Britta Weber, Hans-Christian Hege, Probabilistic Marching Cubes,
 *  Computer Graphics Forum, Volume 30, Issue 3, pp.931-940, 2011.
 */
/*****************************************************************************/
#pragma once
#include <kvs/Module>
#include <kvs/FilterBase>
#include <kvs/StructuredVolumeObject>
#include <kvs/StructuredVolumeObjectList>


namespace kvs
{

/*===========================================================================*/
/**
 *  @brief  ProbabilisticMarchingCubes class.
 */
/*===========================================================================*/
class ProbabilisticMarchingCubes :
        public kvs::FilterBase,
        public kvs::StructuredVolumeObject
{
    kvsModule( kvs::ProbabilisticMarchingCubes, Filter );
    kvsModuleBaseClass( kvs::FilterBase );
    kvsModuleSuperClass( kvs::StructuredVolumeObject );

private:
    double m_isolevel = 0.0; ///< isolevel
    size_t m_nsamples = 1000; ///< number of sampling points
    kvs::ValueArray<float> m_mean_values{}; ///< mean values

public:
    ProbabilisticMarchingCubes() = default;
    virtual ~ProbabilisticMarchingCubes() = default;
    ProbabilisticMarchingCubes(
        const kvs::StructuredVolumeObjectList* objects,
        const double isolevel = 0.0,
        const size_t nsamples = 100 ):
        m_isolevel( isolevel ),
        m_nsamples( nsamples )
    {
        this->exec( objects );
    }

    void setIsolevel( const double isolevel ) { m_isolevel = isolevel; }
    void setNumberOfSamples( const size_t nsamples ) { m_nsamples = nsamples; }
    const kvs::ValueArray<float>& meanValues() const { return m_mean_values; }

    SuperClass* exec( const kvs::ObjectBase* object );

private:
    void filtering_uniform(
        const kvs::StructuredVolumeObject& volume,
        const kvs::ValueArray<float>& values );

    void filtering_rectilinear(
        const kvs::StructuredVolumeObject& volume,
        const kvs::ValueArray<float>& values );
};

} // end of namespace kvs
