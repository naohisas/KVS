/*****************************************************************************/
/**
 *  @file   SharedPointer.h
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
#ifndef KVS_SHARED_POINTER_H_INCLUDE
#define KVS_SHARED_POINTER_H_INCLUDE


#include <iostream>
#include <utility>
#include <kvs/Assert>
#include "SharedCount.h"


namespace kvs
{

namespace temporal
{

template <typename T> struct ref { typedef T& reference; };
template <> struct ref<void> { typedef void reference; };
template <> struct ref<const void> { typedef void reference; };
template <> struct ref<volatile void> { typedef void reference; };
template <> struct ref<const volatile void> { typedef void reference; };

}

template <typename T>
class WeakPointer;

template <typename T>
class SharedPointer
{
public:
    typedef T element_type;

private:
    typedef SharedPointer<T> this_type;

    // to avoid void&
    typedef typename kvs::temporal::ref<T>::reference reference;

public:
    // constructors

/*==========================================================================*/
/**
 *  Constructs a empty SharedPointer object.
 */
/*==========================================================================*/
    SharedPointer()
        : m_pointer( NULL ), m_shared_count()
    {}

/*==========================================================================*/
/**
 *  Constructs a SharedPointer object that owns the pointer \p p.
 *  @param p [in] raw pointer
 */
/*==========================================================================*/
    template <typename Y>
    explicit SharedPointer( Y* p )
        : m_pointer( p ), m_shared_count( p )
    {
        // @TODO do_enable_shared;
    }

/*==========================================================================*/
/**
 *  Constructs a SharedPointer object that owns the pointer \p p and deleter \p d.
 *  @param p [in] raw pointer
 *  @param d [in] deleter object against \p p
 */
/*==========================================================================*/
    template <typename Y, typename D>
    SharedPointer( Y* p, D d )
        : m_pointer( p ), m_shared_count( p, d )
    {
        // @TODO do_enable_shared;
    }

/*==========================================================================*/
/**
 *  Constructs a copy of \p r.
 *  @param other [in] shared pointer
 */
/*==========================================================================*/
    SharedPointer( const SharedPointer& r )
        : m_pointer( r.m_pointer ), m_shared_count( r.m_shared_count )
    {}

    template <typename Y>
    SharedPointer( const SharedPointer<Y>& r )
        : m_pointer( r.m_pointer ), m_shared_count( r.m_shared_count )
    {}

/*==========================================================================*/
/**
 *  Constructs a SharedPointer instance that stores \p p and shares ownership with \p r.
 */
/*==========================================================================*/
    template <typename Y>
    SharedPointer( const SharedPointer<Y>& r, T* p )
        : m_pointer( p ), m_shared_count( r.m_shared_count )
    {}

public:
    // assingment

    this_type& operator =( const this_type& rhs )
    {
        this_type tmp( rhs );
        tmp.swap( *this );
        return *this;
    }

    template <typename Y>
    this_type& operator =( const SharedPointer<Y>& rhs )
    {
        this_type tmp( rhs );
        tmp.swap( *this );
        return *this;
    }

public:
    // modifiers

    void swap( this_type& other )
    {
        std::swap( m_pointer, other.m_pointer );
        m_shared_count.swap( other.m_shared_count );
    }

// sp.reset()
// is equivarent to
// SharedPointer().swap( sp )
    void reset()
    {
        this_type tmp;
        tmp.swap( *this );
    }

// sp.reset( p )
// is equivarent to
// SharedPointer( p ).swap( sp )
    template <typename Y>
    void reset( Y* ptr )
    {
        KVS_ASSERT( ptr == NULL || m_pointer != ptr );
        this_type tmp( ptr );
        tmp.swap( *this );
    }

// sp.reset( p, d )
// is equivarent to
// SharedPointer( p, d ).swap( sp )
    template <typename Y, typename D>
    void reset( Y* ptr, D deleter )
    {
        KVS_ASSERT( ptr == NULL || m_pointer != ptr );
        this_type tmp( ptr, deleter );
        tmp.swap( *this );
    }

public:
    // observers

// Returns raw pointer.
    T* get() const
    {
        return m_pointer;
    }

    reference operator *() const
    {
        KVS_ASSERT( m_pointer != NULL );
        return *m_pointer;
    }

    T* operator ->() const
    {
        KVS_ASSERT( m_pointer != NULL );
        return m_pointer;
    }

    long use_count() const
    {
        return m_shared_count.use_count();
    }

// sp.unique()
// is equivarent to
// sp.use_count() == 1
// Note: unique() maybe faster than use_count() == 1. 
//       If you are using unique() to implement copy on write, 
//       do not rely on a specific value when get() == 0.
    bool unique() const
    {
        return this->use_count() == 1;
    }

    operator bool() const
    {
        return m_pointer != NULL;
    }

private:
// for WeakPointer.lock().
    SharedPointer( T* pointer, const detail::shared_pointer_impl::SharedCount& shared_count )
        : m_pointer( pointer ), m_shared_count( shared_count )
    {}

public:
// for get_deleter()
    template<typename D>
    D* _get_deleter_impl() const
    {
        return reinterpret_cast<D*>( m_shared_count.deleter( typeid( D ) ) );
    }

private:
    T* m_pointer;
    detail::shared_pointer_impl::SharedCount m_shared_count;

    template<typename U>
    friend class SharedPointer;
    template <typename U>
    friend class WeakPointer;
}; // SharedPointer


// Non-member operator functions. //
template <typename T, typename U>
inline SharedPointer<T> const_pointer_cast( const SharedPointer<U>& other )
{
    T* ptr = const_cast<T*>( other.get() );
    SharedPointer<T> ret( other, ptr );
    return ret;
}

template <typename T, typename U>
inline SharedPointer<T> static_pointer_cast( const SharedPointer<U>& other )
{
    T* ptr = static_cast<T*>( other.get() );
    SharedPointer<T> ret( other, ptr );
    return ret;
}

template <typename T, typename U>
inline SharedPointer<T> dynamic_pointer_cast( const SharedPointer<U>& other )
{
    T* ptr = dynamic_cast<T*>( other.get() );
    if ( ptr == NULL )
    {
        SharedPointer<T> ret;
        return ret;
    }

    SharedPointer<T> ret( other, ptr );
    return ret;
}

template <typename D, typename T>
inline D* get_deleter( const SharedPointer<T>& sp )
{
    return sp.template _get_deleter_impl<D>();
}

template <typename T, typename U>
inline bool operator ==( const SharedPointer<T>& left, const SharedPointer<U>& right )
{
    return left.get() == right.get();
}

template <typename T, typename U>
inline bool operator !=( const SharedPointer<T>& left, const SharedPointer<U>& right )
{
    return !( left == right );
}

template <typename T>
inline std::ostream& operator <<( std::ostream& os, const SharedPointer<T>& lhs )
{
    return os << lhs.get();
}

} // kvs

namespace std
{

template <typename T>
inline void swap( kvs::SharedPointer<T>& s1, kvs::SharedPointer<T>& s2 )
{
    s1.swap( s2 );
}

} // std

#endif
