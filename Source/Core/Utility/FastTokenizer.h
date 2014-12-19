/****************************************************************************/
/**
 *  @file FastTokenizer.h
 */
/*----------------------------------------------------------------------------
 *
 *  Copyright (c) Visualization Laboratory, Kyoto University.
 *  All rights reserved.
 *  See http://www.viz.media.kyoto-u.ac.jp/kvs/copyright/ for details.
 *
 *  $Id: FastTokenizer.h 1707 2014-01-27 07:37:04Z naohisa.sakamoto@gmail.com $
 */
/****************************************************************************/
#if KVS_ENABLE_DEPRECATED
#ifndef KVS__FAST_TOKENIZER_H_INCLUDE
#define KVS__FAST_TOKENIZER_H_INCLUDE

#include <cstring>
#include <kvs/Assert>


namespace kvs
{

/*==========================================================================*/
/**
 *  FastTokenizer.
 */
/*==========================================================================*/
class FastTokenizer
{
private:

    enum { BufferSize = 256 };

private:

    size_t      m_ntokens;
    const char* m_tokens[ BufferSize ];

public:

    FastTokenizer();

    FastTokenizer( char* const source, const char* const delimiter );

    ~FastTokenizer();

public:

    void set( char* const source, const char* const delimiter );

    const char* get( size_t index ) const;

public:

    size_t ntokens() const;

public:

};

inline void FastTokenizer::set( char* const source, const char* const delimiter )
{
    size_t ntokens = 0;

    for ( const char* p = strtok( source, delimiter ); p != NULL; p = strtok( NULL, delimiter ) )
    {
        m_tokens[ ntokens ] = p;
        ++ntokens;

        KVS_ASSERT( ntokens < BufferSize );
    }

    m_ntokens = ntokens;
}

inline const char* FastTokenizer::get( size_t index ) const
{
    KVS_ASSERT( index < m_ntokens );

    return m_tokens[ index ];
}

inline size_t FastTokenizer::ntokens() const
{
    return m_ntokens;
}

} // end of namespace kvs

#endif // KVS__FAST_TOKENIZER_H_INCLUDE
#endif
