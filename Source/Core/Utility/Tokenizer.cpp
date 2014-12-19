/****************************************************************************/
/**
 *  @file Tokenizer.cpp
 */
/*----------------------------------------------------------------------------
 *
 *  Copyright (c) Visualization Laboratory, Kyoto University.
 *  All rights reserved.
 *  See http://www.viz.media.kyoto-u.ac.jp/kvs/copyright/ for details.
 *
 *  $Id: Tokenizer.cpp 1416 2013-02-15 10:59:48Z s.yamada0808@gmail.com $
 */
/****************************************************************************/
#include "Tokenizer.h"


namespace kvs
{

/*==========================================================================*/
/**
 *  Constructor.
 */
/*==========================================================================*/
Tokenizer::Tokenizer()
{
    m_next = std::string::npos;
}

/*==========================================================================*/
/**
 *  Constructor.
 *  @param source [in] source
 *  @param delimiter [in] delimiters
 */
/*==========================================================================*/
Tokenizer::Tokenizer( const std::string& source, const std::string& delimiter ):
    m_source( source ),
    m_delimiter( delimiter )
{
    m_next = m_source.find_first_not_of( m_delimiter );
}

/*==========================================================================*/
/**
 *  Substitution oeprator.
 *  @param source [in] source
 */
/*==========================================================================*/
Tokenizer& Tokenizer::operator = ( const std::string& source )
{
    m_source = source;
    m_next = m_source.find_first_not_of( m_delimiter );
    return *this;
}

/*==========================================================================*/
/**
 *  Test method whether the token is last.
 *  @return true, if the node is last
 */
/*==========================================================================*/
bool Tokenizer::isLast() const
{
    return m_next == std::string::npos;
}

/*==========================================================================*/
/**
 *  Get the delimitted token.
 *  @return token
 */
/*==========================================================================*/
std::string Tokenizer::token()
{
    if ( this->isLast() )
    {
        return std::string();
    }

    std::string::size_type first = m_next;
    std::string::size_type last = m_source.find_first_of( m_delimiter, first );

    if ( last == std::string::npos )
    {
        m_next = std::string::npos;
        return m_source.substr( first );
    }
    else
    {
        m_next = m_source.find_first_not_of( m_delimiter, last );
        return m_source.substr( first, last - first );
    }
}

} // end of namespace kvs
