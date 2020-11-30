/*****************************************************************************/
/**
 *  @file   SliceRange.h
 *  @author Naohisa Sakamoto
 *  @brief  Range class for slicing into ValueArray and ValueTable
 */
/*****************************************************************************/
#pragma once
#include <initializer_list>
#include <algorithm>
#include <utility>
#include <kvs/Exception>
#include <kvs/Assert>


namespace kvs
{

/*===========================================================================*/
/**
 *  @brief  Range class for slicing into ValueArray and ValueTable
 */
/*===========================================================================*/
struct SliceRange
{
    mutable long start; ///< starting index
    mutable long stop;  ///< stopping index (not included)
    mutable long step;  ///< step (not zero)
    mutable bool has_stop; ///< if true, 'stop' is specified

    SliceRange():
        start( 0 ),
        stop( 1 ),
        step( 1 ),
        has_stop( false ) {}

    SliceRange( const long in_start ):
        start( in_start ),
        stop( 1 ),
        step( 1 ),
        has_stop( false ) {}

    SliceRange( const long in_start, const long in_stop ):
        start( in_start ),
        stop( in_stop ),
        step( 1 ),
        has_stop( true ) {}

    SliceRange( const long in_start, const long in_stop, const long in_step ):
        start( in_start ),
        stop( in_stop ),
        step( in_step ),
        has_stop( true ) {}

    SliceRange( std::initializer_list<long> list ):
        start( list.size() > 0 ? *( list.begin() + 0 ) : 0 ),
        stop( list.size() > 1 ? *( list.begin() + 1 ) : 1 ),
        step( list.size() > 2 ? *( list.begin() + 2 ) : 1 ),
        has_stop( list.size() > 1 ) {}

    long size() const
    {
        KVS_ASSERT( step != 0 );
        return ( stop - start ) / step;
    }

    void adjust( const size_t array_size ) const
    {
        const auto size = static_cast<long>( array_size );

        // Adjust the starting index.
        if ( start < 0 ) { start += size; }
        if ( start > size - 1 )
        {
            KVS_THROW( kvs::InvalidDataException, "Invalid starting index." );
        }

        // Adjust the stopping index.
        if ( !has_stop ) { stop = array_size; }
        else
        {
            if ( stop < 0 ) { stop += size; }
            if ( stop > size )
            {
                KVS_THROW( kvs::InvalidDataException, "Invalid stopping index." );
            }
        }

        // Adjust the slicing step.
        if ( start < stop )
        {
            if ( step < 0 )
            {
                KVS_THROW( kvs::InvalidDataException, "Invalid slicing step." );
            }
        }

        if ( stop < start )
        {
            if ( step > 0 )
            {
                KVS_THROW( kvs::InvalidDataException, "Invalid slicing step." );
            }

            std::swap( start, stop );
            step *= -1;
        }
    }
};

} // end of namespace kvs
