/*****************************************************************************/
/**
 *  @file   WeakPointer.h
 *  @author Naoya Maeda
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
#ifndef KVS_WEAK_POINTER
#define KVS_WEAK_POINTER
#include "SharedPointer.h"


namespace kvs
{

template<typename T>
class SharedPointer;

// Weak pointer class

template <typename T>
class WeakPointer
{
private:
    typedef WeakPointer<T> this_type;

public:
    typedef T element_type;

public:
    // constructors

    WeakPointer()
        : m_pointer( NULL ), m_weak_count()
    {}

    WeakPointer( const WeakPointer& other )
        : m_pointer( other.m_pointer ), m_weak_count( other.m_weak_count )
    {}

    template <typename U>
    WeakPointer( const WeakPointer<U>& other )
        : m_pointer( other.m_pointer ), m_weak_count( other.m_weak_count )
    {}

    template <typename U>
    WeakPointer( const SharedPointer<U>& other )
        : m_pointer( other.m_pointer ), m_weak_count( other.m_shared_count )
    {}

public:
    // assingment

    this_type& operator =( const this_type& rhs )
    {
        this_type tmp( rhs );
        tmp.swap( *this );
        return *this;
    }

    template <typename U>
    this_type& operator =( const WeakPointer<U>& rhs )
    {
        this_type tmp( rhs );
        tmp.swap( *this );
        return *this;
    }

    template <typename U>
    this_type& operator =( const SharedPointer<U>& rhs )
    {
        this_type tmp( rhs );
        tmp.swap( *this );
        return *this;
    }

public:
    // modifiers

    void reset()
    {
        this_type tmp;
        tmp.swap( *this );
    }

    void swap( this_type& other )
    {
        std::swap( m_pointer, other.m_pointer );
        m_weak_count.swap( other.m_weak_count );
    }

public:
    // observers

    long use_count() const
    {
        return m_weak_count.use_count();
    }

    bool expired() const
    {
        return m_weak_count.expired();
    }

    // Makes SharedPointer instance from this. If this WeakPointer already have been expired, returns empty SharedPointer.
    SharedPointer<T> lock() const
    {
        detail::shared_pointer_impl::SharedCount shared_count( m_weak_count );
        if ( !shared_count.is_valid() )
        {
            SharedPointer<T> ret;
            return ret;
        }

        SharedPointer<T> ret( m_pointer, shared_count );
        return ret;
    }

private:
    T* m_pointer;
    detail::shared_pointer_impl::WeakCount m_weak_count;

    template<typename U>
    friend class SharedPointer;
    template <typename U>
    friend class WeakPointer;
}; // WeakPointer

} // kvs

namespace std
{

template <typename T>
inline void swap( kvs::WeakPointer<T>& w1, kvs::WeakPointer<T>& w2 )
{
    w1.swap( w2 );
}

} // std

#endif
