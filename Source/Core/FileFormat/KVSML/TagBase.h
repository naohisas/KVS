/*****************************************************************************/
/**
 *  @file   TagBase.h
 */
/*----------------------------------------------------------------------------
 *
 *  Copyright (c) Visualization Laboratory, Kyoto University.
 *  All rights reserved.
 *  See http://www.viz.media.kyoto-u.ac.jp/kvs/copyright/ for details.
 *
 *  $Id: TagBase.h 1348 2012-11-13 07:05:59Z s.yamada0808@gmail.com $
 */
/*****************************************************************************/
#pragma once
#include <kvs/XMLNode>
#include <kvs/XMLElement>
#include <string>
#include <kvs/Assert>
#include <kvs/String>
#include <kvs/Exception>


namespace kvs
{

namespace kvsml
{

struct TagAttributeNotSpecifiedException : public kvs::Exception
{
    explicit TagAttributeNotSpecifiedException( const std::string& message )
        : kvs::Exception( message ) {}
};

class TagAttributeBase
{
private:
    const char* m_name;

protected:
    bool m_has_value;

protected:
    explicit TagAttributeBase( const char* name ) : 
        m_name( name ), 
        m_has_value( false )
    {
    }

public:
    const char* name() const
    {
        return m_name;
    }

    bool hasValue() const
    {
        return m_has_value;
    }

    void throw_expection() const
    {
        KVS_THROW( kvs::kvsml::TagAttributeNotSpecifiedException, m_name );
    }

private:
    TagAttributeBase( const TagAttributeBase& );
    TagAttributeBase& operator =( const TagAttributeBase& );
};

template <typename T>
class TagAttribute : public TagAttributeBase
{
private:
    T m_value;

public:
    explicit TagAttribute( const char* name ) : 
        TagAttributeBase( name )
    {
    }

    void operator =( const T& value )
    {
        m_has_value = true;
        m_value = value;
    }

    operator const T&() const
    {
        if ( !this->hasValue() )
        {
            this->throw_expection();
        }
        return m_value;
    }

    friend std::ostream& operator <<( std::ostream& os, const TagAttribute& rhs  )
    {
        if ( !rhs.hasValue() )
        {
            rhs.throw_expection();
        }
        return os << rhs.m_value;
    }
};

/*===========================================================================*/
/**
 *  @brief  Base class of the tag.
 */
/*===========================================================================*/
class TagBase
{
protected:
    std::string m_name; ///< tag name
    kvs::XMLNode::SuperClass* m_node; ///< pointer to the node of the XML tree

protected:
    explicit TagBase( const std::string& name );

public:
    kvs::XMLNode::SuperClass* node();
    const kvs::XMLNode::SuperClass* node() const;
    const std::string& name() const;
    bool isExisted( const kvs::XMLNode::SuperClass* parent ) const;
    bool exists() const;

public:
    bool read( const kvs::XMLNode::SuperClass* parent );
    bool write( kvs::XMLNode::SuperClass* parent );

protected:
    bool write_with_element( kvs::XMLNode::SuperClass* parent, const kvs::XMLElement& element );

private:
    TagBase();
};

} // end of namespace kvsml

} // end of namespace kvs
