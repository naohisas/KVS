/****************************************************************************/
/**
 *  @file   FrequencyTable.h
 *  @author Naohisa Sakamoto
 */
/*----------------------------------------------------------------------------
 *
 *  Copyright (c) Visualization Laboratory, Kyoto University.
 *  All rights reserved.
 *  See http://www.viz.media.kyoto-u.ac.jp/kvs/copyright/ for details.
 *
 *  $Id: FrequencyTable.h 1726 2014-04-22 06:53:11Z naohisa.sakamoto@gmail.com $
 */
/****************************************************************************/
#ifndef KVS__FREQUENCY_TABLE_H_INCLUDE
#define KVS__FREQUENCY_TABLE_H_INCLUDE

#include <kvs/ValueArray>
#include <kvs/Type>
#include <kvs/Math>
#include <kvs/Message>
#include <kvs/VolumeObjectBase>
#include <kvs/ImageObject>
#include <kvs/Deprecated>
#include <list>


namespace kvs
{

/*==========================================================================*/
/**
 *  Frequency distribution table class.
 */
/*==========================================================================*/
class FrequencyTable
{
protected:

    kvs::Real64 m_min_range; ///< min. range value
    kvs::Real64 m_max_range; ///< max. range value
    size_t m_max_count; ///< min. count value
    kvs::Real64 m_mean; ///< mean value
    kvs::Real64 m_variance; ///< variance value
    kvs::Real64 m_standard_deviation; ///< standard deviation
    kvs::UInt64 m_nbins; ///< number of bins
    kvs::ValueArray<size_t> m_bin; ///< bin array
    std::list<kvs::Real64> m_ignore_values; ///< ignore value list

public:

    FrequencyTable();
    virtual ~FrequencyTable();

    kvs::Real64 minRange() const { return m_min_range; }
    kvs::Real64 maxRange() const { return m_max_range; }
    size_t maxCount() const { return m_max_count; }
    kvs::Real64 mean() const { return m_mean; }
    kvs::Real64 variance() const { return m_variance; }
    kvs::Real64 standardDeviation() const { return m_standard_deviation; }
    kvs::UInt64 numberOfBins() const { return m_nbins; }
    const kvs::ValueArray<size_t>& bin() const { return m_bin; }

    void setIgnoreValue( const kvs::Real64 value ) { m_ignore_values.push_back( value ); }
    void setRange( const kvs::Real64 min_range, const kvs::Real64 max_range ) { m_min_range = min_range; m_max_range = max_range; }
    void setNumberOfBins( const kvs::UInt64 nbins ) { m_nbins = nbins; }

    void create( const kvs::VolumeObjectBase* volume );
    void create( const kvs::ImageObject* image, const size_t channel = 0 );

    kvs::UInt64 operator [] ( const size_t index ) const;
    kvs::UInt64 at( const size_t index ) const;

private:

    void calculate_range( const kvs::VolumeObjectBase* volume );
    void calculate_range( const kvs::ImageObject* image );
    void count_bin( const kvs::VolumeObjectBase* volume );
    void count_bin( const kvs::ImageObject* image, const size_t channel );
    template <typename T> void binning( const kvs::VolumeObjectBase* volume );
    template <typename T> void binning( const kvs::ImageObject* image, const size_t channel );
    bool is_ignore_value( const kvs::Real64 value );

public:
    KVS_DEPRECATED( kvs::UInt64 nbins() const ) { return this->numberOfBins(); }
    KVS_DEPRECATED( void setNBins( const kvs::UInt64 nbins ) ) { this->setNumberOfBins( nbins ); }
};

/*==========================================================================*/
/**
 *  Create a bin array.
 *  @param volume [in] pointer to the volume object
 */
/*==========================================================================*/
template <typename T>
inline void FrequencyTable::binning( const kvs::VolumeObjectBase* volume )
{
    const size_t veclen = volume->veclen();
    const T* values = reinterpret_cast<const T*>( volume->values().data() );
    const T* const end = values + volume->numberOfNodes() * veclen;
    const kvs::Real64 width = ( m_max_range - m_min_range ) / kvs::Real64( m_nbins - 1 );

    size_t total_count = 0;

    m_max_count = 0;
    if ( veclen == 1 )
    {
        while ( values < end )
        {
            kvs::Real64 value = kvs::Real64( *values );
            ++values;

            if ( !this->is_ignore_value( value ) )
            {
//                kvs::UInt64 index = static_cast<kvs::UInt64>( ( value - m_min_range ) / width );
//                index = kvs::Math::Clamp( index, kvs::UInt64(0), m_nbins - 1 );
                kvs::Real64 temp = static_cast<kvs::UInt64>( ( value - m_min_range ) / width );
                kvs::UInt64 index = kvs::Math::Floor( temp );

                m_bin[index] = m_bin[index] + 1;
                m_max_count = kvs::Math::Max( m_max_count, m_bin[index] );

                total_count++;
            }
        }
    }
    else
    {
        while ( values < end )
        {
            kvs::Real64 magnitude = 0.0;
            for ( size_t i = 0; i < veclen; ++i )
            {
                kvs::Real64 value = kvs::Real64( *values );
                magnitude += static_cast<kvs::Real64>( kvs::Math::Square( value ) );
                ++values;
            }
            magnitude = std::sqrt( magnitude );

            if ( !this->is_ignore_value( magnitude ) )
            {
//                kvs::UInt64 index = static_cast<kvs::UInt64>( ( magnitude - m_min_range ) / width );
//                index = kvs::Math::Clamp( index, kvs::UInt64(0), m_nbins - 1 );
                kvs::Real64 temp = static_cast<kvs::UInt64>( ( magnitude - m_min_range ) / width );
                kvs::UInt64 index = kvs::Math::Floor( temp );

                m_bin[index] = m_bin[index] + 1;
                m_max_count = kvs::Math::Max( m_max_count, m_bin[index] );

                total_count++;
            }
        }
    }

    m_mean = static_cast<kvs::Real64>( total_count ) / m_nbins;

    kvs::Real64 sum = 0;
    for ( size_t i = 0; i < m_nbins; i++ ) sum += kvs::Math::Square( m_bin[i] - m_mean );
    m_variance = sum / m_nbins;

    m_standard_deviation = std::sqrt( m_variance );
}

/*==========================================================================*/
/**
 *  Create a bin array.
 *  @param image [in] pointer to the image object
 *  @param channel [in] color element channel (0, 1, 2, 3)
 */
/*==========================================================================*/
template <typename T>
inline void FrequencyTable::binning( const kvs::ImageObject* image, const size_t channel )
{
    if ( channel >= image->numberOfChannels() )
    {
        kvsMessageError("Specified channel is invalid.");
        return;
    }

    const T* values = reinterpret_cast<const T*>( image->pixels().data() );
    const kvs::Real64 width = ( m_max_range - m_min_range ) / kvs::Real64( m_nbins - 1 );
    const size_t stride  = image->numberOfChannels();
    const size_t npixels = image->width() * image->height();

    size_t total_count = 0;

    m_max_count = 0;
    for ( size_t i = 0; i < npixels; i++ )
    {
        const kvs::Real64 value = kvs::Real64( *( values + channel + i * stride ) );

        if ( !this->is_ignore_value( value ) )
        {
            kvs::UInt64 index = static_cast<kvs::UInt64>( ( value - m_min_range ) / width );
            index = kvs::Math::Clamp( index, kvs::UInt64(0), m_nbins - 1 );

            m_bin[index] = m_bin[index] + 1;
            m_max_count = kvs::Math::Max( m_max_count, m_bin[index] );

            total_count++;
        }
    }

    m_mean = static_cast<kvs::Real64>( total_count ) / m_nbins;

    kvs::Real64 sum = 0;
    for ( size_t i = 0; i < m_nbins; i++ ) sum += kvs::Math::Square( m_bin[i] - m_mean );
    m_variance = sum / m_nbins;

    m_standard_deviation = std::sqrt( m_variance );
}

} // end of namespace kvs

#endif // KVS__FREQUENCY_TABLE_H_INCLUDE
